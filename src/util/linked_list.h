#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

template <typename T>
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
    
        Iterator& operator=(const Iterator &rhs)
        {
            this->node = rhs.node;
            return *this;
        }

        // Prefix ++ overload
        Iterator& operator++()
        {
            this->node = this->node->next;
            return *this;
        }

        // Postfix ++ overload
        Iterator& operator++(int)
        {
            ++*this;
            return *this;
        }

        bool operator!=(const Iterator& rhs) const
        {
            return node->next != rhs.node->next && node->prev != rhs.node->prev;
        }

        T operator*() const
        {
            return node->data;
        }
    };

    LinkedList() {
        head.prev = nullptr;
        tail.next = nullptr;
        head.next = &tail;
        tail.prev = &head;

        _size = 0;
    }
    ~LinkedList() {
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

    void push_back(T entry)
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

    void push_front(T entry)
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

    T pop_front()
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

    T pop_back()
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

    void move_to_index(Iterator& it, const int index)
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

    inline size_t size() const {return this->_size;}

    Iterator begin()
    {
        return Iterator(head.next);
    }

    Iterator end()
    {
        return Iterator(&tail);
    }
};

#endif