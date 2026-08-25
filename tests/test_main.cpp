#include <cassert>
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

int main() {
    assert(exit_code("./app ftp://example.com >/dev/null 2>&1") == 1);
    assert(exit_code("./app >/dev/null 2>&1") == 2);
    assert(exit_code("./app http://127.0.0.1:18081/fixture.txt >/tmp/sky-http-body.txt") == 0);
    assert(exit_code("grep -q 'sky-http-client-fixture' /tmp/sky-http-body.txt") == 0);
    assert(exit_code("./app http://127.0.0.1:18081/missing >/dev/null 2>&1") == 4);
    std::cout << "HTTP client contract tests passed\n";
    return 0;
}
