cmake_minimum_required(VERSION 3.8.2)
project(fmatcher)
set(fmatcher_VERSION_MAJOR 0)
set(fmatcher_VERSION_MINOR 1)
set(fmatcher_VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
