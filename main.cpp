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

///Read program from user
void getProgram() {
    std::cin >> program;
}

///Read data from user
void getInputData() {
    std::cin >> inputData;
}

void init() {
    current_step_pointer = 0;
    processorStack = new stack();
    getProgram();
}
int listToIntRecursive(const list_node *node, const int multiplier) {
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

void reverseStrRecursive(char *str, int start, int end) {
    if (start >= end) {
        return;
    }
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    reverseStrRecursive(str, start + 1, end - 1);
}

void intToStrRecursive(int num, char *str, int &i) {
    if (num == 0) {
        return;
    }
    str[i++] = (num % 10) + '0';
    intToStrRecursive(num / 10, str, i);
}

void intToStr(int num, char *str) {
    int i = 0;

    // Take absolute value
    bool isNegative = num < 0;
    if (isNegative) num = -num;

    if (num == 0) {
        str[i++] = '0';
    } else {
        intToStrRecursive(num, str, i);
    }

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';
}

void handleNumberInsertion(const char *number) {
    list *newList = new list();
    for (int i = getStringLength(number) - 1; i >= 0; --i) {
        newList->add(number[i]);
    }
    processorStack->push(newList);
}

/**
 * Add 2 numbers on top of the stack. Remove them, add new number on top.
 */
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

/**
 * Remove top number from top of stack (A).
 * Put copy of number on position A on top of the stack.
 */
void handleAtSymbol() {
    list *listA = processorStack->pop();

    int A = listToInt(listA);

    list *list_on_position = processorStack->getListByPosition(A);
    processorStack->push(list_on_position);
}

void printChar(const char c) {
    std::cout << c;
}

/**
 * Remove number on top of the stack (A).
 * Put on top of the stack char of number A.
 */
void handleRightBracket() {
    list *listA = processorStack->pop();
    int A = listToInt(listA);
    char asciiChar = static_cast<char>(A);
    list *newList = new list();
    newList->add(asciiChar);
    processorStack->push(newList);
}


// TODO commets
void handleLeftBracketRecursive(list *newList, const char *asciiStr, int index) {
    if (index >= getStringLength(asciiStr)) {
        processorStack->push(newList);
        return;
    }
    newList->add(asciiStr[index], newList->getSize());
    handleLeftBracketRecursive(newList, asciiStr, index + 1);
}


/**
 * Remove number on top of the stack (A).
 * Put on top of the stack number of the value of the first from char A.
 */
void handleLeftBracket() {
    list *listA = processorStack->pop();
    if (listA->getSize() == 0) {
        throw std::out_of_range("List is empty handleLeftBracket main.cpp");
    }

    char firstChar = listA->getListElement(0);
    int firstCharValue = static_cast<int>(firstChar);
    char asciiStr[20];
    intToStr(firstCharValue, asciiStr);

    list *newList = new list();
    handleLeftBracketRecursive(newList, asciiStr, 0);
}


/**
 * Get fist char from number on top of the stack.
 * Put it on top of the stack.
 */
void handleDolarSymbol() {
    char firstChar = processorStack->getListByPosition()->popListElement();
    list *newList = new list();
    newList->add(firstChar);
    processorStack->push(newList);
}


/**
 * Remove number A from top on the stack.
 * Put A on the on of number B (second number on stack)
 */
void handleHashSymbol() {
    list *listA = processorStack->pop();
    list *listB = processorStack->getListByPosition();

    listB->mergeLists(listA);
}

/**
 * Add empty list to processor stack
 */
void handleApostrofSumbol() {
    processorStack->push(new list());
};


/**
 * Remove first element on processor stack.
 */
void handleCommaSymbol() {
    processorStack->pop();
};


/**
 * Put on stack copy of the list form top of stack.
 */
void handleColonSymbol() {
    list *top = processorStack->getListByPosition();
    list *newList = new list(*top);
    processorStack->push(newList);
}

/**
 * Swap first and second element on top of processor stack.
 */
void handleSemicolon() {
    processorStack->flipTopList();
}

/**
 * Print the stack.
 */
void handleAmpersandSymbol() {
    std::cout << *processorStack;
}


/**
 * Add '-' to the end of the list on top of the stack.
 * If '-' already present remove it.
 */
void handleMinusSymbol() {
    list *top = processorStack->getListByPosition();
    if (top != nullptr && top->getSize() > 0 && top->getListElement(top->getSize() - 1) == '-') {
        top->remove(top->getSize() - 1);
    } else {
        top->add('-', top->getSize());
    }
}

/**
* Read sign from inputData, add it to the list on top of the stack.
*
* @param isInputDataRead if set to false program will load data from std in.
* @param inputDataIndex pointer to current char in inputData tabele.
*/
void handleDotSymbol(bool &isInputDataRead, int &inputDataIndex) {
    if (!isInputDataRead) {
        getInputData();
        isInputDataRead = true;
    }
    processorStack->getListByPosition()->add(inputData[inputDataIndex]);
    inputDataIndex++;
}


/**
 *  Take absolute value of number on top of processor stack.
 */
void handleCaretSymbol() {
    list *top = processorStack->getListByPosition();
    if (top->getSize() > 0 && top->getListElement(top->getSize() - 1) == '-') {
        top->remove(top->getSize() - 1);
    }
}

/**
 * Compare 2 top numbers A B. Remove them from stack.
 * If A < B put 1 on top otherwise put 0.
 */
void handleLessSymbol() {
    int A = listToInt(processorStack->pop());
    int B = listToInt(processorStack->pop());

    list *lst = new list();
    lst->add((A < B) + '0');

    processorStack->push(lst);
};

bool compareListsRecursive(const list_node *nodeA, const list_node *nodeB) {
    if (nodeA == nullptr && nodeB == nullptr) {
        return true;
    }
    if (nodeA == nullptr || nodeB == nullptr || nodeA->data != nodeB->data) {
        return false;
    }
    return compareListsRecursive(nodeA->next, nodeB->next);
}

bool compareLists(const list *listA, const list *listB) {
    return compareListsRecursive(listA->head, listB->head);
}

/**
 * Compare 2 top numbers A B. Remove them from stack.
 * If A = B put 1 on top otherwise put 0.
 */
void handleEqualSymbol() {
    list *listA = processorStack->pop();
    list *listB = processorStack->pop();

    compareLists(listA, listB) ? listA->add('1') : listA->add('0');
};

void handleWykrzyknikSymbol() {
    list *lst = processorStack->pop();
    list *newList = new list();

    if (lst->empty() or (lst->getSize() == 1 and lst->getListElement(0) == '0')) {
        newList->add('1');
        processorStack->push(newList);
        return;
    }

    newList->add('0');
    processorStack->push(newList);
};

void handleTyldaSymbol(int number) {
    char str[10];
    intToStr(number, str);
    handleNumberInsertion(str);
};

bool handleQuestionMarkSymbol() {
    return true;
};

int main() {
    init();

    bool isInputDataRead = false;
    bool increseInstructionPointer = true;
    int inputDataIndex = 0;

    do {
        switch (program[current_step_pointer]) {
            case '\'':
                handleApostrofSumbol();
                break;
            case ',':
                handleCommaSymbol();
                break;
            case ':':
                handleColonSymbol();
                break;
            case ';':
                handleSemicolon();
                break;
            case '@':
                handleAtSymbol();
                break;
            case '&':
                handleAmpersandSymbol();
                break;
            case '-':
                handleMinusSymbol();
                break;
            case '+':
                handlePlusSymbol();
                break;
            case '.':
                handleDotSymbol(isInputDataRead, inputDataIndex);
                break;
            case '^':
                handleCaretSymbol();
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
            case '<':
                handleLessSymbol();
                break;
            case '=':
                handleEqualSymbol();
                break;
            case '!':
                handleWykrzyknikSymbol();
                break;
            case '~':
                handleTyldaSymbol(current_step_pointer);
                break;
            // case '?':
            //     increseInstructionPointer = handleQuestionMarkSymbol();
            //     break;
            default:
                processorStack->getListByPosition()->add(program[current_step_pointer]);
        }
        if (increseInstructionPointer) current_step_pointer++;

    } while (program[current_step_pointer] != '\0');

    return 0;
}
