#include <iostream>
#include <memory>
#include <initializer_list>
#include <stdexcept>
#include <iterator>
#include <utility>


template <typename T>
class ParContainer {
public:
    virtual ~ParContainer() = default;

    virtual void push_back(const T& value) = 0;
    virtual void push_back(T&& value) = 0;
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
};


template <typename T>
class SeqContainer : public ParContainer<T> {
private:
    size_t m_size = 0;
    size_t m_capacity = 0;
    std::unique_ptr<T[]> m_data;

    // Резервирование памяти (коэффициент 2.0)
    void reallocate(size_t new_capacity) {
        auto new_data = std::make_unique<T[]>(new_capacity);
        for (size_t i = 0; i < m_size; ++i) {
            new_data[i] = std::move(m_data[i]);
        }
        m_data = std::move(new_data);
        m_capacity = new_capacity;
    }

public:
    //  Итератор (Random Access)
    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(T* ptr) : m_ptr(ptr) {}
        reference operator*() const { return *m_ptr; }

        // Инкремент/Декремент
        Iterator& operator++() { m_ptr++; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        Iterator& operator--() { m_ptr--; return *this; }
        Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }

        // Арифметика (требуется для Random Access и std::advance)
        Iterator& operator+=(difference_type n) { m_ptr += n; return *this; }
        Iterator& operator-=(difference_type n) { m_ptr -= n; return *this; }
        Iterator operator+(difference_type n) const { return Iterator(m_ptr + n); }
        Iterator operator-(difference_type n) const { return Iterator(m_ptr - n); }
        difference_type operator-(const Iterator& other) const { return m_ptr - other.m_ptr; }

        // Сравнения
        bool operator==(const Iterator& other) const { return m_ptr == other.m_ptr; }
        bool operator!=(const Iterator& other) const { return m_ptr != other.m_ptr; }
        bool operator<(const Iterator& other) const { return m_ptr < other.m_ptr; }
        bool operator>(const Iterator& other) const { return m_ptr > other.m_ptr; }
        bool operator<=(const Iterator& other) const { return m_ptr <= other.m_ptr; }
        bool operator>=(const Iterator& other) const { return m_ptr >= other.m_ptr; }

    private:
        T* m_ptr;
    };

    using iterator = Iterator;

    // Конструкторы/Операторы
    SeqContainer() = default;
    SeqContainer(const SeqContainer& other) {
        reallocate(other.m_capacity);
        for (size_t i = 0; i < other.m_size; ++i) m_data[i] = other.m_data[i];
        m_size = other.m_size;
    }
    SeqContainer(SeqContainer&& other) noexcept : m_size(other.m_size), m_capacity(other.m_capacity), m_data(std::move(other.m_data)) {
        other.m_size = 0; other.m_capacity = 0;
    }
    SeqContainer& operator=(const SeqContainer& other) {
        if (this == &other) return *this; auto tmp = other; *this = std::move(tmp); return *this;
    }
    SeqContainer& operator=(SeqContainer&& other) noexcept {
        if (this == &other) return *this;
        m_data = std::move(other.m_data); m_size = other.m_size; m_capacity = other.m_capacity;
        other.m_size = 0; other.m_capacity = 0; return *this;
    }

    // Конструктор принимающий initializer_list
    SeqContainer(std::initializer_list<T> init) {
        reallocate(init.size());
        for (const auto& item : init) push_back(item);
    }

    // push_back (L-value и R-value)
    void push_back(const T& value) override {
        if (m_size == m_capacity) reallocate(m_capacity == 0 ? 1 : m_capacity * 2);
        m_data[m_size++] = value;
    }
    void push_back(T&& value) override {
        if (m_size == m_capacity) reallocate(m_capacity == 0 ? 1 : m_capacity * 2);
        m_data[m_size++] = std::move(value);
    }

    // insert (L-value и R-value)
    iterator insert(iterator pos, const T& value) {
        size_t index = pos - begin();
        if (m_size == m_capacity) reallocate(m_capacity == 0 ? 1 : m_capacity * 2);
        for (size_t i = m_size; i > index; --i) m_data[i] = std::move(m_data[i - 1]);
        m_data[index] = value; m_size++; return iterator(&m_data[index]);
    }
    iterator insert(iterator pos, T&& value) {
        size_t index = pos - begin();
        if (m_size == m_capacity) reallocate(m_capacity == 0 ? 1 : m_capacity * 2);
        for (size_t i = m_size; i > index; --i) m_data[i] = std::move(m_data[i - 1]);
        m_data[index] = std::move(value); m_size++; return iterator(&m_data[index]);
    }

    // erase
    iterator erase(iterator pos) {
        size_t index = pos - begin();
        if (index >= m_size) return end();
        for (size_t i = index; i < m_size - 1; ++i) m_data[i] = std::move(m_data[i + 1]);
        m_size--; return iterator(&m_data[index]);
    }

    // operator[] (доступ по индексу)
    T& operator[](size_t index) {
        if (index >= m_size) throw std::out_of_range("Index out of range");
        return m_data[index];
    }

    // size, begin/end
    size_t size() const override { return m_size; }
    bool empty() const override { return m_size == 0; }
    iterator begin() { return iterator(m_data.get()); }
    iterator end() { return iterator(m_data.get() + m_size); }
};


