#ifndef DLLCONTAINER_CPP
#define DLLCONTAINER_CPP

#include "DLLContainer.h"

template <typename T>
DLLContainer<T>::Node::Node(const T &val) : data(val), next(nullptr), prev(nullptr) {}

template <typename T>
DLLContainer<T>::iterator::iterator(Node *p) : ptr(p) {}

template <typename T>
T &DLLContainer<T>::iterator::operator*() { return ptr->data; }

template <typename T>
typename DLLContainer<T>::iterator &DLLContainer<T>::iterator::operator++()
{
    if (ptr)
        ptr = ptr->next;
    return *this;
}

template <typename T>
bool DLLContainer<T>::iterator::operator!=(const iterator &other) const
{
    return ptr != other.ptr;
}

template <typename T>
T DLLContainer<T>::iterator::get() const { return ptr->data; }

template <typename T>
DLLContainer<T>::DLLContainer() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

template <typename T>
DLLContainer<T>::~DLLContainer()
{
    Node *current = m_head;
    while (current)
    {
        Node *next = current->next;
        delete current;
        current = next;
    }
}

template <typename T>
DLLContainer<T>::DLLContainer(DLLContainer &&other) noexcept
    : m_head(other.m_head), m_tail(other.m_tail), m_size(other.m_size)
{
    other.m_head = nullptr;
    other.m_tail = nullptr;
    other.m_size = 0;
}

template <typename T>
DLLContainer<T> &DLLContainer<T>::operator=(DLLContainer &&other) noexcept
{
    if (this != &other)
    {
        this->~DLLContainer();
        m_head = other.m_head;
        m_tail = other.m_tail;
        m_size = other.m_size;
        other.m_head = nullptr;
        other.m_tail = nullptr;
        other.m_size = 0;
    }
    return *this;
}

template <typename T>
void DLLContainer<T>::push_back(const T &value)
{
    Node *newNode = new Node(value);
    if (!m_tail)
    {
        m_head = m_tail = newNode;
    }
    else
    {
        m_tail->next = newNode;
        newNode->prev = m_tail;
        m_tail = newNode;
    }
    m_size++;
}

template <typename T>
size_t DLLContainer<T>::size() const { return m_size; }

template <typename T>
T &DLLContainer<T>::operator[](size_t index)
{
    if (index >= m_size)
        throw std::out_of_range("Index out of range");
    Node *current = m_head;
    for (size_t i = 0; i < index; ++i)
    {
        current = current->next;
    }
    return current->data;
}

template <typename T>
void DLLContainer<T>::insert(size_t index, const T &value)
{
    if (index > m_size)
        throw std::out_of_range("Index out of range");
    if (index == m_size)
    {
        push_back(value);
        return;
    }

    Node *newNode = new Node(value);
    if (index == 0)
    {
        newNode->next = m_head;
        if (m_head)
            m_head->prev = newNode;
        m_head = newNode;
    }
    else
    {
        Node *current = m_head;
        for (size_t i = 0; i < index; ++i)
            current = current->next;

        newNode->next = current;
        newNode->prev = current->prev;
        if (current->prev)
            current->prev->next = newNode;
        current->prev = newNode;
    }
    m_size++;
}

template <typename T>
void DLLContainer<T>::erase(size_t index)
{
    if (index >= m_size)
        throw std::out_of_range("Index out of range");

    Node *toDelete = m_head;
    for (size_t i = 0; i < index; ++i)
        toDelete = toDelete->next;

    if (toDelete->prev)
        toDelete->prev->next = toDelete->next;
    else
        m_head = toDelete->next;

    if (toDelete->next)
        toDelete->next->prev = toDelete->prev;
    else
        m_tail = toDelete->prev;

    delete toDelete;
    m_size--;
}

template <typename T>
typename DLLContainer<T>::iterator DLLContainer<T>::begin() { return iterator(m_head); }

template <typename T>
typename DLLContainer<T>::iterator DLLContainer<T>::end() { return iterator(nullptr); }

#endif