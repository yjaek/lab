#include <iostream>
#include <ostream>

#include "lib/LinearProbingHashSet.h"

int main()
{
    LinearProbingHashSet<int> hs{10};
    std::cout << "Insert 1: " << hs.insert(1) << std::endl;
    std::cout << "Insert 2: " << hs.insert(2) << std::endl;
    std::cout << "Insert 3: " << hs.insert(3) << std::endl;
    std::cout << "Contain 1: " << hs.contains(1) << std::endl;
    std::cout << "Contain 2: " << hs.contains(2) << std::endl;
    std::cout << "Contain 3: " << hs.contains(3) << std::endl;
    std::cout << "Contain 4: " << hs.contains(4) << std::endl;
}