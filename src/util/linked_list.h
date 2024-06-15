#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

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
        Iterator& operator+=(int rhs)
        {
            for (int i = 0; i < rhs && this->node->next!=nullptr; i++)
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
        bool operator!=(const Iterator& rhs) const
        {
            return node->next != rhs.node->next && node->prev != rhs.node->prev;
        }

        inline 
        bool operator==(const Iterator& rhs) const
        {
            return this->operator!=(rhs);
        }

        inline 
        T operator*() const
        {
            return node->data;
        }
    };

    LinkedList();
    ~LinkedList();

    void push_back(T entry);

    void push_front(T entry);

    T pop_front();

    T pop_back();

    void move_to_index(Iterator it, const int index);
    void insert(T entry, const int index);

    Iterator erase(Iterator& it);

    inline 
    size_t size() const {return this->_size;}

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
};



#endif