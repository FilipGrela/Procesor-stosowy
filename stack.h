//
// Created by Filip on 01/04/2025.
//

#ifndef STACK_H
#define STACK_H

#include <iostream>

struct node {
    char data;
    node *next;
};

class stack {
    stack(const stack &);
    node *head;
    stack &operator=(const stack &);
    friend std::ostream& operator<<(std::ostream& os, const stack &stack);
public:
    stack();
    ~stack();
    int push(const char &);
    int getSize() const;
    char pop();
    bool empty() const;
private:
    int size;
};



#endif //STACK_H
