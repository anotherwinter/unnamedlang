cmake_minimum_required(VERSION 3.28)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

file(GLOB EXAMPLE_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/examples/*"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/examples
)

foreach(file ${EXAMPLE_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${file}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/examples"
    )
endforeach()