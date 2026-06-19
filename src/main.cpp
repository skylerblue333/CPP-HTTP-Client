#include <iostream>
#include <string>
std::string get(std::string url) { return "response from " + url; }
int main() {
    std::cout << "Starting CPP-HTTP-Client..." << std::endl;
    return 0;
}
