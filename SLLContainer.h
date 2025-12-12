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

// --- Реализация шаблона (перенесена из SLLContainer.cpp) ---

template <typename T>
SLLContainer<T>::Node::Node(const T &val) : data(val), next(nullptr) {}

template <typename T>
SLLContainer<T>::iterator::iterator(Node *p) : ptr(p) {}

template <typename T>
T &SLLContainer<T>::iterator::operator*() { return ptr->data; }

template <typename T>
typename SLLContainer<T>::iterator &SLLContainer<T>::iterator::operator++()
{
    if (ptr)
        ptr = ptr->next;
    return *this;
}

template <typename T>
bool SLLContainer<T>::iterator::operator!=(const iterator &other) const
{
    return ptr != other.ptr;
}

template <typename T>
T SLLContainer<T>::iterator::get() const { return ptr->data; }

template <typename T>
SLLContainer<T>::SLLContainer() : m_head(nullptr), m_size(0) {}

template <typename T>
SLLContainer<T>::~SLLContainer()
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
SLLContainer<T>::SLLContainer(SLLContainer &&other) noexcept
    : m_head(other.m_head), m_size(other.m_size)
{
    other.m_head = nullptr;
    other.m_size = 0;
}

template <typename T>
SLLContainer<T> &SLLContainer<T>::operator=(SLLContainer &&other) noexcept
{
    if (this != &other)
    {
        this->~SLLContainer();
        m_head = other.m_head;
        m_size = other.m_size;
        other.m_head = nullptr;
        other.m_size = 0;
    }
    return *this;
}

template <typename T>
void SLLContainer<T>::push_back(const T &value)
{
    Node *newNode = new Node(value);
    if (!m_head)
    {
        m_head = newNode;
    }
    else
    {
        Node *current = m_head;
        while (current->next)
            current = current->next;
        current->next = newNode;
    }
    m_size++;
}

template <typename T>
size_t SLLContainer<T>::size() const { return m_size; }

template <typename T>
T &SLLContainer<T>::operator[](size_t index)
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
void SLLContainer<T>::insert(size_t index, const T &value)
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
        m_head = newNode;
    }
    else
    {
        Node *current = m_head;
        for (size_t i = 0; i < index - 1; ++i)
            current = current->next;

        newNode->next = current->next;
        current->next = newNode;
    }
    m_size++;
}

template <typename T>
void SLLContainer<T>::erase(size_t index)
{
    if (index >= m_size)
        throw std::out_of_range("Index out of range");

    if (index == 0)
    {
        Node *toDelete = m_head;
        m_head = m_head->next;
        delete toDelete;
    }
    else
    {
        Node *prev = m_head;
        for (size_t i = 0; i < index - 1; ++i)
            prev = prev->next;

        Node *toDelete = prev->next;
        prev->next = toDelete->next;
        delete toDelete;
    }
    m_size--;
}

template <typename T>
typename SLLContainer<T>::iterator SLLContainer<T>::begin() { return iterator(m_head); }

template <typename T>
typename SLLContainer<T>::iterator SLLContainer<T>::end() { return iterator(nullptr); }
