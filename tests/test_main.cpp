#include <iostream>
#include <string>
#include <map>
#include <cassert>
#include <sstream>

class HttpRequest {
public:
    std::string method, url, body;
    std::map<std::string, std::string> headers;
    HttpRequest(const std::string& m, const std::string& u) : method(m), url(u) {}
    void set_header(const std::string& k, const std::string& v) { headers[k] = v; }
    std::string serialize() const {
        std::ostringstream oss;
        oss << method << " " << url << " HTTP/1.1\r\n";
        for (const auto& h : headers) oss << h.first << ": " << h.second << "\r\n";
        oss << "\r\n" << body;
        return oss.str();
    }
};

class HttpResponse {
public:
    int status_code; std::string body;
    HttpResponse(int c, const std::string& b) : status_code(c), body(b) {}
    bool ok() const { return status_code >= 200 && status_code < 300; }
};

int main() {
    HttpRequest req("GET", "/health");
    req.set_header("Accept", "application/json");
    assert(req.headers["Accept"] == "application/json");
    assert(req.method == "GET");
    
    HttpResponse r200(200, "OK");
    assert(r200.ok() == true);
    
    HttpResponse r404(404, "Not Found");
    assert(r404.ok() == false);
    
    std::cout << "All HTTP client tests passed!\n";
    return 0;
}
