#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include "../error.h"

template <class T>
class LinkedList
{

private:
    class Container 
    {
        friend LinkedList;

        T          data;
        Container* next;
        Container* prev;

        public:
        Container() : data(), next(nullptr), prev(nullptr) {}
        Container(T _data) : data(_data), next(nullptr), prev(nullptr) {}
    };

    Container head;
    Container tail;
    size_t    _size;

public:
    class Iterator
    {
        friend LinkedList;
        private:
        Container* node;

        public:
        Iterator(Container * _node) noexcept :
            node (_node) { }
    
        inline 
        Iterator& operator=(const Iterator &rhs)
        {
            this->node = rhs.node;
            return *this;
        }

        // Prefix ++ overload
        inline 
        Iterator& operator++()
        {
            if (this->node->next == nullptr)
            {
                return *this;
            }
            this->node = this->node->next;
            return *this;
        }

        inline 
        Iterator& operator--()
        {
            if (this->node->prev == nullptr)
            {
                return *this;
            }
            this->node = this->node->prev;
            return *this;
        }

        inline 
        Iterator& operator+=(unsigned rhs)
        {
            for (unsigned i = 0; i < rhs && this->node->next!=nullptr; i++)
                this->node = this->node->next;

            return *this;
        }

        // Postfix ++ overload
        inline 
        Iterator& operator++(int)
        {
            ++*this;
            return *this;
        }

        inline 
        Iterator& operator--(int)
        {
            --*this;
            return *this;
        }

        inline 
        bool operator!=(const Iterator& rhs) const
        {
            return !this->operator==(rhs);
        }

        inline 
        bool operator==(const Iterator& rhs) const
        {
            return node->next == rhs.node->next && node->prev == rhs.node->prev;
        }

        inline 
        T& operator*() const
        {
            if (node->prev == nullptr || node->next == nullptr)
            {
                throw std::exception("Iterator pointed to head or tail, can't dereference");
            }
            return node->data;
        }

        inline
        T& operator->() const
        {
            return this->operator*();
        }
    };

    LinkedList();
    LinkedList(const LinkedList & rhs) {
        critical_error("NOT IMPLEMENTED\n");
    }
    ~LinkedList();

    void push_back(T entry);

    void push_front(T entry);

    T pop_front();

    T pop_back();

    void move_to_index(Iterator it, const int index);
    void insert(T entry, const int index);


    // O(N) Insert Sort. Does not guarantee list is always sorted unless you only add using this method
    // Starting from the head of the list, will insert entry once the condition returns true
    // will be s.t. entry will be on the left/prev to the element in the list that returned true
    //
    // Returns an iterator to the entry added
    Iterator sorted_insert(T entry, bool (*condition)(const T& rhs, const T& new_entry));

    Iterator erase(Iterator& it);

    inline 
    size_t size() const {return this->_size;}

    // inline T operator[](int n)
    // {
    //     Container * node = head.next;
    //     for (int i = 0; i < n && this->node->next!=nullptr; i++)
    //         node = node->next;

    //     return node->data;
    // }

    inline 
    Iterator get_head()
    {
        return Iterator(&head);
    }

    inline 
    Iterator begin()
    {
        return Iterator(head.next);
    }

    inline 
    Iterator end()
    {
        return Iterator(&tail);
    }

    void operator=(const LinkedList &rhs)
    {
        critical_error("NOT IMPLEMENTED\n");
    }
};



#endif