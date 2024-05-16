// On my honor, I pledge that I have neither received nor provided improper assistance in the completion of this assignment. 
// Signed: 조다빈 Student Number: 22000711

#include <iostream>

// Example 1
/*
long long unsigned factorial(int n) {
    if (n == 1 || n == 0)
        return n;
    return n * factorial(n - 1);
}
*/

// Example 2
/*
long long unsigned factorial(int n) {
    return (n == 1 || n == 0) ? 1 : n * factorial(n - 1);    // using ternary operator
}
*/

// Example 3
/*
long long unsigned factorial(int n) {
    std::cout << "n = " << n << std::endl;
    if (n == 1 || n == 0)
        return n;
    auto result = n * factorial(n - 1);
    std::cout << "n = " << n << "\tn! = " << result << std::endl;
    return result;
}
*/

// Example 4
long long unsigned factorial(int n) {
    if (n == 1 || n == 0) 
        return n;
    auto result = n * factorial(n - 1);
    return result;
}


int main() {
    int n[] = {1, 2, 3, 4, 5, 8, 12, 20};
    for (auto x : n)
        std::cout << "factorial(" << x << ") = " << factorial(x) << std::endl;
    return 0;
}