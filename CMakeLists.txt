cmake_minimum_required(VERSION 3.10)
project(protobuf-spec-comparator)

option(STATIC "Build static binary, use: -DSTATIC=ON" OFF)
message(STATUS "Build static binary, use: -DSTATIC=ON")

set(CMAKE_CXX_STANDARD 23)
include_directories(".")


find_package(Protobuf CONFIG REQUIRED)
set ( Protobuf_USE_STATIC_LIBS ON )
if ( Protobuf_FOUND )
    message( STATUS "Protobuf version : ${Protobuf_VERSION}" )
    message( STATUS "Protobuf include path : ${Protobuf_INCLUDE_DIRS}" )
    message( STATUS "Protobuf libraries : ${Protobuf_LIBRARIES}" )
    message( STATUS "Protobuf compiler libraries : ${Protobuf_PROTOC_LIBRARIES}")
    message( STATUS "Protobuf lite libraries : ${Protobuf_LITE_LIBRARIES}")
    message( STATUS "Protobuf use static : ${Protobuf_USE_STATIC_LIBS}" )
else()
    message( WARNING "Protobuf package not found -> specify search path via PROTOBUF_ROOT variable")
endif()

enable_testing()
add_subdirectory(tests)

if ( STATIC )
    message( WARNING "Using statis linking check if glibc-static is installed" )
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

add_executable(protobuf-spec-compare comparison.cpp main.cpp)
target_link_libraries(protobuf-spec-compare protobuf::libprotobuf )


