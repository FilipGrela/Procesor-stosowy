#include "stack.h"

/// The program memory stores the program. The program is a sequence of instructions, and each instruction is a single character.
char program[20000];

/// The instruction pointer stores the number of the instruction that will be executed next and increments by one after its execution (for most instructions).
int current_step_pointer;

stack *processorStack;

// ╔══════════════════════════════════╗
// ║                                  ║
// ║       Input / Output func.       ║
// ║                                  ║
// ╚══════════════════════════════════╝

/**
 * Reads the program from the user.
 */
void getProgram() {
    std::cin >> program;
}

/**
 * Reads the input data from the user.
 */
char getInputData() {
    return std::cin.get();
}

/// Print a character to the console.
void printChar(const char c) {
    std::cout << c;
}

// ╔══════════════════════════════════╗
// ║                                  ║
// ║            Utilities             ║
// ║                                  ║
// ╚══════════════════════════════════╝

// ==============================
//     Operations on strings
// ==============================

/**
 * Counts the length of a string recursively.
 *
 * @param str String to check
 * @return Length of the string
 */
int getStringLength(const char *str, int acc = 0) {
    if (str == nullptr or *str == '\0') {
        return acc;
    }
    return getStringLength(str + 1, acc + 1);
}

/**
* Reverses a string recursively.
*
* @param str String to reverse
* @param start Start index
* @param end End index
*/
void reverseString(char *str, int start, int end) {
    end--;
    if (getStringLength(str) <= 1) {
        return;
    }

    if (str[end] == '-') {
        end--;
    }

    if (start >= end) {
        return;
    }
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    reverseString(str, start + 1, end);
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
char *removeTrailingZeros(char *str, bool reverse = true) {
    int length = getStringLength(str);
    if (length == 0 ||
        (length == 1 && str[0] == '0') ||
        (length == 2 && str[0] == '0' && str[1] == '-')) {
        str[1] = '\0';
        return str;
    }

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

    reverse ? reverseString(str, 0, length) : void(0);
    return str;
}

// ==============================
//           Conversions
// ==============================

int listToIntRecursive(const list_node *node, const int multiplier) {
    if (node == nullptr or node->data == '-') {
        return 0;
    }
    int digit = node->data - '0';

    return digit * multiplier + listToIntRecursive(node->next, multiplier * 10);
}

/**
 * Convert a list of characters to an integer.
 *
 * @param lst List to convert
 * @return Integer value of the list
 */
int listToInt(list *lst) {
    if (lst->getSize() == 0) {
        return 0;
    }
    bool isNegative = (lst->getListElement(lst->getSize() - 1) == '-');
    int result = listToIntRecursive(lst->head, 1);
    return isNegative ? -result : result;
}

/**
 * Reverse a string recursively.
 * Function always puts '-' at the end of the string if it is present.
 *
 * @param str String to reverse
 * @param start Start index
 * @param end End index
 */
void intToStrRecursive(int num, char *str, int &i) {
    if (num == 0) {
        return;
    }
    str[i++] = (num % 10) + '0';
    intToStrRecursive(num / 10, str, i);
}

/**
 * Convert an integer to a string.
 *
 * @param num Integer to convert
 * @param str String to store the result
 */
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

// ==============================
//        Operations on lists
// ==============================

void handleNumberInsertionRecursive(list *newList, const char *number, int index) {
    if (index < 0) {
        processorStack->push(newList);
        return;
    }
    newList->add(number[index]);
    handleNumberInsertionRecursive(newList, number, index - 1);
}

/**
 * Insert a number into the stack.
 *
 * @param number Number to insert
 */
void handleNumberInsertion(const char *number) {
    list *newList = new list();
    handleNumberInsertionRecursive(newList, number, getStringLength(number) - 1);
}

/**
 * Add empty list to processor stack
 */
void addEmptyList() {
    processorStack->push(new list());
};

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
 * Check if the list is empty or contains only a single '0' character.
 *
 * @param lst The list to check.
 * @return true if the list is empty or contains only '0', false otherwise.
 */
bool isListIsEmptyOrZero(list *lst) {
    if (lst->empty()) {
        return true;
    }
    if (lst->getSize() == 1 and lst->getListElement(0) == '0') {
        return true;
    }
    return false;
}

// ==============================
//          Comparisons
// ==============================

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
    if ((A[0] == '\0' and B[0] == '\0') or
        (A[0] == '\0' and B[0] == '-') or
        (B[0] == '\0' and A[0] == '-')) {
        return -1;
    }

    int lengthA = getStringLength(A);
    int lengthB = getStringLength(B);

    bool isANegative = (A[0] == '-') or (A[lengthA - 1] == '-');
    bool isBNegative = (B[0] == '-') or (B[lengthB - 1] == '-');

    // Handle cases where one or both numbers are negative
    if (isANegative and !isBNegative) return 0;
    if (!isANegative and isBNegative) return 1;

    if (isANegative and isBNegative) {
        // Both are negative, compare absolute values
        A[lengthA - 1] = '\0';
        B[lengthB - 1] = '\0';
        return compareStringNumbers(B, A); // Reverse comparison for negative numbers
    }

    // Compare lengths
    if (lengthA != lengthB) {
        return lengthB < lengthA;
    }

    // Compare lexicographically using recursion
    return compareStringNumbersRecursive(A, B, 0);
}

