cmake_minimum_required(VERSION 3.14)
project(ApartmentPlanner CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Всі файли логіки без main.cpp
file(GLOB LIB_SOURCES "src/*.cpp")
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

# 1. Головна програма
add_executable(${PROJECT_NAME} src/main.cpp ${LIB_SOURCES})
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})

# 2. Юніт-тести
enable_testing()
add_executable(RunTests tests/test_main.cpp ${LIB_SOURCES})
add_test(NAME UnitTests COMMAND RunTests)