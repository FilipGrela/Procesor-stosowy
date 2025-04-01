//
// Created by Filip on 01/04/2025.
//

#include "stack.h"

#include <assert.h>

stack::stack() : size(0), head(nullptr){}

stack::~stack() {
    while (!empty()) {
        pop();
    }
}

stack::stack(const stack &other) : size(other.size), head(nullptr) {
    if (other.head) {

        head = new node{
            other.head->data,
            nullptr
        };

        node *current = head;

        node *otherCurrent = other.head->next;
        while (otherCurrent) {
            current->next = new node{otherCurrent->data, nullptr};
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
            head = new node{
                other.head->data,
                nullptr
            };
            node *current = head;
            node *otherCurrent = other.head->next;

            while (otherCurrent) {
                current->next = new node{otherCurrent->data, nullptr};
                current = current->next;
                otherCurrent = otherCurrent->next;
            }
        }
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const stack &other) {
    node *current = other.head;
    int index = 1;
    os << "{ \n";
    while (current) {
        os << index << ": \"" << current->data << "\"";
        current = current->next;
        if (current) {
            os << ",";
        }
        os << "\n";
        ++index;
    }
    os << "}" << std::endl;
    return os;
}

int stack::push(const char &value) {
    node *newNode = new node{value, head};
    head = newNode;
    ++size;
    return size;
}

char stack::pop() {
    assert(!empty() && "Stack is empty. You tried to pop empty stack!");
    node *temp = head;
    char value = head->data;
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