#pragma once
#include <stdexcept>

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

#include "SeqContainer.cpp"