// ==============================
//     Operations on large numbers
// ==============================

void subtractLargeNumbersRecursive(const char *num1, const char *num2, int i, int j, int borrow, char *result, int &k) {
    if (i < 0 && j < 0 && borrow == 0) {
        result[k] = '\0';
        reverseString(result, 0, k);
        return;
    }

    int n1 = (i >= 0) ? num1[i] - '0' : 0;
    int n2 = (j >= 0) ? num2[j] - '0' : 0;
    int current = n1 - n2 - borrow;
    if (current < 0) {
        current += 10;
        borrow = 1;
    } else {
        borrow = 0;
    }
    result[k++] = '0' + current;

    subtractLargeNumbersRecursive(num1, num2, i - 1, j - 1, borrow, result, k);
}

/**
 * Subtract two large numbers represented as strings.
 *
 * @param num1 First number
 * @param num2 Second number
 * @return Result of the subtraction
 */
char *subtractLargeNumbers(char *num1, char *num2) {
    int len1 = getStringLength(num1);
    int len2 = getStringLength(num2);

    // Determine the maximum length and allocate space for the result
    int maxLen = len1 > len2 ? len1 : len2;
    char *result = new char[maxLen + 2]; // +2 for possible '-' sign and '\0'
    int k = 0;

    // Determine if the result should be negative
    bool isNegative = false;
    if (compareStringNumbers(num1, num2) == 0) {
        // Swap the numbers if num1 is smaller than num2
        char *temp = num1;
        int tempLen = len1;
        num1 = num2;
        len1 = len2;
        num2 = temp;
        len2 = tempLen;

        isNegative = true;
    }

    // Perform the subtraction recursively
    subtractLargeNumbersRecursive(num1, num2, len1 - 1, len2 - 1, 0, result, k);

    // Add the negative sign if the result is negative
    if (isNegative) {
        result[k++] = '-';
        result[k] = '\0';
    }

    return result;
}

void addLargeNumbersRecursive(const char *num1, const char *num2, int i, int j, int carry, char *result, int &k) {
    if (i < 0 && j < 0 && carry == 0) {
        result[k] = '\0';
        return;
    }

    int n1 = (i >= 0) ? num1[i] - '0' : 0;
    int n2 = (j >= 0) ? num2[j] - '0' : 0;
    int current = n1 + n2 + carry;
    carry = current / 10;
    current = current % 10;
    result[k++] = '0' + current;

    addLargeNumbersRecursive(num1, num2, i - 1, j - 1, carry, result, k);
}

/**
 * Add two large numbers represented as strings.
 *
 * @param num1 First number
 * @param num2 Second number
 * @return Result of the addition
 */
char *addLargeNumbers(char const *num1, const char *num2) {
    int len1 = getStringLength(num1);
    int len2 = getStringLength(num2);

    // Allocate space for the result, including space for carry-over
    int maxLen = len1 > len2 ? len1 : len2;
    char *result = new char[maxLen + 3]; // +2 for possible carry and '\0'
    int k = 0;

    addLargeNumbersRecursive(num1, num2, len1 - 1, len2 - 1, 0, result, k);

    return result;
}

// ╔══════════════════════════════════╗
// ║                                  ║
// ║         Symbol handlers          ║
// ║                                  ║
// ╚══════════════════════════════════╝

/**
 * Add empty list to processor stack.
 */
void handleApostrofSymbol() {
    addEmptyList();
}

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
    if (processorStack->getSize() == 0) {
        return;
    }
    list *top = processorStack->getListByPosition();
    list *newList = new list(*top);
    processorStack->push(newList);
}

/**
 * Remove top number from top of stack (A).
 * Put copy of number on position A on top of the stack.
 */
