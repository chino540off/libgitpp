cmake_minimum_required(VERSION 3.10)

project(libgitpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -Werror -Wno-multichar -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG " -O0 -ggdb -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_RELEASE " -O2")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

# For some external project macros
include(FetchContent)

# Download libgit2 from git and build it
FetchContent_Declare(
  libgit2
  GIT_REPOSITORY https://github.com/libgit2/libgit2.git
  GIT_TAG v1.8.1
  SYSTEM OVERRIDE_FIND_PACKAGE)
set(BUILD_TESTS
    OFF
    CACHE BOOL "" FORCE)
set(BUILD_CLI
    OFF
    CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES
    OFF
    CACHE BOOL "" FORCE)
set(BUILD_FUZZERS
    OFF
    CACHE BOOL "" FORCE)
set(libgit2_INSTALL YES)
FetchContent_MakeAvailable(libgit2)

add_library(libgitpp INTERFACE)
target_sources(
  libgitpp
  INTERFACE ${PROJECT_SOURCE_DIR}/git/reference.hh
            ${PROJECT_SOURCE_DIR}/git/exceptions.hh
            ${PROJECT_SOURCE_DIR}/git/blob.hh
            ${PROJECT_SOURCE_DIR}/git/repository.hh
            ${PROJECT_SOURCE_DIR}/git/tree.hh
            ${PROJECT_SOURCE_DIR}/git/oid.hh
            ${PROJECT_SOURCE_DIR}/git/commit.hh)
target_include_directories(libgitpp INTERFACE ${PROJECT_SOURCE_DIR})
target_link_libraries(libgitpp INTERFACE libgit2package libgit2)

#
# Tests
#
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  # MEMORYCHECK_COMMAND_OPTIONS needs to place prior to
  # CTEST_MEMORYCHECK_COMMAND
  set(MEMORYCHECK_COMMAND_OPTIONS
      "--leak-check=full
      --show-leak-kinds=definite,possible
      --error-exitcode=1
      --suppressions=${LIBGIT2_SOURCE_DIR}/libgit2_clar.supp")
  set(CTEST_MEMORYCHECK_COMMAND valgrind)

  include(CTest)

  add_subdirectory(tests)
endif()
