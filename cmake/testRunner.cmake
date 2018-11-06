# Load addional modules
include(ProcessorCount)
ProcessorCount(NUM_PROCESSORS)
set(NUM_PROCESSORS ${NUM_PROCESSORS} CACHE STRING "Processor count")

# Run tests with print on failure
add_custom_target(${PROJECT_NAME}_check 
    ${CMAKE_COMMAND} -E echo CWD=${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E echo CMD=${CMAKE_CTEST_COMMAND} -C $<CONFIG>
    COMMAND ${CMAKE_COMMAND} -E echo ----------------------------------
    COMMAND ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1 ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
    COMMAND ${CMAKE_COMMAND} -E echo ----------------------------------
)

# Run tests with verbose output 
add_custom_target(${PROJECT_NAME}_check_verbose 
    COMMAND ${CMAKE_COMMAND} -E echo ----------------------------------
    COMMAND ${CMAKE_CTEST_COMMAND}
        --force-new-ctest-process
        --verbose
        --output-on-failure
        ${CONFIG_PARAMETER} --parallel ${NUM_PROCESSORS} 
        #--test-action test
    COMMAND ${CMAKE_COMMAND} -E echo ----------------------------------
)