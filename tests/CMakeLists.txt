cmake_minimum_required(VERSION 3.28)

add_library(core
    ../src/front/lexer.cpp
    ../src/front/dfalg_out.cpp
    ../src/front/parser.cpp
    ../src/front/ast.cpp
    ../src/front/bison_wrapper.cpp
    ../src/diagnostics.cpp
    ../src/initializer.cpp
    ../src/front/typedast.cpp
    ../src/front/typedastbuilder.cpp
    ../src/alloc/arena.cpp
    ../src/front/typedastanalyzer.cpp
    ../src/front/symbolregistry.cpp
    ../src/front/scope.cpp
    ../src/front/nodeallocator.cpp
    ../src/middle/tac.cpp
    ../src/middle/ssastate.cpp
    ../src/middle/layoutregistry.cpp
    ../src/middle/tacbuilder.cpp
    ../src/middle/tacoptimizer.cpp
    ../src/middle/foldfunctions.cpp
)

target_include_directories(core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(core PRIVATE DBG)

add_executable(test_lexer test_lexer.cpp)
target_compile_definitions(test_lexer PRIVATE DBG)
target_link_libraries(test_lexer core)

add_executable(test_parser test_parser.cpp)
target_compile_definitions(test_parser PRIVATE DBG)
target_link_libraries(test_parser core)

add_executable(test_typedast test_typedast.cpp)
target_compile_definitions(test_typedast PRIVATE DBG)
target_link_libraries(test_typedast core)

add_executable(test_tac test_tac.cpp)
target_compile_definitions(test_tac PRIVATE DBG)
target_link_libraries(test_tac core)

file(GLOB EXAMPLE_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/examples/*"
)

add_custom_command(TARGET core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:core>/examples
)

foreach(file ${EXAMPLE_FILES})
    add_custom_command(TARGET core POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${file}"
            "$<TARGET_FILE_DIR:core>/examples"
    )
endforeach()