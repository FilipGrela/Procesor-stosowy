//
// Created by Filip on 01/04/2025.
//

#ifndef STACK_H
#define STACK_H

#include <iostream>
#include "list.h"

struct stack_node {
    list *data;
    stack_node *next;
};

class stack {
    stack(const stack &);
    stack_node *head;
    stack &operator=(const stack &);
    friend std::ostream& operator<<(std::ostream& os, const stack &stack);
public:
    stack();
    ~stack();
    int getSize() const;
    bool empty() const;
    void flipTopList();
    int push(list*);
    list* pop();
    list* &getListByPosition(int index = 0, stack_node *node = nullptr);
private:
    int size;
    void deleteStack(stack_node *node);
    void copyStack(stack_node *&dest, stack_node *src);
    void printStack(std::ostream& os, stack_node *node, int index) const;
};

#endif //STACK_H