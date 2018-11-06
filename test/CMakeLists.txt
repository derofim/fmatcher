cmake_minimum_required (VERSION 2.8)
project (TEST)

include_directories (
    ${TEST_SOURCE_DIR}/src # https://cmake.org/cmake/help/v3.6/variable/PROJECT-NAME_SOURCE_DIR.html
)

set(TEST_SOURCE_FILES
    main.cpp
)

add_library(test_main OBJECT ${TEST_SOURCE_FILES})

add_executable(MainTest argparse.test.cpp $<TARGET_OBJECTS:test_main>)

#add_dependencies(MainTest fmatcher)

#target_link_libraries(lib2 lib1 ${lib2_deps})fmatcher

enable_testing()

#add_test(NAME fmatcher1 COMMAND fmatcher 100)

add_test(NAME MainTest COMMAND MainTest)

include(testRunner)

#target_link_libraries(test_main fmatcher)

add_custom_target(ctest-cleanup 
    ${CMAKE_COMMAND} -E remove Tests/ctest.log
    ${CMAKE_COMMAND} -E remove Tests
    ${CMAKE_COMMAND} -E remove Testing
    ${CMAKE_COMMAND} -E remove test
)

# Force running test after build (after each change in source files)
add_custom_command(TARGET MainTest
    POST_BUILD
    COMMAND  ${CMAKE_COMMAND} --build . --target ${PROJECT_NAME}_check_verbose
    DEPENDS ogs vtkdiff ctest-cleanup
)