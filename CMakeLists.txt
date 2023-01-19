cmake_minimum_required(VERSION 3.17)
project(webtrade-tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(googletest)

set(TESTS_SOURCES
    OrderTests.cpp
)

enable_testing()

include(GoogleTest)

add_executable(${PROJECT_NAME} ${TESTS_SOURCES})
add_definitions(-DUNIT_TESTS)
target_link_libraries(${PROJECT_NAME} PRIVATE gmock_main gtest_main)
gtest_discover_tests(${PROJECT_NAME})