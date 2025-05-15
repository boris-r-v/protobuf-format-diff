cmake_minimum_required(VERSION 3.10)
project(protobuf-spec-comparator)

find_package(Protobuf CONFIG REQUIRED)

add_executable(protobuf-spec-compare comparison.cpp main.cpp)
target_link_libraries(protobuf-spec-compare protobuf::libprotobuf)

enable_testing()

add_subdirectory(tests)
