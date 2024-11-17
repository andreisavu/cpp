#include "basics.h"
#include <iostream>

int main()
{
    // Demonstrate add function
    int sum = add(5, 3);
    std::cout << "5 + 3 = " << sum << std::endl;

    // Demonstrate greet function
    std::string greeting = greet("Bazel User");
    std::cout << greeting << std::endl;

    return 0;
}