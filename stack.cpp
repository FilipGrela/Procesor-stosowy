//
// Created by Filip on 01/04/2025.
//

#include "stack.h"

#include <assert.h>
#include <string.h>

stack::stack() : size(0), head(nullptr){}

stack::~stack() {
    while (!empty()) {
        pop();
    }
}

stack::stack(const stack &other) : size(other.size), head(nullptr) {
    if (other.head) {

        head = new stack_node{
            other.head->data,
            nullptr
        };

        stack_node *current = head;

        stack_node *otherCurrent = other.head->next;
        while (otherCurrent) {
            current->next = new stack_node{otherCurrent->data, nullptr};
            current = current->next;
            otherCurrent = otherCurrent->next;
        }
    }
}

stack &stack::operator=(const stack &other) {
    if (this != &other) {
        while (!empty()) {
            pop();
        }
        size = other.size;
        if (other.head) {
            head = new stack_node{
                other.head->data,
                nullptr
            };
            stack_node *current = head;
            stack_node *otherCurrent = other.head->next;

            while (otherCurrent) {
                current->next = new stack_node{otherCurrent->data, nullptr};
                current = current->next;
                otherCurrent = otherCurrent->next;
            }
        }
    }
    return *this;
}

void stack::printStack(std::ostream& os, stack_node *node, int index) const {
    if (node == nullptr) {
        return;
    }
    printStack(os, node->next, index + 1);
    os << index << ": " << *(node->data) << "\n";
}

std::ostream& operator<<(std::ostream& os, const stack &other) {
    other.printStack(os, other.head, 0);
    return os;
}

int stack::push(list *value) {
    stack_node *newNode = new stack_node{
        value,
        head
    };
    head = newNode;
    return ++size;
}

list *stack::getTopElement() {
    assert(!empty() && "Stack is empty. You tried to get top element of empty stack!");
    return head->data;
}

list* stack::pop() {
    assert(!empty() && "Stack is empty. You tried to pop empty stack!");
    stack_node *temp = head;
    list *value = head->data;
    head = head->next;
    delete temp;
    --size;
    return value;
}

int stack::getSize() const {
    return this->size;
}


bool stack::empty() const {
    return head == nullptr;
}