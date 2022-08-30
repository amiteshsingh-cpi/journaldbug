cmake_minimum_required(VERSION 3.22)

project("test")
find_package(spdlog REQUIRED)
add_executable(test main.cpp)

target_link_libraries(test PRIVATE systemd)
