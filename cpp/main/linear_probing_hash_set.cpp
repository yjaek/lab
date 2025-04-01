#include <iostream>
#include <string>

#include "lib/LinearProbingHashSet.h"

int main()
{
    LinearProbingHashSet<std::string> hs{10};
    std::cout << hs.contains() << std::endl;
    ;
}