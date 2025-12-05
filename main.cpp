#include <iostream>
#include <string>

#include "SeqContainer.h"
#include "DLLContainer.h"
#include "SLLContainer.h"

// Вспомогательная функция для вывода содержимого контейнера
template <typename Container>
void print_container(const std::string& prefix, Container& c) {
    std::cout << prefix;
    bool first = true;
    for (auto iter = c.begin(); iter != c.end(); ++iter) {
        if (!first) std::cout << ", ";
        std::cout << *iter;
        first = false;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Testing SeqContainer\n";
    SeqContainer<int> seq_c;

    // 2. Добавление десяти элементов (0, 1 ... 9)
    for (int i = 0; i < 10; ++i) {
        seq_c.push_back(i);
    }

    print_container("Content: ", seq_c);

    std::cout << "Size: " << seq_c.size() << std::endl;

    // 5. Удаление 3-го (2), 5-го (4) и 7-го (6) элементов. Удаляем 6, 4, 2
    seq_c.erase(6);
    seq_c.erase(4);
    seq_c.erase(2);

    print_container("After erase: ", seq_c);

    seq_c.insert(0, 10);

    print_container("Add 10 to start: ", seq_c);

    // 9. Добавление элемента 20 в середину. Текущий размер 8. Середина (индекс 4).
    seq_c.insert(4, 20);

    print_container("Add 20 to middle: ", seq_c);

    // 11. Добавление элемента 30 в конец.
    seq_c.push_back(30);

    print_container("Add 30 to end: ", seq_c);



    std::cout << "\nTesting DLLContainer\n";
    DLLContainer<int> dll_c;

    // 2. Добавление десяти элементов (0, 1 ... 9)
    for (int i = 0; i < 10; ++i) {
        dll_c.push_back(i);
    }

    print_container("Content: ", dll_c);

    std::cout << "Size: " << dll_c.size() << std::endl;

    // 5. Удаление 3-го (2), 5-го (4) и 7-го (6) элементов. Удаляем 6, 4, 2
    dll_c.erase(6);
    dll_c.erase(4);
    dll_c.erase(2);

    print_container("After erase: ", dll_c);

    // 7. Добавление элемента 10 в начало (индекс 0)
    dll_c.insert(0, 10);

    print_container("Add 10 to start: ", dll_c);

    // 9. Добавление элемента 20 в середину. Текущий размер 8. Середина (индекс 4).
    dll_c.insert(4, 20);

    print_container("Add 20 to middle: ", dll_c);

    // 11. Добавление элемента 30 в конец.
    dll_c.push_back(30);

    print_container("Add 30 to end: ", dll_c);



    std::cout << "\nTesting SLLContainer\n";
    SLLContainer<int> sll_c;

    // 2. Добавление десяти элементов (0, 1 ... 9)
    for (int i = 0; i < 10; ++i) {
        sll_c.push_back(i);
    }

    print_container("Content: ", sll_c);

    std::cout << "Size: " << sll_c.size() << std::endl;

    // 5. Удаление 3-го (2), 5-го (4) и 7-го (6) элементов. Удаляем 6, 4, 2
    sll_c.erase(6);
    sll_c.erase(4);
    sll_c.erase(2);

    print_container("After erase: ", sll_c);

    // 7. Добавление элемента 10 в начало (индекс 0)
    sll_c.insert(0, 10);

    print_container("Add 10 to start: ", sll_c);

    // 9. Добавление элемента 20 в середину. Текущий размер 8. Середина (индекс 4).
    sll_c.insert(4, 20);

    print_container("Add 20 to middle: ", sll_c);

    // 11. Добавление элемента 30 в конец.
    sll_c.push_back(30);

    print_container("Add 30 to end: ", sll_c);

    return 0;
}