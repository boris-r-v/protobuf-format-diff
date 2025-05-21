This tool allows you to compare versions of a Protocol Buffer specification
(entire .proto files or specific message and enum type).
The purpose is to see clearly whether the newer version maintains backwards compatibility and what new features it adds.

## Building

Prerequisites:

- CMake
- make
- C++ compiler
- Protocol Buffer libraries

Steps:

    mkdir build
    cd build
    cmake ..
    make -j4

## Usage

    protobuf-spec-compare -b ../proto/file1.proto -c ../proto/file2.proto -i /data/tools/protoc/include -s cls_gen.CounterAttributes

The program takes a few arguments:

    -b : path to base proto file [mandatory]
    -c : path to comparing proto file [mandatory]
    -h : print usage help and exit [optional]
    -i : path to protobuf include folder [optional]
    -n : use number of fields to fetch it from protobuf scheme, instead of field name [optional]
    -o : show given program option, before comparing report [optional]
    -s : type-name (message or enum) to compare, if skipped - will be comparing whole proto file. It is need to set package name, if packaging is used [optional]


You can add the following options:

- `-n`: Report compatibility of the binary serialization as opposed to the JSON serialization or similar. See below for details.

### Behavior

The definition of a message or enum `type-name` in file1.proto and file2.proto is compared as detailed in the following sections.

If type-name is skipped, then all the messages and enums in file1.proto and file2.proto are compared.
Added and removed messages and enums are reported.

### Message comparison

Message fields are matched by name (default) or by number (when using `-n`).

Fields present in file1 and missing in file2 are reported as removed, and vice-versa for added fields.
Any changes in matching fields are reported.

If two matching fields both have an enum or message type, then those enums and message types are also compared.

### Enum comparison

Enum values are matched by name (a binary comparison mode will be added where values are matched by ID number).

Values present in file1 and missing in file2 are reported as removed, and vice-versa for added fields.
Any changes in maching enum values are reported.


### Static build
    mkdir build
    cd build
    cmake .. -DSTATIC=ON
    make -j4

If the build is not successful due to the lack of static versions of libraries, they need to be installed. 

    make -j10
    [ 25%] Built target run-po-tests
    [ 62%] Built target run-tests
    [ 75%] Linking CXX executable protobuf-spec-compare
    /usr/bin/ld: cannot find -lpthread
    /usr/bin/ld: cannot find -lrt
    /usr/bin/ld: cannot find -lm
    /usr/bin/ld: cannot find -lpthread
    /usr/bin/ld: cannot find -lc
    collect2: error: ld returned 1 exit status
    make[2]: *** [CMakeFiles/protobuf-spec-compare.dir/build.make:181: protobuf-spec-compare] Error 1
    make[1]: *** [CMakeFiles/Makefile2:102: CMakeFiles/protobuf-spec-compare.dir/all] Error 2
    make: *** [Makefile:101: all] Error 2


The required package had to be downloaded and installed manually:

    sudo rpm -ivh --nodeps glibc-static-2.28-251.0.2.el8_10.2.x86_64.rpm

Checking if it is acualy static linking:

    ldd protobuf-spec-compare
        not a dynamic executable

