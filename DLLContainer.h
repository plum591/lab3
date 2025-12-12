#include <stdexcept>

template <typename T>
class DLLContainer
{
private:
    struct Node
    {
        T data;
        Node *next;         // указатель на след узел
        Node *prev;         // указатель на пред узел
        Node(const T &val); // конструктор узла
    };

    Node *m_head; // указатель на первый узел
    Node *m_tail; // указатель на последний узел
    size_t m_size;

public:
    class iterator
    {
    private:
        Node *ptr; // указатель на текущий узел

    public:
        iterator(Node *p); // конструктор итератора
        T &operator*();    // перегруженный оператор разыменования
        iterator &operator++();
        bool operator!=(const iterator &other) const;
        T get() const;
    };

    DLLContainer();
    ~DLLContainer();
    DLLContainer(DLLContainer &&other) noexcept;
    DLLContainer &operator=(DLLContainer &&other) noexcept;

    void push_back(const T &value);
    void insert(size_t index, const T &value);
    void erase(size_t index);
    size_t size() const;
    T &operator[](size_t index);
    iterator begin();
    iterator end();
};

template <typename T>
DLLContainer<T>::Node::Node(const T &val) : data(val), next(nullptr), prev(nullptr) {} // конструктор узла
template <typename T>
DLLContainer<T>::iterator::iterator(Node *p) : ptr(p) {}

template <typename T>
T &DLLContainer<T>::iterator::operator*() { return ptr->data; } // возвращает ссылку(можно менять само значение)

template <typename T>
typename DLLContainer<T>::iterator &DLLContainer<T>::iterator::operator++()
{
    if (ptr)
        ptr = ptr->next; // переход к следующему элементу
    return *this;
}

template <typename T>
bool DLLContainer<T>::iterator::operator!=(const iterator &other) const
{
    return ptr != other.ptr;
}

template <typename T>
T DLLContainer<T>::iterator::get() const { return ptr->data; } // возвращает значение (меняет копию значения)

template <typename T>
DLLContainer<T>::DLLContainer() : m_head(nullptr), m_tail(nullptr), m_size(0) {} // пустой список

template <typename T>
DLLContainer<T>::~DLLContainer() // деструктор. Поочерёдно удаляет узлы
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
        m_head = m_tail = newNode; // пустой список + новый узел
    }
    else
    {
        m_tail->next = newNode; // новый узел = след после последнего
        newNode->prev = m_tail;
        m_tail = newNode; // новый узел становится последним
    }
    m_size++;
}

template <typename T>
size_t DLLContainer<T>::size() const { return m_size; }

template <typename T>
T &DLLContainer<T>::operator[](size_t index)
{
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
    if (index == m_size)
    {
        push_back(value);
        return;
    }

    Node *newNode = new Node(value);
    if (index == 0) // вставка в начало
    {
        newNode->next = m_head;
        if (m_head)
            m_head->prev = newNode;
        m_head = newNode;
    }
    else
    {
        Node *current = m_head; // начинаем с 0 элемента
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
    Node *toDelete = m_head;
    for (size_t i = 0; i < index; ++i)
        toDelete = toDelete->next; // находим узел для удаления

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