template <typename T>
class DLLContainer : public ParContainer<T> {
private:
    struct Node {
        T data;
        std::unique_ptr<Node> next;
        Node* prev = nullptr;
        Node(const T& val) : data(val) {}
        Node(T&& val) : data(std::move(val)) {}
    };

    std::unique_ptr<Node> head;
    Node* tail = nullptr;
    size_t m_size = 0;

public:
    // Итератор (Bidirectional)
    class Iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(Node* node) : m_node(node) {}
        reference operator*() const { return m_node->data; }
        Iterator& operator++() { if (m_node) m_node = m_node->next.get(); return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        Iterator& operator--() { if (m_node) m_node = m_node->prev; return *this; }
        Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }
        bool operator==(const Iterator& other) const { return m_node == other.m_node; }
        bool operator!=(const Iterator& other) const { return m_node != other.m_node; }
        Node* getNode() const { return m_node; }
    private:
        Node* m_node;
    };

    using iterator = Iterator;

    // Конструкторы/Операторы/Initializer list
    DLLContainer() = default;
    DLLContainer(std::initializer_list<T> init) { for (const auto& item : init) push_back(item); }
    DLLContainer(const DLLContainer& other) { for (auto it = other.head.get(); it != nullptr; it = it->next.get()) push_back(it->data); }
    DLLContainer(DLLContainer&& other) noexcept
        : head(std::move(other.head)), tail(other.tail), m_size(other.m_size) {
        other.tail = nullptr; other.m_size = 0;
    }
    DLLContainer& operator=(DLLContainer&& other) noexcept {
        if (this == &other) return *this;
        head = std::move(other.head); tail = other.tail; m_size = other.m_size;
        other.tail = nullptr; other.m_size = 0; return *this;
    }

    // push_back (L-value и R-value)
    void push_back(const T& value) override {
        auto new_node = std::make_unique<Node>(value); Node* raw_node = new_node.get();
        if (!head) { head = std::move(new_node); }
        else { new_node->prev = tail; tail->next = std::move(new_node); }
        tail = raw_node; m_size++;
    }
    void push_back(T&& value) override {
        auto new_node = std::make_unique<Node>(std::move(value)); Node* raw_node = new_node.get();
        if (!head) { head = std::move(new_node); }
        else { new_node->prev = tail; tail->next = std::move(new_node); }
        tail = raw_node; m_size++;
    }

    // insert (L-value/R-value)
    iterator insert(iterator pos, const T& value) {
        Node* curr = pos.getNode();
        if (!curr) { push_back(value); return iterator(tail); }
        auto new_node = std::make_unique<Node>(value); Node* new_raw = new_node.get();
        if (curr->prev) {
            new_node->prev = curr->prev;
            new_node->next = std::move(curr->prev->next);
            curr->prev->next = std::move(new_node);
            curr->prev = new_raw;
        }
        else {
            new_node->next = std::move(head);
            if (new_node->next) new_node->next->prev = new_raw;
            head = std::move(new_node);
        }
        m_size++; return iterator(new_raw);
    }
    iterator insert(iterator pos, T&& value) {
        Node* curr = pos.getNode();
        if (!curr) { push_back(std::move(value)); return iterator(tail); }
        auto new_node = std::make_unique<Node>(std::move(value)); Node* new_raw = new_node.get();
        if (curr->prev) {
            new_node->prev = curr->prev;
            new_node->next = std::move(curr->prev->next);
            curr->prev->next = std::move(new_node);
            curr->prev = new_raw;
        }
        else {
            new_node->next = std::move(head);
            if (new_node->next) new_node->next->prev = new_raw;
            head = std::move(new_node);
        }
        m_size++; return iterator(new_raw);
    }

    // erase
    iterator erase(iterator pos) {
        Node* curr = pos.getNode(); if (!curr) return end();
        Node* nextIterNode = curr->next.get();
        if (curr->prev) {
            if (curr->next) { curr->next->prev = curr->prev; curr->prev->next = std::move(curr->next); }
            else { curr->prev->next = nullptr; tail = curr->prev; }
        }
        else {
            if (curr->next) { curr->next->prev = nullptr; head = std::move(curr->next); }
            else { head = nullptr; tail = nullptr; }
        }
        m_size--; return iterator(nextIterNode);
    }

    // operator[]
    T& operator[](size_t index) {
        if (index >= m_size) throw std::out_of_range("Index out of range");
        Node* curr = head.get();
        for (size_t i = 0; i < index; ++i) curr = curr->next.get();
        return curr->data;
    }

    // size, begin/end
    size_t size() const override { return m_size; }
    bool empty() const override { return m_size == 0; }
    iterator begin() { return iterator(head.get()); }
    iterator end() { return iterator(nullptr); }
};


