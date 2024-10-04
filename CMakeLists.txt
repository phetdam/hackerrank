cmake_minimum_required(VERSION 3.16)

# major, minor, patch and full version
set(PDHKR_MAJOR_VERSION 0)
set(PDHKR_MINOR_VERSION 1)
set(PDHKR_PATCH_VERSION 0)
set(
    PDHKR_VERSION
    ${PDHKR_MAJOR_VERSION}.${PDHKR_MINOR_VERSION}.${PDHKR_PATCH_VERSION}
)
project(
    hackerrank
    VERSION ${PDHKR_VERSION}
    DESCRIPTION "A collection of solved HackerRank problems"
    HOMEPAGE_URL https://github.com/phetdam/hackerrank
    LANGUAGES CXX
)

# HackerRank uses GCC 8.3.0 and supports C++11, C++14, C++20 as the typical
# available choices of C++ standard. C++20 is used here by default, but it is
# not required, so CMake should allow "graceful" decay into C++17, C++14, etc.
# for that reason, a lot of the code is actually written as C++17, not C++20.
# see http://tinyurl.com/mr3t7vfe for HackerRank environment documentation
set(CMAKE_CXX_STANDARD 20)

# enable AddressSanitizer
option(PDHKR_ENABLE_ASAN "Compile with AddressSanitizer enabled" OFF)

# determine if building with multi-config generator or not
get_property(
    PDHKR_MULTI_CONFIG_GENERATOR
    GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG
)

# multi-config generator
if(PDHKR_MULTI_CONFIG_GENERATOR)
    message(STATUS "Build configs: ${CMAKE_CONFIGURATION_TYPES}")
else()
    message(STATUS "Build configs: ${CMAKE_BUILD_TYPE}")
endif()

# AddressSanitizer
if(PDHKR_ENABLE_ASAN)
    message(STATUS "AddressSanitizer: Enabled")
else()
    message(STATUS "AddressSanitizer: Disabled")
endif()

# put all build artifacts in top-level build directory. multi-config generators
# like Visual Studio will put a per-config subdirectory as well
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# set CMake module path, enable testing, include test helpers
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(CTest)
include(pdhkr_testing)

# compile flags
if(MSVC)
    add_compile_options(
        /Wall
        # warn about unreferenced inline function being removed
        /wd4514
        # warn about copy constructor implicitly defined as deleted
        /wd4625
        # warn about copy assignment operator implicitly defined as deleted
        /wd4626
        # warn about padding added after a data member
        /wd4820
        # warn about MSVC not being able to enforce C++17 left-to-right
        # evaluation order requirement for ->*, [], >>, << operators
        /wd4866
        # warn about MSVC not being able to enforce braced-init-list
        # left-to-right evaluation order requirement
        /wd4868
        # warn about inline function not being inlined
        /wd4710
        # warn about non-inline function being selected for inline expansion
        /wd4711
        # warn about possible performance loss storing 32-bit float in memory
        /wd4738
        # warn about move assignment operator implicitly defined as deleted
        /wd5027
        # warn about Spectre mitigation for memory load with /QSpectre
        /wd5045
    )
else()
    add_compile_options(-Wall)
endif()

# include directory
set(PDHKR_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${PDHKR_INCLUDE_DIR})

add_subdirectory(src)
