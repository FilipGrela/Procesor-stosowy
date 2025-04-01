#include "stack.h"


char program[20000]; ///< Pamięć programu przechowuje program. Program jest sekwencją instrukcji, a każda instrukcja jest jednym znakiem.
char inputData[20000]; ///< Pamięć danych przechowuje dane wejściowe. Dane wejściowe są sekwencją znaków, a każdy znak jest jednym bajtem.
int current_step; ///< Wskaźnik instrukcji przechowuje numer instrukcji, która będzie wykonana następna i zwiększa się o jeden po jej wykonaniu (w przypadku większości instrukcji).
stack *processorStack; ///< Stos procesora.

void getProgram() {
    std::cin >> program;
}

void getInputData() {
    std::cin >> inputData;
}

void init() {
    current_step = 0;
    processorStack = new stack();
    getProgram();
    getInputData();
}

int main() {
    init();

    return 0;
}
