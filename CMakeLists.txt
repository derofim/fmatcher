cmake_minimum_required (VERSION 2.8)

project (main)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory (src) 

add_subdirectory (test)

# Run test on each change in source files
#add_dependencies(fmatcher_check TEST_check)
#add_dependencies(fmatcher_check_verbose TEST_check_verbose)

#enable_testing()
