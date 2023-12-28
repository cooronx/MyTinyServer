
cmake_minimum_required(VERSION 3.15)


SET(CMAKE_CXX_COMPILER /usr/bin/clang)
SET(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_EXPORT_COMPILECOMMANDS true)
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_BUILD_TYPE DEBUG)
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb -pthread")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
project(MyTinyServer)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(Server_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
set(Server_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/include)
include_directories(${Server_SRC_INCLUDE_DIR} ${Server_TEST_INCLUDE_DIR})

add_definitions("-Wall -g")

add_subdirectory(src)
add_subdirectory(test)



