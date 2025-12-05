#include <iostream>
#include <string>

#include "SeqContainer.h"
#include "DLLContainer.h"
#include "SLLContainer.h"

template <typename Container>
void print_container(const std::string &prefix, Container &c)
{
    std::cout << prefix;
    bool first = true;
    for (auto iter = c.begin(); iter != c.end(); ++iter)
    {
        if (!first)
            std::cout << ", ";
        std::cout << *iter;
        first = false;
    }
    std::cout << std::endl;
}

int main()
{
    std::cout << "SeqContainer\n";
    SeqContainer<int> seq_c;

    for (int i = 0; i < 10; ++i)
    {
        seq_c.push_back(i);
    }

    print_container("Content: ", seq_c);

    std::cout << "Size: " << seq_c.size() << std::endl;

    seq_c.erase(6);
    seq_c.erase(4);
    seq_c.erase(2);

    print_container("After erase: ", seq_c);

    seq_c.insert(0, 10);

    print_container("Add 10 to start: ", seq_c);

    seq_c.insert(4, 20);

    print_container("Add 20 to middle: ", seq_c);

    seq_c.push_back(30);

    print_container("Add 30 to end: ", seq_c);

    std::cout << "\nDLLContainer\n";
    DLLContainer<int> dll_c;

    for (int i = 0; i < 10; ++i)
    {
        dll_c.push_back(i);
    }

    print_container("Content: ", dll_c);

    std::cout << "Size: " << dll_c.size() << std::endl;

    dll_c.erase(6);
    dll_c.erase(4);
    dll_c.erase(2);

    print_container("After erase: ", dll_c);

    dll_c.insert(0, 10);

    print_container("Add 10 to start: ", dll_c);

    dll_c.insert(4, 20);

    print_container("Add 20 to middle: ", dll_c);

    dll_c.push_back(30);

    print_container("Add 30 to end: ", dll_c);

    std::cout << "\n SLLContainer\n";
    SLLContainer<int> sll_c;

    for (int i = 0; i < 10; ++i)
    {
        sll_c.push_back(i);
    }

    print_container("Content: ", sll_c);

    std::cout << "Size: " << sll_c.size() << std::endl;

    sll_c.erase(6);
    sll_c.erase(4);
    sll_c.erase(2);

    print_container("After erase: ", sll_c);

    sll_c.insert(0, 10);

    print_container("Add 10 to start: ", sll_c);

    sll_c.insert(4, 20);

    print_container("Add 20 to middle: ", sll_c);

    sll_c.push_back(30);

    print_container("Add 30 to end: ", sll_c);

    return 0;
}