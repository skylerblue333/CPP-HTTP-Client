#include <curl/curl.h>

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {
constexpr std::size_t kMaxResponseBytes = 1024 * 1024;
constexpr long kConnectTimeoutMs = 2000;
constexpr long kTotalTimeoutMs = 5000;

struct ResponseBuffer {
    std::string body;
    bool overflow = false;
};

bool is_allowed_url(std::string_view url) {
    return url.rfind("https://", 0) == 0 || url.rfind("http://", 0) == 0;
}

std::size_t write_callback(char* ptr, std::size_t size, std::size_t nmemb, void* userdata) {
    auto* buffer = static_cast<ResponseBuffer*>(userdata);
    const std::size_t bytes = size * nmemb;
    if (bytes > kMaxResponseBytes - buffer->body.size()) {
        buffer->overflow = true;
        return 0;
    }
    buffer->body.append(ptr, bytes);
    return bytes;
}

struct HttpResponse {
    long status = 0;
    std::string body;
};

HttpResponse get_url(const std::string& url) {
    if (!is_allowed_url(url)) {
        throw std::invalid_argument("only http:// and https:// URLs are allowed");
    }

    CURL* handle = curl_easy_init();
    if (handle == nullptr) {
        throw std::runtime_error("failed to initialize libcurl");
    }

    ResponseBuffer buffer;
    char error_buffer[CURL_ERROR_SIZE] = {0};

    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_PROTOCOLS_STR, "http,https");
    curl_easy_setopt(handle, CURLOPT_REDIR_PROTOCOLS_STR, "http,https");
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 3L);
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT_MS, kConnectTimeoutMs);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, kTotalTimeoutMs);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "sky-http-client/0.1");
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, error_buffer);
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);

    const CURLcode code = curl_easy_perform(handle);
    long status = 0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &status);
    curl_easy_cleanup(handle);

    if (buffer.overflow) {
        throw std::runtime_error("response exceeded 1 MiB limit");
    }
    if (code != CURLE_OK) {
        const std::string detail = error_buffer[0] != '\0' ? error_buffer : curl_easy_strerror(code);
        throw std::runtime_error("request failed: " + detail);
    }

    return HttpResponse{status, std::move(buffer.body)};
}
}  // namespace

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: sky-http-client <http-or-https-url>\n";
        return 2;
    }

    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        std::cerr << "failed to initialize libcurl globals\n";
        return 1;
    }

    try {
        const HttpResponse response = get_url(argv[1]);
        std::cout << response.body;
        curl_global_cleanup();
        return response.status >= 200 && response.status < 400 ? 0 : 4;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        curl_global_cleanup();
        return 1;
    }
}
