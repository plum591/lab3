#include <stdexcept>
#include <utility>

template <typename T>
class SeqContainer
{
private:
    T *m_data;
    size_t m_size;
    size_t m_capacity;

    void reallocate(size_t new_capacity);

public:
    using iterator = T *;

    SeqContainer();
    ~SeqContainer();

    SeqContainer(SeqContainer &&other) noexcept;
    SeqContainer &operator=(SeqContainer &&other) noexcept;

    void push_back(const T &value);
    void insert(size_t index, const T &value);
    void erase(size_t index);

    size_t size() const;
    T &operator[](size_t index);

    iterator begin();
    iterator end();
};


template <typename T>
void SeqContainer<T>::reallocate(size_t new_capacity)
{
    if (new_capacity <= m_capacity)
        return;
    T *new_block = new T[new_capacity];
    for (size_t i = 0; i < m_size; ++i)
    {
        new_block[i] = std::move(m_data[i]);
    }
    delete[] m_data;
    m_data = new_block;
    m_capacity = new_capacity;
}

template <typename T>
SeqContainer<T>::SeqContainer() : m_data(nullptr), m_size(0), m_capacity(0) {}

template <typename T>
SeqContainer<T>::~SeqContainer() { delete[] m_data; }

template <typename T>
SeqContainer<T>::SeqContainer(SeqContainer &&other) noexcept
    : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity)
{
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

template <typename T>
SeqContainer<T> &SeqContainer<T>::operator=(SeqContainer &&other) noexcept
{
    if (this != &other)
    {
        delete[] m_data;
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }
    return *this;
}

template <typename T>
void SeqContainer<T>::push_back(const T &value)
{
    if (m_size == m_capacity)
    {
        size_t new_cap = m_capacity == 0 ? 1 : static_cast<size_t>(m_capacity * 1.6) + 1;
        reallocate(new_cap);
    }
    m_data[m_size] = value;
    m_size++;
}

template <typename T>
void SeqContainer<T>::insert(size_t index, const T &value)
{
    if (index > m_size)
        throw std::out_of_range("Index out of range");
    if (m_size == m_capacity)
    {
        size_t new_cap = m_capacity == 0 ? 1 : static_cast<size_t>(m_capacity * 1.6) + 1;
        reallocate(new_cap);
    }
    for (size_t i = m_size; i > index; --i)
    {
        m_data[i] = std::move(m_data[i - 1]);
    }
    m_data[index] = value;
    m_size++;
}

template <typename T>
void SeqContainer<T>::erase(size_t index)
{
    if (index >= m_size)
        throw std::out_of_range("Index out of range");
    for (size_t i = index; i < m_size - 1; ++i)
    {
        m_data[i] = std::move(m_data[i + 1]);
    }
    m_size--;
}

template <typename T>
size_t SeqContainer<T>::size() const { return m_size; }

template <typename T>
T &SeqContainer<T>::operator[](size_t index)
{
    if (index >= m_size)
        throw std::out_of_range("Index out of range");
    return m_data[index];
}

template <typename T>
typename SeqContainer<T>::iterator SeqContainer<T>::begin() { return m_data; }

template <typename T>
typename SeqContainer<T>::iterator SeqContainer<T>::end() { return m_data + m_size; }
