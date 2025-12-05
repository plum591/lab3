#pragma once
#include <stdexcept>    

template <typename T>
class DLLContainer {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node(const T& val);
    };

    Node* m_head;
    Node* m_tail;
    size_t m_size;

public:
    // Структура Итератор (Задание V)
    class iterator {
    private:
        Node* ptr;

    public:
        iterator(Node* p);
        T& operator*();
        iterator& operator++();
        bool operator!=(const iterator& other) const;
        T get() const; // Метод get()
    };

    // Конструкторы и Деструктор
    DLLContainer();
    ~DLLContainer();
    DLLContainer(DLLContainer&& other) noexcept;
    DLLContainer& operator=(DLLContainer&& other) noexcept;

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

// Включаем файл реализации здесь
#include "DLLContainer.cpp"