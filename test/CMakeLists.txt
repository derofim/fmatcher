cmake_minimum_required (VERSION 2.8)
project (TEST)

include_directories (
    ${TEST_SOURCE_DIR}/src # https://cmake.org/cmake/help/v3.6/variable/PROJECT-NAME_SOURCE_DIR.html
)

#set(SOURCE_FILES
#    test.cpp
#)

enable_testing()
add_library(test_main OBJECT main.cpp)
add_executable(MyTest argparse.test.cpp $<TARGET_OBJECTS:test_main>)
add_test(NAME MyTest COMMAND MyTest)

#add_executable (Test ${SOURCE_FILES})