#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

class HttpClient {
public:
    std::string get(const std::string& host, const std::string& path = "/") {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) return "Error: socket creation failed";
        
        struct hostent* server = gethostbyname(host.c_str());
        if (!server) { close(sock); return "Error: DNS resolution failed"; }
        
        struct sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(80);
        memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);
        
        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            close(sock);
            return "Error: connection failed";
        }
        
        std::string request = "GET " + path + " HTTP/1.0\r\nHost: " + host + "\r\n\r\n";
        send(sock, request.c_str(), request.size(), 0);
        
        std::string response;
        char buffer[4096];
        int n;
        while ((n = recv(sock, buffer, sizeof(buffer)-1, 0)) > 0) {
            buffer[n] = '\0';
            response += buffer;
        }
        
        close(sock);
        return response.substr(0, 500); // Return first 500 chars
    }
};

int main() {
    HttpClient client;
    std::cout << "Making HTTP GET request to example.com...\n";
    std::string response = client.get("example.com", "/");
    std::cout << "Response:\n" << response << "\n";
    return 0;
}
