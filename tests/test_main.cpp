#include <iostream>
#include <cassert>
#include <string>

std::string get(std::string url) { return "response from " + url; }

int main() {
    std::string result = get("http://test");
    assert(result == "response from http://test");
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
