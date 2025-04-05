//
// Created by Filip on 01/04/2025.
//

#include "list.h"

list::list() : head(nullptr), size(0) {
}

list::list(const list &other) : head(nullptr), size(other.size) {
    copyList(head, other.head);
}

void list::copyList(list_node *&thisNode, list_node *otherNode) {
    if (otherNode == nullptr) {
        thisNode = nullptr;
    } else {
        thisNode = new list_node{otherNode->data, nullptr};
        copyList(thisNode->next, otherNode->next);
    }
}

char *list::getString() const {
    if (head == nullptr) {
        return nullptr;
    }
    char *str = new char[size+2];
    list_node *current = head;
    for (int i = 0; i < size; ++i) {
        str[i] = current->data;
        current = current->next;
    }
    str[size] = '\0';
    return str;
}

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

void list::addOnBegining(const char value) {
    list_node *newNode = new list_node;
    newNode->data = value;
    newNode->next = head;
    head = newNode;
    ++size;
}

void list::add(const char value, int index) {
    if (index < 0 || index > size) {
        // throw std::out_of_range("Index out of range add");
    }
    if (index == 0) {
        addOnBegining(value);
    } else {
        list_node *newNode = new list_node{value, nullptr};
        list_node *current = head;
        for (int i = 1; i < index; ++i) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
        ++size;
    }
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

void list::mergeLists(list *otherList) {
    if (otherList == nullptr || otherList->head == nullptr) {
        return;
    }
    if (head == nullptr) {
        head = otherList->head;
    } else {
        mergeListsRecursive(head, otherList->head);
    }
    size += otherList->size;
    otherList->head = nullptr;
}

void list::mergeListsRecursive(list_node *current, list_node *otherHead) {
    if (current->next == nullptr) {
        current->next = otherHead;
    } else {
        mergeListsRecursive(current->next, otherHead);
    }
}


void list::remove(const int index) {
    if (index < 0 || index >= size) {
        // throw std::out_of_range("Index out of range remove");
    }
    removeNode(head, index);
}

int list::getSize() const {
    return size;
}

char list::getElementRecursive(list_node *node, int index) const {
    if (node == nullptr) {
        // throw std::out_of_range("Index out of range getElementRecursive");
    }
    if (index == 0) {
        return node->data;
    }
    return getElementRecursive(node->next, index - 1);
}

char list::getListElement(int index) {
    if (head == nullptr) {
        return '\0';
    }
    if (index < 0 || index >= size) {
        // throw std::out_of_range("Index out of range getListElement");
    }
    return getElementRecursive(head, index);
}

void list::printList(std::ostream &os, list_node *node) const {
    if (node == nullptr) {
        return;
    }
    os << node->data;
    printList(os, node->next);
}

bool list::empty() const {
    return getSize() == 0;
}


char list::popListElement() {
    if (head == nullptr) {
        return '\0';
    }
    char value = head->data;
    list_node *temp = head;
    head = head->next;
    delete temp;
    --size;
    return value;
}

std::ostream &operator<<(std::ostream &os, const list &lst) {
    lst.printList(os, lst.head);
    return os;
}
