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

void handleNumberInsertionRecursive(list *newList, const char *number, int index) {
    if (index < 0) {
        processorStack->push(newList);
        return;
    }
    newList->add(number[index]);
    handleNumberInsertionRecursive(newList, number, index - 1);
}

void handleNumberInsertion(const char *number) {
    list *newList = new list();
    handleNumberInsertionRecursive(newList, number, getStringLength(number) - 1);
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

/**
 * @brief Recursively handles the insertion of ASCII string characters into a list.
 *
 * This function inserts characters from the ASCII string `asciiStr` into the list `newList`
 * starting from the specified `index`. The function continues recursively until all characters
 * are added to the list.
 *
 * @param newList The list to which characters are added.
 * @param asciiStr The ASCII string whose characters are to be added to the list.
 * @param index The current index in the ASCII string.
 */
void insertAsciiStringRecursive(list *newList, const char *asciiStr, int index) {
    if (index >= getStringLength(asciiStr)) {
        processorStack->push(newList);
        return;
    }
    newList->add(asciiStr[index], newList->getSize());
    insertAsciiStringRecursive(newList, asciiStr, index + 1);
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
    insertAsciiStringRecursive(newList, asciiStr, 0);
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
 * Put A on the on top of number B (second number on stack)
 */
void handleHashSymbol() {
    list *listA = processorStack->pop();
    list *listB = processorStack->getListByPosition();

    listB->mergeLists(listA);
}

/**
 * Add empty list to processor stack
 */
void handleApostrofSymbol() {
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

    if (top == nullptr) throw std::out_of_range("List is empty handleMinusSymbol main.cpp");

    if (top->getSize() > 0 and top->getListElement(top->getSize() - 1) == '-') {
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
    if (top->getSize() > 0 and top->getListElement(top->getSize() - 1) == '-') {
        top->remove(top->getSize() - 1);
    }
}

/**
 * @brief Removes trailing zeros from a string representing a number.
 *
 * This function removes any trailing zeros from the input string `str`.
 * If the string ends with a minus sign after removing zeros, it includes the minus sign.
 *
 * @param str The input string representing a number.
 * @return The modified string with trailing zeros removed.
 */
char *removeTrailingZeros(char *str) {
    int length = getStringLength(str);
    if (length == 0) return str;

    // Find the position of the last non-zero character
    int index = length - 1;
    if (str[index] == '0') {
        str[index] = '\0';
        return removeTrailingZeros(str);
    }

    // If the string ends with a minus sign, include it
    if (index > 0 and str[index] == '-' and str[index - 1] == '0') {
        str[index - 1] = '-';
        str[index] = '\0';
        return removeTrailingZeros(str);
    }

    reverseStrRecursive(str, 0, getStringLength(str) - 1);
    return str;
}

int compareStringNumbersRecursive(const char *A, const char *B, int index) {
    if ((A[index] == '\0' and B[index] == '\0') or
        (A[index] == '\0' and B[index] == '-') or
        (B[index] == '\0' and A[index] == '-'))
        return -1; // They are equal

    if (A[index] != B[index]) return B[index] < A[index];

    return compareStringNumbersRecursive(A, B, index + 1);
}


/**
 * @brief Compares two string representations of numbers.
 *
 * @param A The first string number to compare.
 * @param B The second string number to compare.
 * @return -1 when A == B, 0 when A < B, 1 when A > B.
 */
int compareStringNumbers(char *A, char *B) {
    // Check if both strings are empty or equal to "-"
    if ((A[0] == '\0' or (A[0] == '-' and A[1] == '\0')) and
        (B[0] == '\0' or (B[0] == '-' and B[1] == '\0'))) {
        return -1;
    }

    bool isANegative = (A[0] == '-') or (A[getStringLength(A) - 1] == '-');
    bool isBNegative = (B[0] == '-') or (B[getStringLength(B) - 1] == '-');

    // Handle cases where one or both numbers are negative
    if (isANegative and !isBNegative) return 0;
    if (!isANegative and isBNegative) return 1;

    if (isANegative and isBNegative) {
        // Both are negative, compare absolute values
        A++;
        B++;
        return compareStringNumbers(B, A); // Reverse comparison for negative numbers
    }

    // Compare lengths
    int lenA = getStringLength(A);
    int lenB = getStringLength(B);
    if (lenA != lenB) {
        return lenB < lenA;
    }

    // Compare lexicographically using recursion
    return compareStringNumbersRecursive(A, B, 0);
}

/**
 * Compare 2 top numbers A B. Remove them from stack.
 * If A = B put 1 on top otherwise put 0.
 */
void handleEqualSymbol() {
    list *listA = processorStack->pop();
    list *listB = processorStack->pop();

    char *A = listA->getString();
    char *B = listB->getString();

    A = removeTrailingZeros(A);
    B = removeTrailingZeros(B);

    list *lst = new list();
    compareStringNumbers(A, B) == -1 ? lst->add('1') : lst->add('0');
    processorStack->push(lst);
};

/**
 * Compare 2 top numbers A B. Remove them from stack.
 * If A < B put 1 on top otherwise put 0.
 */
void handleLessSymbol() {
    char *A = processorStack->pop()->getString();
    char *B = processorStack->pop()->getString();

    A = removeTrailingZeros(A);
    B = removeTrailingZeros(B);

    list *lst = new list();

    int result = compareStringNumbers(A, B);
    lst->add(result == 1 ? '1' : '0');

    processorStack->push(lst);
}

bool isListIsEmptyOrZero(list *lst) {
    if (lst->empty()) {
        return true;
    }
    if (lst->getSize() == 1 and lst->getListElement(0) == '0') {
        return true;
    }
    return false;
}

void handleWykrzyknikSymbol() {
    list *lst = processorStack->pop();
    list *newList = new list();

    if (isListIsEmptyOrZero(lst)) {

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


/**
 * @brief Conditional jump operation.
 *
 * This function performs a conditional jump based on the top elements of the stack.
 * It pops a number T from the stack, then pops a list W from the stack.
 * If the list W is not empty and does not contain only the character '0',
 * it sets the instruction pointer to the value of T and does not increment the instruction pointer.
 */
bool handleQuestionMarkSymbol(int &current_step_pointer) {
    list *lstT = processorStack->pop();
    list *lstW = processorStack->pop();

    if (isListIsEmptyOrZero(lstW)) return true;

    current_step_pointer = listToInt(lstT);

    return false;
};

int main() {
    init();

    bool isInputDataRead = false;
    int inputDataIndex = 0;

    do {
        bool increaseInstructionPointer = true;
        switch (program[current_step_pointer]) {
            case '\'':
                handleApostrofSymbol();
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
            case '?':
                increaseInstructionPointer = handleQuestionMarkSymbol(current_step_pointer);
                break;
            default:
                processorStack->getListByPosition()->add(program[current_step_pointer]);
        }
        if (increaseInstructionPointer) current_step_pointer++;
    } while (program[current_step_pointer] != '\0');

    return 0;
}
