#include <string>
#include <iostream>

int add(int a, int b) {
    return a + b;
}

std::string greet(const std::string& name) {
    return "Hello, " + name + "!";
}

int main() {
    // Demonstrate add function
    int sum = add(5, 3);
    std::cout << "5 + 3 = " << sum << std::endl;

    // Demonstrate greet function
    std::string greeting = greet("Bazel User");
    std::cout << greeting << std::endl;

    return 0;
} 