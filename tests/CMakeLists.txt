
add_executable(run-tests test.cpp ../comparison.cpp)
target_link_libraries(run-tests protobuf::libprotobuf)

function(add_comparison_test_w_options dir_name options)
  message(STATUS "Adding test ${dir_name} ${options}")
  add_test(NAME "${dir_name}" COMMAND
          run-tests "${dir_name}" ${options}
          WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endfunction()

function(add_comparison_test dir_name)
  add_comparison_test_w_options("${dir_name}" "")
endfunction()

find_package(Boost REQUIRED COMPONENTS program_options)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)
#message(STATUS "Boost include dir: ${Boost_INCLUDE_DIRS} ")
#message(STATUS "Boost library dir: ${Boost_LIBRARY_DIRS} ")
#message(STATUS "Link with folowing boost libs: ${Boost_LIBRARIES} ")

add_executable(run-po-tests test-op.cpp )
target_link_libraries(run-po-tests ${Boost_LIBRARIES} )

function(add_program_option_test dir_name)
  message(STATUS "Adding program option test ${dir_name}")
  add_test(NAME "${dir_name}" COMMAND
          run-po-tests "${dir_name}"
          WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endfunction()



add_comparison_test(msg_added)
add_comparison_test(msg_removed)
add_comparison_test(enum_added)
add_comparison_test(enum_removed)
add_comparison_test(enum_value_added)
add_comparison_test(enum_value_removed)
add_comparison_test(enum_value_id_changed)
add_comparison_test(field_added)
add_comparison_test(field_removed)
add_comparison_test(field_id_changed)
add_comparison_test(field_label_changed)
add_comparison_test(field_type_changed)
add_comparison_test(field_default_value_changed)
add_comparison_test(field_message_type_name_changed)
add_comparison_test(field_message_type_changed)
add_comparison_test(field_enum_type_name_changed)
add_comparison_test(field_enum_type_changed)
add_comparison_test(msg_recursion)
add_comparison_test_w_options(binary_message_diff use_field_number)
add_comparison_test_w_options(binary_enum_diff use_field_number)
add_program_option_test(program_option)
