#pragma once
#include <stdexcept>

template <typename T>
class SLLContainer
{
private:
    struct Node
    {
        T data;
        Node *next;
        Node(const T &val);
    };

    Node *m_head;
    size_t m_size;

public:
    class iterator
    {
    private:
        Node *ptr;

    public:
        iterator(Node *p);
        T &operator*();
        iterator &operator++();
        bool operator!=(const iterator &other) const;
        T get() const;
    };

    SLLContainer();
    ~SLLContainer();
    SLLContainer(SLLContainer &&other) noexcept;
    SLLContainer &operator=(SLLContainer &&other) noexcept;

    void push_back(const T &value);
    void insert(size_t index, const T &value);
    void erase(size_t index);

    size_t size() const;
    T &operator[](size_t index);

    iterator begin();
    iterator end();
};

#include "SLLContainer.cpp"