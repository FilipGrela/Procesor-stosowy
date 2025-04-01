#include <string.h>

#include "stack.h"

// '&1&2&3&'&4&5&6&'&7&8&9&:&:&;&;&,&,&;&;&'0&@&,&'1&@&,&'2&@&,&,,,'9999999&
// '0&@&,&'1&@&,&'2&@&,&,,,'9999999&

char program[20000];
///< Pamięć programu przechowuje program. Program jest sekwencją instrukcji, a każda instrukcja jest jednym znakiem.

char inputData[20000];
///< Pamięć danych przechowuje dane wejściowe. Dane wejściowe są sekwencją znaków, a każdy znak jest jednym bajtem.

int current_step_pointer;
///< Wskaźnik instrukcji przechowuje numer instrukcji, która będzie wykonana następna i zwiększa się o jeden po jej wykonaniu (w przypadku większości instrukcji).

stack *processorStack;
///< Stos procesora.

void getProgram() {
    std::cin >> program;
}

void getInputData() {
    std::cin >> inputData;
}

void init() {
    current_step_pointer = 0;
    processorStack = new stack();
    getProgram();
    // getInputData();
}

int listToIntRecursive(list_node *node, int multiplier) {
    if (node == nullptr or node->data == '-') {
        return 0;
    }
    int digit = node->data - '0';
    return digit * multiplier + listToIntRecursive(node->next, multiplier * 10);
}

int listToInt(list *lst) {
    if (lst->getSize() == 0) {
        return 0;
    }
    bool isNegative = (lst->getListElement(lst->getSize()-1) == '-');
    int result = listToIntRecursive(lst->head, 1);
    return isNegative ? -result : result;
}

void intToStr(int num, char* str) {
    int i = 0;
    bool isNegative = false;

    if (num < 0) {
        isNegative = true;
        num = -num;
    }

    do {
        str[i++] = (num % 10) + '0';
        num /= 10;
    } while (num != 0);

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    for (int j = 0; j < i / 2; ++j) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}

void handleNumberInsertion(const char* number) {
    list *newList = new list();
    for (int i = strlen(number) - 1; i >= 0; --i) {
        newList->add(number[i]);
    }
    processorStack->push(newList);
}

void handlePlusSymbol() {
    list *listA = processorStack->pop();
    list *listB = processorStack->pop();

    int A = listToInt(listA);
    int B = listToInt(listB);

    int C = A + B;
    char result[20];
    intToStr(C, result);

    handleNumberInsertion(result);
}

void handleAtSymbol() {
    list *listA = processorStack->pop();

    int A = listToInt(listA);

    list *list_on_position = processorStack->getListByPosition(A);
    processorStack->push(list_on_position);
}


int main() {
    init();

    list *top = nullptr;
    list *newList = nullptr; // Deklaracja zmiennej przed instrukcją switch

    do {
        switch (program[current_step_pointer]) {
            case '\'':
                processorStack->push(new list());
                break;
            case ',':
                processorStack->pop();
                break;
            case ':':
                top = processorStack->getListByPosition();
                newList = new list(*top);
                processorStack->push(newList);
                break;
            case ';':
                processorStack->flipTopList();
                break;
            case '@':
                handleAtSymbol();
                break;
            case '&':
                std::cout << *processorStack;
                break;
            case '-':
                top = processorStack->getListByPosition();
                if (top->getSize() > 0 and top->getListElement(0) == '-') {
                    top->popListElement();
                } else {
                    top->add('-');
                }
                break;
            case '+':
                handlePlusSymbol();
                break;
            default:
                processorStack->getListByPosition()->add(program[current_step_pointer]);
        }
        current_step_pointer++;
    } while (current_step_pointer <= strlen(program));

    return 0;
}
