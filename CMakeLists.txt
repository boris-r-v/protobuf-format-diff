cmake_minimum_required(VERSION 3.10)
project(protobuf-spec-comparator)

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


add_executable(protobuf-spec-compare comparison.cpp main.cpp)
target_link_libraries(protobuf-spec-compare protobuf::libprotobuf)

enable_testing()

add_subdirectory(tests)