template <typename T>
class SLLContainer : public ParContainer<T> {
private:
    struct Node {
        T data;
        std::unique_ptr<Node> next;
        Node(const T& val) : data(val) {}
        Node(T&& val) : data(std::move(val)) {}
    };

    std::unique_ptr<Node> head;
    Node* tail = nullptr;
    size_t m_size = 0;
public:
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag; // 9. Forward Iterator
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        Iterator(Node* node) : m_node(node) {}
        reference operator*() const { return m_node->data; }
        Iterator& operator++() { if (m_node) m_node = m_node->next.get(); return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const Iterator& other) const { return m_node == other.m_node; }
        bool operator!=(const Iterator& other) const { return m_node != other.m_node; }
        Node* getNode() const { return m_node; }
    private:
        Node* m_node;
    };
    using iterator = Iterator;
    SLLContainer() = default;
    SLLContainer(std::initializer_list<T> init) { for (const auto& item : init) push_back(item); }

    // operator[]
    T& operator[](size_t index) {
        if (index >= m_size) throw std::out_of_range("Index out of range");
        Node* curr = head.get();
        for (size_t i = 0; i < index; ++i) curr = curr->next.get();
        return curr->data;
    }

    // push_back (L-value/R-value)
    void push_back(const T& value) override {
        auto new_node = std::make_unique<Node>(value); Node* raw = new_node.get();
        if (!head) { head = std::move(new_node); }
        else { tail->next = std::move(new_node); }
        tail = raw; m_size++;
    }
    void push_back(T&& value) override {
        auto new_node = std::make_unique<Node>(std::move(value)); Node* raw = new_node.get();
        if (!head) { head = std::move(new_node); }
        else { tail->next = std::move(new_node); }
        tail = raw; m_size++;
    }

    // insert (L-value)
    iterator insert(iterator pos, const T& value) {
        if (pos == begin()) {
            auto new_node = std::make_unique<Node>(value); Node* raw = new_node.get();
            new_node->next = std::move(head); head = std::move(new_node);
            if (m_size == 0) tail = raw; m_size++; return iterator(raw);
        }
        Node* prev = head.get();
        while (prev && prev->next.get() != pos.getNode()) { prev = prev->next.get(); }
        if (prev) {
            auto new_node = std::make_unique<Node>(value); Node* raw = new_node.get();
            new_node->next = std::move(prev->next); prev->next = std::move(new_node);
            if (!raw->next) tail = raw; m_size++; return iterator(raw);
        }
        return end();
    }
    // insert (R-value)
    iterator insert(iterator pos, T&& value) {
        if (pos == begin()) {
            auto new_node = std::make_unique<Node>(std::move(value)); Node* raw = new_node.get();
            new_node->next = std::move(head); head = std::move(new_node);
            if (m_size == 0) tail = raw; m_size++; return iterator(raw);
        }
        Node* prev = head.get();
        while (prev && prev->next.get() != pos.getNode()) { prev = prev->next.get(); }
        if (prev) {
            auto new_node = std::make_unique<Node>(std::move(value)); Node* raw = new_node.get();
            new_node->next = std::move(prev->next); prev->next = std::move(new_node);
            if (!raw->next) tail = raw; m_size++; return iterator(raw);
        }
        return end();
    }

    // erase
    iterator erase(iterator pos) {
        if (pos == end()) return end();
        if (pos == begin()) {
            head = std::move(head->next); if (!head) tail = nullptr; m_size--; return begin();
        }
        Node* prev = head.get();
        while (prev && prev->next.get() != pos.getNode()) { prev = prev->next.get(); }
        if (prev) {
            Node* nextIterNode = pos.getNode()->next.get();
            prev->next = std::move(pos.getNode()->next);
            if (!prev->next) tail = prev; m_size--; return iterator(nextIterNode);
        }
        return end();
    }

    // size, begin/end
    size_t size() const override { return m_size; }
    bool empty() const override { return m_size == 0; }
    iterator begin() { return iterator(head.get()); }
    iterator end() { return iterator(nullptr); }
};


