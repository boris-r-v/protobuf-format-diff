cmake_minimum_required(VERSION 3.10)
project(protobuf-spec-comparator)

option(STATIC "Build static binary, use: -DSTATIC=ON" OFF)
message(STATUS "Build static binary, use: -DSTATIC=ON")

set(CMAKE_CXX_STANDARD 23)
include_directories(".")


find_package(Protobuf CONFIG REQUIRED)

if ( Protobuf_FOUND )
    message( STATUS "Protobuf version : ${Protobuf_VERSION}" )
    message( STATUS "Protobuf include path : ${Protobuf_INCLUDE_DIRS}" )
else()
    message( WARNING "Protobuf package not found -> specify search path via PROTOBUF_ROOT variable")
endif()

enable_testing()
add_subdirectory(tests)

if ( STATIC )
    message( WARNING "Using statis linking check if glibc-static is installed" )
    set ( Protobuf_USE_STATIC_LIBS ON )
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

add_executable(protobuf-spec-compare comparison.cpp main.cpp)
target_link_libraries(protobuf-spec-compare protobuf::libprotobuf )


