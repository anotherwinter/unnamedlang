cmake_minimum_required(VERSION 3.28)
project(UnnamedLang VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake_config/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h.in
)

enable_testing()
add_subdirectory(src)
add_subdirectory(tests)
