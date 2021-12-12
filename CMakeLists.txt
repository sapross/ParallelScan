cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(PAD01_SCAN LANGUAGES CXX) 

## Get external content
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG       v2.13.3
)

FetchContent_MakeAvailable(Catch2)

## set global compiler options
set(CMAKE_CXX_STANDARD 17)
#TODO: Compiler Flags!
if(CMAKE_COMPILER_IS_GNUCXX)
  message(STATUS "GCC detected, adding compile flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

## define targets

add_library(lib STATIC
  include/*.hpp
  include/*.cpp
  )

add_executable(test
 test/test.cpp
  )



add_executable(bench
  benchmark/benchmark.cpp
  common/csv_reporter.hpp
  common/csv_reporter.cpp
  common/logrange_generator.hpp
  )

target_compile_definitions(bench PRIVATE CATCH_CONFIG_ENABLE_BENCHMARKING)
target_include_directories(bench PRIVATE common)

# define target linkage

target_include_directories(lib PUBLIC include)

target_link_libraries(bench PUBLIC lib Catch2)

target_link_libraries(test PUBLIC lib Catch2)


