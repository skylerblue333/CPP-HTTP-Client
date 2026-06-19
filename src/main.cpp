#include <iostream>
#include <string>
#include <map>
#include <sstream>

class HttpRequest {
public:
    std::string method;
    std::string url;
    std::map<std::string, std::string> headers;
    std::string body;
    
    HttpRequest(const std::string& m, const std::string& u) : method(m), url(u) {}
    
    void set_header(const std::string& key, const std::string& value) {
        headers[key] = value;
    }
    
    std::string serialize() const {
        std::ostringstream oss;
        oss << method << " " << url << " HTTP/1.1\r\n";
        for (const auto& h : headers) {
            oss << h.first << ": " << h.second << "\r\n";
        }
        oss << "\r\n" << body;
        return oss.str();
    }
};

class HttpResponse {
public:
    int status_code;
    std::string body;
    std::map<std::string, std::string> headers;
    
    HttpResponse(int code, const std::string& b) : status_code(code), body(b) {}
    bool ok() const { return status_code >= 200 && status_code < 300; }
};

int main() {
    HttpRequest req("GET", "https://api.example.com/health");
    req.set_header("Accept", "application/json");
    req.set_header("User-Agent", "CPP-HTTP-Client/3.0");
    
    std::cout << "Request serialized:\n" << req.serialize() << "\n";
    
    HttpResponse resp(200, "{\"status\":\"healthy\"}");
    std::cout << "Response OK: " << resp.ok() << "\n";
    std::cout << "Body: " << resp.body << "\n";
    return 0;
}