int main() {
    std::cout << "SeqContainer (sequential)\n";

    SeqContainer<int> vec;

    for (int i = 0; i < 10; ++i) vec.push_back(i);

    std::cout << "Initial content: ";
    bool first = true;
    for (auto val : vec) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << "\n";

    std::cout << "Container size: " << vec.size() << "\n";

    // 5. Удаление третьего (по счёту), пятого и седьмого элементов.
    auto it_v = vec.begin();
    std::advance(it_v, 2); // 3-й элемент (индекс 2, значение 2)
    it_v = vec.erase(it_v);
    std::advance(it_v, 1); // 5-й элемент (изначально, теперь индекс 3, значение 4)
    it_v = vec.erase(it_v);
    std::advance(it_v, 1); // 7-й элемент (изначально, теперь индекс 4, значение 6)
    vec.erase(it_v);

    std::cout << "After removal: ";
    first = true;
    for (auto val : vec) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << "\n";

    // 7. Добавление элемента 10 в начало контейнера.
    vec.insert(vec.begin(), 10);

    std::cout << "After adding 10 to the beginning: ";
    first = true;
    for (auto val : vec) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << "\n";

    // 9. Добавление элемента 20 в середину контейнера (позиция 4: между 3 и 5)
    auto mid_it_v = vec.begin();
    std::advance(mid_it_v, 4);
    vec.insert(mid_it_v, 20);

    std::cout << "After adding 20 to the middle: ";
    first = true;
    for (auto val : vec) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << "\n";

    // 11. Добавление элемента 30 в конец контейнера.
    vec.push_back(30);

    std::cout << "Final content: ";
    first = true;
    for (auto val : vec) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << "\n\n";

    // ===============================================================
    std::cout << "DLLContainer (doubly linked list)\n";

    DLLContainer<int> list;

    for (int i = 0; i < 10; ++i) list.push_back(i);

    std::cout << "Initial content: ";
    first = true;
    for (auto val : list) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << "\n";

    std::cout << "Container size: " << list.size() << "\n";

    // 5. Удаление третьего (по счёту), пятого и седьмого элементов.
    auto it_l = list.begin();
    std::advance(it_l, 2); // 3-й элемент
    it_l = list.erase(it_l);
    std::advance(it_l, 1); // 5-й элемент (изначально)
    it_l = list.erase(it_l);
    std::advance(it_l, 1); // 7-й элемент (изначально)
    list.erase(it_l);

    std::cout << "After removal: ";
    first = true;
    for (auto val : list) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << "\n";

    // 7. Добавление элемента 10 в начало контейнера.
    list.insert(list.begin(), 10);

    std::cout << "After adding 10 to the beginning: ";
    first = true;
    for (auto val : list) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << "\n";

    // 9. Добавление элемента 20 в середину контейнера (позиция 4)
    auto mid_it_l = list.begin();
    std::advance(mid_it_l, 4);
    list.insert(mid_it_l, 20);

    std::cout << "After adding 20 to the middle: ";
    first = true;
    for (auto val : list) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << "\n";

    // 11. Добавление элемента 30 в конец контейнера.
    list.push_back(30);

    std::cout << "Final content: ";
    first = true;
    for (auto val : list) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << "\n\n";

    std::cout << "SLLContainer (singly linked list)\n";

    SLLContainer<int> fwd_list;

    for (int i = 0; i < 10; ++i) fwd_list.push_back(i);

    std::cout << "Initial content: ";
    first = true;
    for (auto val : fwd_list) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << "\n";

    std::cout << "Container size: " << fwd_list.size() << "\n";

    // 5. Удаление третьего (по счёту), пятого и седьмого элементов.
    auto it_f = fwd_list.begin();
    std::advance(it_f, 2);
    it_f = fwd_list.erase(it_f);
    std::advance(it_f, 1);
    it_f = fwd_list.erase(it_f);
    std::advance(it_f, 1);
    fwd_list.erase(it_f);

    std::cout << "After removal: ";
    first = true;
    for (auto val : fwd_list) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << "\n";

    // 7. Добавление элемента 10 в начало контейнера.
    fwd_list.insert(fwd_list.begin(), 10);

    std::cout << "After adding 10 to the beginning: ";
    first = true;
    for (auto val : fwd_list) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << "\n";

    // 9. Добавление элемента 20 в середину контейнера (позиция 4)
    auto mid_it_f = fwd_list.begin();
    std::advance(mid_it_f, 4);
    fwd_list.insert(mid_it_f, 20);

    std::cout << "After adding 20 to the middle: ";
    first = true;
    for (auto val : fwd_list) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << "\n";

    // 11. Добавление элемента 30 в конец контейнера.
    fwd_list.push_back(30);

    std::cout << "Final content: ";
    first = true;
    for (auto val : fwd_list) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << "\n\n";

    return 0;
}