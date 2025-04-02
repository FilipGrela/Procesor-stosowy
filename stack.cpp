//
// Created by Filip on 01/04/2025.
//

#include "stack.h"

#include <assert.h>

stack::stack() : size(0), head(nullptr){}

void stack::deleteStack(stack_node *node) {
    if (node == nullptr) {
        return;
    }
    deleteStack(node->next);
    delete node->data;
    delete node;
}

stack::~stack() {
    deleteStack(head);
}

void stack::copyStack(stack_node *&dest, stack_node *src) {
    if (src == nullptr) {
        dest = nullptr;
        return;
    }
    dest = new stack_node{src->data, nullptr};
    copyStack(dest->next, src->next);
}

stack::stack(const stack &other) : size(other.size), head(nullptr) {
    copyStack(head, other.head);
}

stack &stack::operator=(const stack &other) {
    if (this != &other) {
        deleteStack(head);
        size = other.size;
        copyStack(head, other.head);
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

void stack::flipTopList() {
    if (head == nullptr) {
        throw std::out_of_range("Stack is empty");
    }
    list *temp = head->data;
    head->data = head->next->data;
    head->next->data = temp;
}

list* &stack::getListByPosition(int index, stack_node *node) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range in getListByPosition.");
    }
    if (node == nullptr) {
        node = head;
    }
    if (index == 0) {
        return node->data;
    }
    return getListByPosition(index - 1, node->next);
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