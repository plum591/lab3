#include <stdexcept>
#include <utility>

template <typename T>
class SeqContainer
{
private:
    T *m_data;
    size_t m_size;
    size_t m_capacity;

    void reallocate(size_t new_capacity); // функция для расширения памяти

public:
    using iterator = T *;

    SeqContainer();  // конструктор. создание пустого контейнера
    ~SeqContainer(); // деструктор

    SeqContainer(SeqContainer &&other) noexcept;            // перемещающий конструктор
    SeqContainer &operator=(SeqContainer &&other) noexcept; // перемещающий оператор присваивания

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
    if (new_capacity <= m_capacity) // если новая память меньше норм
        return;
    iterator new_block = new T[new_capacity]; // иначе выделяем новую память под больший сущ массив
    for (size_t i = 0; i < m_size; ++i)
    {
        new_block[i] = std::move(m_data[i]); // в новый массив перемещаем данные старого
    }
    delete[] m_data;
    m_data = new_block;
    m_capacity = new_capacity;
}

template <typename T>
SeqContainer<T>::SeqContainer() : m_data(nullptr), m_size(0), m_capacity(0) {} // пустой контейнер

template <typename T>
SeqContainer<T>::~SeqContainer() { delete[] m_data; } // деструктор

template <typename T>
SeqContainer<T>::SeqContainer(SeqContainer &&other) noexcept
    : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) // два указателя Т
{
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0; // обнуление старого указателя
}

template <typename T>
SeqContainer<T> &SeqContainer<T>::operator=(SeqContainer &&other) noexcept
{
    if (this != &other) // если указатель не равен себе
    {
        delete[] m_data;
        m_data = other.m_data; // передача владения указателем
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        other.m_data = nullptr; // обнуление указателя источника
        other.m_size = 0;
        other.m_capacity = 0;
    }
    return *this; // разыменованный указатель(само значение объекта)
}

template <typename T>
void SeqContainer<T>::push_back(const T &value)
{
    if (m_size == m_capacity)
    {
        size_t new_cap;
        if (m_capacity == 0)
        {
            new_cap = 1;
        }
        else
        {
            new_cap = static_cast<size_t>(m_capacity * 1.6) + 1;
        }
        reallocate(new_cap); // новый увеличенный контейнер
    }
    m_data[m_size] = value; // вставка в конец
    m_size++;
}

template <typename T>
void SeqContainer<T>::insert(size_t index, const T &value)
{
    if (m_size == m_capacity)
    {
        size_t new_cap;
        if (m_capacity == 0)
        {
            new_cap = 1;
        }
        else
        {
            new_cap = static_cast<size_t>(m_capacity * 1.6) + 1;
        }
        reallocate(new_cap);
    }
    for (size_t i = m_size; i > index; --i) // пока размер больше индекса -1
    {
        m_data[i] = std::move(m_data[i - 1]); // смещаем числа на 1 с конца
    }
    m_data[index] = value; // вставляем нужную переменную
    m_size++;
}

template <typename T>
void SeqContainer<T>::erase(size_t index) // заменяет выбранный элемент на передистояший
{
    for (size_t i = index; i < m_size - 1; ++i)
    {
        m_data[i] = std::move(m_data[i + 1]);
    }
    m_size--; // удаляет последний элемент
}

template <typename T>
size_t SeqContainer<T>::size() const { return m_size; }

template <typename T>
T &SeqContainer<T>::operator[](size_t index)
{
    return m_data[index];
}

template <typename T>
typename SeqContainer<T>::iterator SeqContainer<T>::begin() { return m_data; }

template <typename T>
typename SeqContainer<T>::iterator SeqContainer<T>::end() { return m_data + m_size; }
