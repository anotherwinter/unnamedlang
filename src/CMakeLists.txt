cmake_minimum_required(VERSION 3.28)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(repl ${SRC_FILES})

target_include_directories(repl
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

file(GLOB EXAMPLE_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/examples/*"
)

add_custom_command(TARGET repl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:repl>/examples
)

foreach(file ${EXAMPLE_FILES})
    add_custom_command(TARGET repl POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${file}"
            "$<TARGET_FILE_DIR:repl>/examples"
    )
endforeach()