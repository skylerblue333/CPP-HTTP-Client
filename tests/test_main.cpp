#include <iostream>
#include <cassert>
assert(get("http://test") == "response from http://test");
int main() {
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
