//
// Created by Filip on 01/04/2025.
//

#ifndef LIST_H
#define LIST_H
#include <ostream>

struct list_node {
    char data;
    list_node *next;
};

class list {
    list &operator=(const list &);
    friend std::ostream& operator<<(std::ostream& os, const list &list);
public:
    list_node *head;
    list();
    list(const list &other);
    ~list();
    void add(char value, int index = 0);
    bool empty();
    void remove(int index);
    int getSize() const;
    char getListElement(int index);
    char popListElement();

    /***
     * Adds the elements of the other list to the end of this list.
     */
    void mergeLists(list *otherList);
private:
    void addOnBegining(char value);
    void copyList(list_node *&thisNode, list_node *otherNode);
    void removeNode(list_node *&node, int index);
    char getElementRecursive(list_node *node, int index) const;
    void mergeListsRecursive(list_node *current, list_node *otherHead);
    void deleteList(list_node *node);
    void printList(std::ostream& os, list_node *node) const;
    int size;
};

#endif //LIST_H
