cmake_minimum_required(VERSION 4.0)

project(subscriber-notifs VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "src/*.h" "src/*.hpp")

add_executable(sub_notifs ${SRC_FILES} ${HEADER_FILES})

target_include_directories(sub_notifs PRIVATE ${HEADER_FILES})

# from: https://cmake.org/cmake/help/latest/module/FindThreads.html
find_package(Threads REQUIRED)
target_link_libraries(sub_notifs PRIVATE Threads::Threads)