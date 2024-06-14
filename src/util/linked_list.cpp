#include "linked_list.h"
#include "../error.h"
#include "stdio.h"

// explicit instantiations
template class LinkedList<int>;
// You will only be able to use LinkedList with the listed above

template <typename T>
LinkedList<T>::LinkedList() {
    head.prev = nullptr;
    tail.next = nullptr;
    head.next = &tail;
    tail.prev = &head;

    _size = 0;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    Container * curr = head.next;
    Container * temp;

    // when curr->next == nullptr, curr is tail.
    while(curr->next != nullptr)
    {
        temp = curr;
        curr = curr->next;
        delete temp;
    }
}

template <typename T>
void 
LinkedList<T>::push_back(T entry)
{
    Container * new_entry = new Container(entry);

    Container * left = tail.prev;
    Container * right = &tail;

    new_entry->prev = left;
    new_entry->next = right;

    left->next = new_entry;
    right->prev = new_entry;

    _size++;
}

template <typename T>
void 
LinkedList<T>::push_front(T entry)
{
    Container * new_entry = new Container(entry);

    Container * left = &head;
    Container * right = head.next;

    new_entry->prev = left;
    new_entry->next = right;

    left->next = new_entry;
    right->prev = new_entry;

    _size++;
}

template <typename T>
T 
LinkedList<T>::pop_front()
{
    Container * left = &head;
    Container * right = head.next;

    left->next = right->next;
    right->next->prev = left;

    T ret = right->data;

    delete right;

    _size--;

    return ret;
}

template <typename T>
T 
LinkedList<T>::pop_back()
{
    Container * left = tail.prev;
    Container * right = &tail;

    right->prev = left->prev;
    left->prev->next = right;

    T ret = left->data;

    delete left;

    _size--;

    return ret;
}

template <typename T>
void 
LinkedList<T>::move_to_index(Iterator& it, const int index)
{
    if (this->_size <= index)
    {
        critical_error("index OOR: Index=%d >= Size=%zu\n", index, _size);
    }

    Container * left = it.node->prev;
    Container * right = it.node->next;
    
    // "remove" iterators node from list
    right->prev = left;
    left->next  = right;

    // find where to place it
    Container * curr = &head;
    for (int i = 0; i < index; i++)
    {
        curr = curr->next;
    }
    // curr is now at left of where we should insert

    left = curr;
    right = curr->next;

    left->next = it.node;
    right->prev = it.node;

    it.node->prev = left;
    it.node->next = right;
}