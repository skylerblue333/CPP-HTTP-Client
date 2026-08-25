#include <cstdlib>
#include <iostream>
#include <sys/wait.h>

static int exit_code(const char* command) {
    const int status = std::system(command);
    if (status == -1 || !WIFEXITED(status)) {
        return -1;
    }
    return WEXITSTATUS(status);
}

static int expect_exit(const char* command, int expected) {
    const int actual = exit_code(command);
    if (actual != expected) {
        std::cerr << "command failed expectation: " << command << " (expected " << expected
                  << ", got " << actual << ")\n";
        return 1;
    }
    return 0;
}

int main() {
    int failures = 0;
    failures += expect_exit("./app ftp://example.com >/dev/null 2>&1", 1);
    failures += expect_exit("./app >/dev/null 2>&1", 2);
    failures += expect_exit("./app http://127.0.0.1:18081/fixture.txt >/tmp/sky-http-body.txt", 0);
    failures += expect_exit("grep -q 'sky-http-client-fixture' /tmp/sky-http-body.txt", 0);
    failures += expect_exit("./app http://127.0.0.1:18081/missing >/dev/null 2>&1", 4);
    if (failures != 0) {
        return 1;
    }
    std::cout << "HTTP client contract tests passed\n";
    return 0;
}
