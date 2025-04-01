//
// Created by Filip on 01/04/2025.
//

#include "list.h"
#include <cstring>

list::list() : size(0), head(nullptr) {}

list::~list() {
    while (head != nullptr) {
        list_node *temp = head;
        head = head->next;
        delete temp;
    }
}

void list::add(const char value) {
    list_node *newNode = new list_node;
    newNode->data = value;
    newNode->next = head;
    head = newNode;
    ++size;
}

void list::remove(const char *value) {
    list_node *current = head;
    list_node *previous = nullptr;

    while (current != nullptr && current->data != 0) {
        previous = current;
        current = current->next;
    }

    if (current != nullptr) {
        if (previous == nullptr) {
            head = current->next;
        } else {
            previous->next = current->next;
        }
        delete current;
        --size;
    }
}

int list::getSize() const {
    return size;
}

char list::getElement(int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    list_node *current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

void list::printList(std::ostream& os, list_node *node) const {
    if (node == nullptr) {
        return;
    }
    os << node->data;
    printList(os, node->next);
}

std::ostream& operator<<(std::ostream& os, const list &lst) {
    lst.printList(os, lst.head);
    return os;
}
