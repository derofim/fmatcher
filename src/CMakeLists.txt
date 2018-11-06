cmake_minimum_required(VERSION 3.8.2)
project(fmatcher)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(SOURCE_FILES
    main.cpp
)

add_executable(fmatcher ${SOURCE_FILES})

set_property(TARGET fmatcher PROPERTY CXX_STANDARD 14)
set_property(TARGET fmatcher PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET fmatcher PROPERTY CXX_EXTENSIONS OFF)

if( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
    target_compile_options( fmatcher PRIVATE -Wall -Wextra -pedantic -Werror )
endif()
if( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
	target_compile_options( fmatcher PRIVATE /W4 /WX )
endif()

#include(testRunner)

# Force running test after build (after each change in source files)
#add_custom_command(TARGET fmatcher
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E echo "Running POST_BUILD steps..."
#)

#add_dependencies(fmatcher MainTest)

#add_test(NAME MyTest COMMAND MyTest)

#enable_testing()
#add_test(NAME fmatcher COMMAND fmatcher 100)