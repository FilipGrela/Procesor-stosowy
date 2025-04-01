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
    list(const list &);
    list_node *head;
    list &operator=(const list &);
    friend std::ostream& operator<<(std::ostream& os, const list &list);
public:
    list();
    ~list();
    void add(char);
    void remove(const char *);
    int getSize() const;
    char getElement(int index);
private:
    void printList(std::ostream& os, list_node *node) const;
    int size;
};

#endif //LIST_H
