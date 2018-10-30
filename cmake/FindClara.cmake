find_path(Clara_INCLUDE_DIRS
    NAMES clara.hpp
    PATHS ${PROJECT_SOURCE_DIR}/thirdparty/Clara
    PATH_SUFFIXES include
)

if(Clara_INCLUDE_DIRS)
  add_library(Clara INTERFACE IMPORTED)
  target_include_directories(Clara INTERFACE ${Clara_INCLUDE_DIRS})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Clara REQUIRED_VARS Clara_INCLUDE_DIRS)