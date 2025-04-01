//
// Created by Filip on 01/04/2025.
//

#include "list.h"
#include <cstring>

list::list() : size(0), head(nullptr) {}

void list::deleteList(list_node *node) {
    if (node == nullptr) {
        return;
    }
    deleteList(node->next);
    delete node;
}

list::~list() {
    deleteList(head);
}

void list::add(const char value) {
    list_node *newNode = new list_node;
    newNode->data = value;
    newNode->next = head;
    head = newNode;
    ++size;
}

void list::removeNode(list_node *&node, int index) {
    if (node == nullptr) {
        return;
    }
    if (index == 0) {
        list_node *temp = node;
        node = node->next;
        delete temp;
        --size;
    } else {
        removeNode(node->next, index - 1);
    }
}

void list::remove(const int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    removeNode(head, index);
}

int list::getSize() const {
    return size;
}

char list::getElementRecursive(list_node *node, int index) const {
    if (node == nullptr) {
        throw std::out_of_range("Index out of range");
    }
    if (index == 0) {
        return node->data;
    }
    return getElementRecursive(node->next, index - 1);
}

char list::getElement(int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return getElementRecursive(head, index);
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
