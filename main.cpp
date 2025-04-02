#include "stack.h"

char program[20000];
///< Pamięć programu przechowuje program. Program jest sekwencją instrukcji, a każda instrukcja jest jednym znakiem.

char inputData[20000];
///< Pamięć danych przechowuje dane wejściowe. Dane wejściowe są sekwencją znaków, a każdy znak jest jednym bajtem.

int current_step_pointer;
///< Wskaźnik instrukcji przechowuje numer instrukcji, która będzie wykonana następna i zwiększa się o jeden po jej wykonaniu (w przypadku większości instrukcji).

stack *processorStack;
///< Stos procesora.

int getStringLength(const char *str, int acc = 0) {
    if (*str == '\0') {
        return acc;
    }
    return getStringLength(str + 1, acc + 1);
}

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
    bool isNegative = (lst->getListElement(lst->getSize() - 1) == '-');
    int result = listToIntRecursive(lst->head, 1);
    return isNegative ? -result : result;
}

void intToStr(int num, char *str) {
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

void handleNumberInsertion(const char *number) {
    list *newList = new list();
    for (int i = getStringLength(number) - 1; i >= 0; --i) {
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

void printChar(const char c) {
    std::cout << c;
}

void handleRightBracket() {
    list *listA = processorStack->pop();
    int A = listToInt(listA);
    char asciiChar = static_cast<char>(A);
    list *newList = new list();
    newList->add(asciiChar);
    processorStack->push(newList);
}

void handleLeftBracketRecursive(list *newList, const char *asciiStr, int index) {
    if (index < 0) {
        processorStack->push(newList);
        return;
    }
    newList->add(asciiStr[index], newList->getSize());
    handleLeftBracketRecursive(newList, asciiStr, index - 1);
}

void handleLeftBracket() {
    list *listA = processorStack->pop();
    if (listA->getSize() == 0) {
        throw std::out_of_range("List is empty");
    }

    char firstChar = listA->getListElement(0);
    int firstCharValue = static_cast<int>(firstChar);
    char asciiStr[20];
    intToStr(firstCharValue, asciiStr);

    list *newList = new list();
    handleLeftBracketRecursive(newList, asciiStr, getStringLength(asciiStr) - 1);
}

void handleDolarSymbol() {
    char firstChar = processorStack->getListByPosition()->popListElement();
    list *newList = new list();
    newList->add(firstChar);
    processorStack->push(newList);
}

void handleHashSymbol() {
    list *listA = processorStack->pop();
    list *listB = processorStack->getListByPosition();

    listB->mergeLists(listA);
}

int main() {
    init();

    list *top = nullptr;
    list *newList = nullptr; // Deklaracja zmiennej przed instrukcją switch

    bool isInputDataRead = false;
    int inputDataIndex = 0;

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
                if (top != nullptr and top->getSize() > 0 and top->getListElement(top->getSize() - 1) == '-') {
                    top->remove(top->getSize() - 1);
                } else {
                    top->add('-', top->getSize());
                }
                break;
            case '+':
                handlePlusSymbol();
                break;
            case '.':
                if (!isInputDataRead) {
                    getInputData();
                    isInputDataRead = true;
                }
                processorStack->getListByPosition()->add(inputData[inputDataIndex]);
                inputDataIndex++;
                break;
            case '^':
                top = processorStack->getListByPosition();
                if (top->getSize() > 0 and top->getListElement(top->getSize() - 1) == '-') {
                    top->remove(top->getSize() - 1);
                }
                break;
            case '>':
                printChar(processorStack->getListByPosition()->popListElement());
                processorStack->pop();
                break;
            case ']':
                handleRightBracket();
                break;
            case '[':
                handleLeftBracket();
            break;
            case '$':
                handleDolarSymbol();
            break;
            case '#':
                handleHashSymbol();
            break;
            default:
                processorStack->getListByPosition()->add(program[current_step_pointer]);
        }
        current_step_pointer++;
    } while (program[current_step_pointer] != '\0');

    return 0;
}
