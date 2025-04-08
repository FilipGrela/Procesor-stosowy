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

void list::reverse() {
    reverseRecursive(nullptr, head);
}

void list::reverseRecursive(list_node *prev, list_node *current) {
    if (current == nullptr) {
        head = prev;
        return;
    }
    list_node *next = current->next;
    current->next = prev;
    reverseRecursive(current, next);
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

void list::addOnBeginning(const char value) {
    list_node *newNode = new list_node;
    newNode->data = value;
    newNode->next = head;
    head = newNode;
    ++size;
}

void list::add(const char value, int index) {
    if (index == 0) {
        addOnBeginning(value);
    } else {
        addRecursive(head, value, index);
    }
}

void list::addRecursive(list_node *&node, const char value, int index) {
    if (index == 1) {
        list_node *newNode = new list_node{value, node->next};
        node->next = newNode;
        ++size;
    } else {
        addRecursive(node->next, value, index - 1);
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

/**
 * @brief Merges another list into this list.
 *
 * This function merges the nodes of another list into this list.
 * If the other list is empty or null, the function does nothing.
 * If this list is empty, it simply takes the head of the other list.
 * Otherwise, it recursively merges the nodes of the other list into this list.
 *
 * @param otherList Pointer to the list to be merged.
 */
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
    removeNode(head, index);
}

int list::getSize() const {
    return size;
}

char list::getElementRecursive(list_node *node, int index) const {
    if (index == 0) {
        return node->data;
    }
    return getElementRecursive(node->next, index - 1);
}

char list::getListElement(int index) {
    if (head == nullptr) {
        return '\0';
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