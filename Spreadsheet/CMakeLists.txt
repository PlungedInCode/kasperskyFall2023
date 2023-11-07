cmake_minimum_required(VERSION 3.8)

set (This csvreader)

project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

# add_subdirectory(
#     googletest
# )

set(HEADERS
    src/Table.hpp
    src/Cell.hpp
)

set(SOURCES
    src/Table.cpp
    src/Cell.cpp
    main.cpp
)


add_library(Lib STATIC ${SOURCES} ${HEADERS})

add_executable(${This} ${SOURCES})

add_subdirectory(tests)