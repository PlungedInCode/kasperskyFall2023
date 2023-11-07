cmake_minimum_required(VERSION 3.8)

set (This csvreaderTests)

set(SOURCES
    tests.cpp
    ../src/Table.cpp
    ../src/Cell.cpp
)

add_executable(${This} ${SOURCES})

target_link_libraries( ${This} PUBLIC
    gtest_main
)

# Specify the include directories for your test target
target_include_directories(${This} PRIVATE ${CMAKE_SOURCE_DIR}/src)


add_test(
    NAME ${This}
    COMMAND ${This}
)
