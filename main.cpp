#include "stack.h"

// ╔══════════════════════════════════╗
// ║                                  ║
// ║       Input / Output func.       ║
// ║                                  ║
// ╚══════════════════════════════════╝

/**
 * Reads the program from the user.
 */
void getProgram(char program[20000]) {
    std::cin.getline(program, 20000);
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

void removeTrailingZerosRecursively(list *lst) {
    if (lst->empty()) {
        return;
    }

    // Remove the last element if it is '0'
    if (lst->getSize() > 1 && lst->getListElement(lst->getSize() - 1) == '0') {
        lst->remove(lst->getSize() - 1);
        removeTrailingZerosRecursively(lst);
    }
}

void removeLeadingZerosFromList(list *lst) {
    bool isNegative = false;

    // Check if the number is negative (minus at the end or the beginning)
    if (lst->getListElement(lst->getSize() - 1) == '-') {
        isNegative = true;
        lst->remove(lst->getSize() - 1);
    } else if (lst->getListElement(0) == '-') {
        isNegative = true;
        lst->remove(0);
    }

    removeTrailingZerosRecursively(lst);

    // If the number was negative, add the minus sign at the end
    if (isNegative and (lst->getSize() > 1 or lst->getListElement(0) != '0')) {
        lst->add('-');
    }
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

// ==============================
//        Operations on lists
// ==============================

/**
 * Add empty list to processor stack
 */
void addEmptyList(stack *&processorStack) {
    processorStack->push(new list());
};

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


int compareListNumbersRecursive(list *A, list *B, int indexA, int indexB) {
        // If both lists are empty, they are equal
        if (indexA < 0 && indexB < 0) {
            return -1;
        }

        // If one list is empty, the other is greater
        if (indexA < 0) return 0; // A < B
        if (indexB < 0) return 1; // A > B

        char charA = A->getListElement(indexA);
        char charB = B->getListElement(indexB);

        // Compare digits
        if (charA != charB) {
            return (charA > charB) ? 1 : 0;
        }

        // Recursively compare the next digits
        return compareListNumbersRecursive(A, B, indexA - 1, indexB - 1);
    }


/**
 * @brief Compares two representations of numbers stored in lists.
 *
 * @param A The first list representing a number.
 * @param B The second list representing a number.
 * @return -1 when A == B, 0 when A < B, 1 when A > B.
 */
int compareListNumbers(list *A, list *B) {
    // Remove leading zeros from both lists
    removeLeadingZerosFromList(A);
    removeLeadingZerosFromList(B);

    // Check if both lists are empty or contain only '-'
    if ((A->empty() && B->empty()) ||
        (A->empty() && B->getListElement(0) == '-') ||
        (B->empty() && A->getListElement(0) == '-')) {
        return -1; // They are equal
    }

    int lengthA = A->getSize();
    int lengthB = B->getSize();

    bool isANegative = (A->getListElement(0) == '-');
    bool isBNegative = (B->getListElement(0) == '-');

    // Handle cases where one number is negative
    if (isANegative && !isBNegative) return 0; // A < B
    if (!isANegative && isBNegative) return 1; // A > B

    if (isANegative && isBNegative) {
        // Both numbers are negative, compare absolute values
        A->remove(0); // Temporarily remove '-' from A
        B->remove(0); // Temporarily remove '-' from B
        int result = compareListNumbers(B, A); // Reverse comparison for negative numbers
        A->add('-'); // Restore '-' to A
        B->add('-'); // Restore '-' to B
        return result;
    }

    // Compare lengths
    if (lengthA != lengthB) {
        return (lengthA > lengthB) ? 1 : 0;
    }

    // Recursively compare digits
    return compareListNumbersRecursive(A, B, lengthA - 1, lengthB - 1);
}

// ==============================
//     Operations on large numbers
// ==============================

void subtractListsRecursive(list_node *node1, list_node *node2, int borrow, list *result) {
    if (node1 == nullptr && node2 == nullptr && borrow == 0) {
        return;
    }

    int n1 = (node1 != nullptr) ? node1->data - '0' : 0;
    int n2 = (node2 != nullptr) ? node2->data - '0' : 0;
    int current = n1 - n2 - borrow;

    if (current < 0) {
        current += 10;
        borrow = 1;
    } else {
        borrow = 0;
    }

    result->add(current + '0');

    subtractListsRecursive(
            (node1 != nullptr) ? node1->next : nullptr,
            (node2 != nullptr) ? node2->next : nullptr,
            borrow,
            result
    );
}

list *subtractLists(list *list1, list *list2) {
    list *result = new list();


    // Check if the result should be negative
    bool isNegative = false;
    if (compareListNumbers(list1, list2) == 0) {
        // Swap lists if list1 < list2
        list *temp = list1;
        list1 = list2;
        list2 = temp;

        isNegative = true;
    }

    subtractListsRecursive(list1->head, list2->head, 0, result);

    result->reverse(); // Reverse the result to get the correct digit order
    removeLeadingZerosFromList(result);
    // Add the '-' sign to the result if it is negative
    if (isNegative) {
        result->add('-', result->getSize());
    }

    return result;
}

void addListsRecursive(list_node *node1, list_node *node2, int carry, list *result) {
    if (node1 == nullptr && node2 == nullptr && carry == 0) {
        return;
    }

    int n1 = (node1 != nullptr) ? node1->data - '0' : 0;
    int n2 = (node2 != nullptr) ? node2->data - '0' : 0;
    int current = n1 + n2 + carry;
    carry = current / 10;
    current = current % 10;

    result->add(current + '0');

    addListsRecursive(
            (node1 != nullptr) ? node1->next : nullptr,
            (node2 != nullptr) ? node2->next : nullptr,
            carry,
            result
    );
}

list *addLists(list *list1, list *list2) {
    list *result = new list();

    addListsRecursive(list1->head, list2->head, 0, result);

    result->reverse();

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
void handleApostrofSymbol(stack *&processorStack) {
    addEmptyList(processorStack);
}

/**
 * Remove first element on processor stack.
 */
void handleCommaSymbol(stack *&processorStack) {
    processorStack->pop();
};

/**
 * Put on stack copy of the list form top of stack.
 */
void handleColonSymbol(stack *&processorStack) {
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
void handleAtSymbol(stack *&processorStack) {
    list *listA = processorStack->pop();

    int A = listToInt(listA);

    list *list_on_position = new list(*processorStack->getListByPosition(A));
    processorStack->push(list_on_position);
}

/**
 * @brief Recursively converts an integer to a list of characters.
 *
 * This function takes an integer value and recursively adds its digits
 * to the list in the correct order.
 *
 * @param value The integer value to convert.
 * @param newList The list to store the digits.
 */
void convertAsciiValueToListRecursive(int value, list *newList) {
    if (value == 0) {
        return;
    }
    convertAsciiValueToListRecursive(value / 10, newList);
    newList->add((value % 10) + '0');
}

/**
 * @brief Remove number on top of the stack (A).
 * Put on top of the stack number of the value of the first from char A.
 *
 * This function pops the top list from the stack, retrieves the ASCII value
 * of the first character, and pushes a new list containing the ASCII value
 * as a sequence of digits onto the stack.
 *
 * @param processorStack The stack to operate on.
 */
void handleLeftBracket(stack *&processorStack) {
    list *listA = processorStack->pop();
    if (listA->getSize() == 0) {
        throw std::out_of_range("List is empty handleLeftBracket main.cpp");
    }

    // Get the ASCII value of the first character
    char firstChar = listA->getListElement(0);
    int firstCharValue = static_cast<int>(firstChar);

    // Create a new list and populate it recursively
    list *newList = new list();
    if (firstCharValue == 0) {
        newList->add('0');
    } else {
        convertAsciiValueToListRecursive(firstCharValue, newList);
    }

    // Push the new list onto the stack
    processorStack->push(newList);
}


/**
 * Get fist char from number on top of the stack.
 * Put it on top of the stack.
 */
void handleDolarSymbol(stack *&processorStack) {
    char firstChar = processorStack->getListByPosition()->popListElement();
    list *newList = new list();
    newList->add(firstChar);
    processorStack->push(newList);
}


/**
 * Remove number A from top on the stack.
 * Put A on the on top of number B (second number on stack)
 */
void handleHashSymbol(stack *&processorStack) {
    if (processorStack->getSize() < 2) return;
    list *listA = processorStack->pop();

    list *listB = processorStack->getListByPosition();

    listB->mergeLists(listA);
}

/**
 * Swap first and second element on top of processor stack.
 */
void handleSemicolon(stack *&processorStack) {
    processorStack->flipTopList();
}

/**
 * Print the stack.
 */
void handleAmpersandSymbol(stack *&processorStack) {
    std::cout << *processorStack;
}


/**
 * Add '-' to the end of the list on top of the stack.
 * If '-' already present remove it.
 */
void handleMinusSymbol(stack *&processorStack) {
    list *top = processorStack->getListByPosition();

    if (top == nullptr) {
        throw std::out_of_range("Lista jest pusta w handleMinusSymbol main.cpp");
    }

    if (top->getSize() > 0 && top->getListElement(top->getSize() - 1) == '-') {
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
void handleDotSymbol(stack *&processorStack, int &inputDataIndex) {
    processorStack->getListByPosition()->add(getInputData());
    inputDataIndex++;
}


/**
 *  Take absolute value of number on top of processor stack.
 */
void handleCaretSymbol(stack *&processorStack) {
    list *top = processorStack->getListByPosition();
    if (top->getSize() > 0 and top->getListElement(top->getSize() - 1) == '-') {
        top->remove(top->getSize() - 1);
    }
}

/**
 * Remove number on top of the stack (A).
 * Put on top of the stack char of number A.
 */
void handleRightBracket(stack *&processorStack) {
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
void handleEqualSymbol(stack *&processorStack) {
    if (processorStack->getSize() < 2) {
        list *newList = new list();
        newList->add('0');
        processorStack->push(newList);
        return;
    }

    list *listA = processorStack->pop();
    list *listB = processorStack->pop();

    // Remove leading zeros from both lists
    removeLeadingZerosFromList(listA);
    removeLeadingZerosFromList(listB);

    int comparisonResult = compareListNumbers(listA, listB);

    // Push the result onto the stack
    list *result = new list();
    result->add(comparisonResult == -1 ? '1' : '0');
    processorStack->push(result);
}


/**
 * Compare 2 top numbers A B. Remove them from stack.
 * If A < B put 1 on top otherwise put 0.
 */
void handleLessSymbol(stack *&processorStack) {
    list *A = processorStack->pop();
    list *B = processorStack->pop();

    removeLeadingZerosFromList(A);
    removeLeadingZerosFromList(B);

    list *lst = new list();

    int result = compareListNumbers(A, B);
    lst->add(result == 1 ? '1' : '0');

    processorStack->push(lst);
}

/**
 * Handle the '!' symbol.
 * Pops first list from stack. If the list is empty or contains only '0', push '1' onto the stack.
 * Otherwise, push '0' onto the stack.
 */
void handleWykrzyknikSymbol(stack *&processorStack) {
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
 * @brief Recursively converts an integer to a list of characters.
 *
 * This function takes an integer value and recursively adds its digits
 * to the list in the correct order.
 *
 * @param value The integer value to convert.
 * @param newList The list to store the digits.
 */
void convertNumberToListRecursive(int value, list *newList) {
    if (value == 0) {
        return;
    }
    convertNumberToListRecursive(value / 10, newList);
    newList->add((value % 10) + '0');
}

/**
 * Handle the '~' symbol.
 * Converts number to a list and pushes it onto the stack.
 *
 * @param number The number to convert to a list.
 */
void handleTyldaSymbol(stack *&processorStack, int number) {
    list *newList = new list();

    if (number == 0) {
        newList->add('0');
    } else {
        convertNumberToListRecursive(number, newList);
    }

    processorStack->push(newList);
}

/**
 * @brief Conditional jump operation.
 *
 * This function performs a conditional jump based on the top elements of the stack.
 * It pops a number T from the stack, then pops a list W from the stack.
 * If the list W is not empty and does not contain only the character '0',
 * it sets the instruction pointer to the value of T and does not increment the instruction pointer.
 */
bool handleQuestionMarkSymbol(stack *&processorStack, int &current_step_pointer) {
    list *lstT = processorStack->pop();
    list *lstW = processorStack->pop();

    if (isListIsEmptyOrZero(lstW)) return true;

    current_step_pointer = listToInt(lstT);

    return false;
};

/**
 * Dodaj 2 liczby na szczycie stosu. Usuń je, dodaj nową liczbę na szczyt.
 */
void handlePlusSymbol(stack *&processorStack) {
    list *listA = processorStack->pop();
    list *listB = processorStack->pop();

    // Remove leading zeros from both lists
    removeLeadingZerosFromList(listA);
    removeLeadingZerosFromList(listB);

    // Check if lists are empty or contain only '0'
    bool isANegative = (listA->getListElement(0) == '-');
    bool isBNegative = (listB->getListElement(0) == '-');

    // Remove '-' from the beginning of the list if present
    if (isANegative) listA->remove(0);
    if (isBNegative) listB->remove(0);

    list *result;
    if (isANegative xor isBNegative) {
        // If one number is negative, perform subtraction
        if (isANegative) {
            result = subtractLists(listB, listA);
        } else {
            result = subtractLists(listA, listB);
        }
    } else {
        // If both numbers are positive or both are negative, perform addition
        result = addLists(listA, listB);
        if (isANegative && isBNegative) {
            // Restore the negative sign if both numbers were negative
            result->add('-', result->getSize());
        }
    }

    // Push the result onto the stack
    processorStack->push(result);
}

// ╔══════════════════════════════════╗
// ║                                  ║
// ║            Main Functions        ║
// ║                                  ║
// ╚══════════════════════════════════╝

void init(stack *&processorStack, char program[20000], int &current_step_pointer) {
    current_step_pointer = 0;
    processorStack = new stack();
    getProgram(program);
}

int main() {
    /// The program memory stores the program. The program is a sequence of instructions, and each instruction is a single character.
    char program[20000];

/// The instruction pointer stores the number of the instruction that will be executed next and increments by one after its execution (for most instructions).
    int current_step_pointer;

    stack *processorStack = nullptr;
    init(processorStack, program, current_step_pointer);

    int inputDataIndex = 0;
    char str = '\0';

    do {
        bool increaseInstructionPointer = true;
        switch (program[current_step_pointer]) {
            case '\'':
                handleApostrofSymbol(processorStack);
                break;
            case ',':
                handleCommaSymbol(processorStack);
                break;
            case ':':
                handleColonSymbol(processorStack);
                break;
            case ';':
                handleSemicolon(processorStack);
                break;
            case '@':
                handleAtSymbol(processorStack);
                break;
            case '&':
                handleAmpersandSymbol(processorStack);
                break;
            case '-':
                handleMinusSymbol(processorStack);
                break;
            case '+':
                handlePlusSymbol(processorStack);
                break;
            case '.':
                handleDotSymbol(processorStack, inputDataIndex);
                break;
            case '^':
                handleCaretSymbol(processorStack);
                break;
            case '>':
                if (!processorStack->empty() && processorStack->getListByPosition()->getSize() > 0) {
                    str = processorStack->getListByPosition()->popListElement();
                    processorStack->pop();
                }
                printChar(str);
                break;
            case ']':
                handleRightBracket(processorStack);
                break;
            case '[':
                handleLeftBracket(processorStack);
                break;
            case '$':
                handleDolarSymbol(processorStack);
                break;
            case '#':
                handleHashSymbol(processorStack);
                break;
            case '<':
                handleLessSymbol(processorStack);
                break;
            case '=':
                handleEqualSymbol(processorStack);
                break;
            case '!':
                handleWykrzyknikSymbol(processorStack);
                break;
            case '~':
                handleTyldaSymbol(processorStack, current_step_pointer);
                break;
            case '?':
                if (processorStack->getSize() < 2) {
                    increaseInstructionPointer = true;
                    break;
                }
                increaseInstructionPointer = handleQuestionMarkSymbol(processorStack, current_step_pointer);
                break;
            default:
                char value = program[current_step_pointer];
                if (processorStack->empty()) {
                    addEmptyList(processorStack);
                }
                processorStack->getListByPosition()->add(value);
        }
        if (increaseInstructionPointer) current_step_pointer++;

    } while (program[current_step_pointer] != '\0');

    return 0;
}