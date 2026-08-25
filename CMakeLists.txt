cmake_minimum_required(VERSION 3.16)
project(sky_http_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL REQUIRED)

add_executable(app src/main.cpp)
add_executable(test_app tests/test_main.cpp)

target_link_libraries(app PRIVATE CURL::libcurl)
target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_options(test_app PRIVATE -Wall -Wextra -Wpedantic -Werror)

enable_testing()
add_test(NAME http_client_contract COMMAND test_app)
set_tests_properties(http_client_contract PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
