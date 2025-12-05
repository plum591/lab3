#pragma once
#include <stdexcept>

template <typename T>
class SeqContainer {
private:
    T* m_data;
    size_t m_size;
    size_t m_capacity;

    void reallocate(size_t new_capacity);

public:
    // Итератор
    using iterator = T*;

    // Конструкторы и Деструктор
    SeqContainer();
    ~SeqContainer();

    // Move Semantics (Задание IV)
    SeqContainer(SeqContainer&& other) noexcept;
    SeqContainer& operator=(SeqContainer&& other) noexcept;

    // Методы интерфейса (Задание I)
    void push_back(const T& value);
    void insert(size_t index, const T& value);
    void erase(size_t index);

    // Доступ и размер
    size_t size() const;
    T& operator[](size_t index);

    // Методы итерации (Задание V)
    iterator begin();
    iterator end();
};

// Включаем файл реализации здесь для корректной компиляции шаблонов
#include "SeqContainer.cpp"