void handleAtSymbol() {
    list *listA = processorStack->pop();

    int A = listToInt(listA);

    list *list_on_position = new list(*processorStack->getListByPosition(A));
    processorStack->push(list_on_position);
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
    if (processorStack->getSize() < 2) return;
    list *listA = processorStack->pop();

    list *listB = processorStack->getListByPosition();

    listB->mergeLists(listA);
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
void handleDotSymbol(int &inputDataIndex) {
    processorStack->getListByPosition()->add(getInputData());
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
 * Compare 2 top numbers A B. Remove them from stack.
 * If A = B put 1 on top otherwise put 0.
 */
void handleEqualSymbol() {
    list *newList = new list();
    if (processorStack->getSize() < 2) {
        newList->add('0');
        processorStack->push(newList);
        return;
    }
    list *listA = processorStack->pop();
    list *listB = processorStack->pop();

    char *A = listA->getString();
    char *B = listB->getString();

    if (A == nullptr or B == nullptr) {
        if (A == nullptr and B == nullptr) {
            newList->add('1');
            processorStack->push(newList);
            return;
        }
        newList->add('0');
        processorStack->push(newList);
        return;
    }

    A = removeTrailingZeros(A);
    B = removeTrailingZeros(B);


    compareStringNumbers(A, B) == -1 ? newList->add('1') : newList->add('0');
    processorStack->push(newList);
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

/**
 * Handle the '!' symbol.
 * Pops first list from stack. If the list is empty or contains only '0', push '1' onto the stack.
 * Otherwise, push '0' onto the stack.
 */
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

/**
 * Handle the '~' symbol.
 * Converts number to a string and pushes it onto the stack.
 *
 * @param number The number to convert to a string.
 */
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

/**
 * Add 2 numbers on top of the stack. Remove them, add new number on top.
 */
void handlePlusSymbol() {
    list *listA = processorStack->pop();
    list *listB = processorStack->pop();

    char *numberA = listA->getString();
    char *numberB = listB->getString();
    removeTrailingZeros(numberA);
    removeTrailingZeros(numberB);

    int lengthA = getStringLength(numberA);
    int lengthB = getStringLength(numberB);

    // Check if the input numbers are negative
    bool isANegative = (numberA[lengthA - 1] == '-');
    bool isBNegative = (numberB[lengthB - 1] == '-');

    // Remove the negative sign if present
    if (isANegative) numberA[lengthA - 1] = '\0';
    if (isBNegative) numberB[lengthB - 1] = '\0';

    char *result;
    if (isANegative xor isBNegative) {
        // If one number is negative, subtract
        if (isANegative) {
            result = subtractLargeNumbers(numberB, numberA);
        } else {
            result = subtractLargeNumbers(numberA, numberB);
        }
        reverseString(result, 0, getStringLength(result));
    } else {
        // If both numbers are positive or both are negative, add
        result = addLargeNumbers(numberA, numberB);
        if (isANegative && isBNegative) {
            int resultLength = getStringLength(result);
            // If both numbers are negative, add and make the result negative
            result[resultLength] = '-';
            result[resultLength + 1] = '\0';
        }
    }


    removeTrailingZeros(result);
    reverseString(result, 0, getStringLength(result));
    handleNumberInsertion(result);
}

// ╔══════════════════════════════════╗
// ║                                  ║
// ║            Main Functions        ║
// ║                                  ║
// ╚══════════════════════════════════╝

void init() {
    current_step_pointer = 0;
    processorStack = new stack();
    getProgram();
    std::cin.ignore();
}

int main() {
    init();

    int inputDataIndex = 0;
    char str = '\0';

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
                handleDotSymbol(inputDataIndex);
                break;
            case '^':
                handleCaretSymbol();
                break;
            case '>':
                if (!processorStack->empty() && processorStack->getListByPosition()->getSize() > 0) {
                    str = processorStack->getListByPosition()->popListElement();
                    processorStack->pop();
                }
                printChar(str);
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
                if (processorStack->getSize() < 2) {
                    increaseInstructionPointer = true;
                    break;
                }
                increaseInstructionPointer = handleQuestionMarkSymbol(current_step_pointer);
                break;
            default:
                char value = program[current_step_pointer];
                if (processorStack->empty()) {
                    addEmptyList();
                }
                processorStack->getListByPosition()->add(value);
        }
        if (increaseInstructionPointer) current_step_pointer++;

    } while (program[current_step_pointer] != '\0');

    return 0;
}