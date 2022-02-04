cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(PAD01_SCAN LANGUAGES CXX) 

## Get external content
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG       v2.13.7
)
FetchContent_MakeAvailable(Catch2)

#set(CMAKE_PREFIX_PATH "/home/oneapi-tbb-2021.5.0/lib/cmake")

find_package(TBB REQUIRED)

## set global compiler options
set(CMAKE_CXX_STANDARD 20)
#TODO: Compiler Flags!
if(CMAKE_COMPILER_IS_GNUCXX)
  message(STATUS "GCC detected, adding compile flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -ltbb")
endif(CMAKE_COMPILER_IS_GNUCXX)
## icpx flags are still iffy. May work or not.
if(CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
  message(STATUS "Intel compiler detected, adding compile flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -ltbb")
endif()


set(CMAKE_POSITION_INDEPENDENT_CODE ON)

OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
  FIND_PACKAGE(OpenMP)
  IF(OPENMP_FOUND)
    IF(CMAKE_COMPILER_IS_GNUCXX)
      SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    ENDIF()
  ENDIF()
ENDIF()

## define targets

add_library(scan STATIC
  include/scan.hpp
  include/scan.cpp
  include/scan-sequential-naive.hpp
  include/scan-sequential-updown.hpp
  include/scan-sequential-tiled.hpp
  include/scan-openmp-provided.hpp
  include/scan-openmp-updown.hpp
  include/scan-openmp-tiled.hpp
  include/scan-tbb-provided.hpp
  include/scan-tbb-updown.hpp
  include/scan-tbb-tiled.hpp  
  )

add_executable(test
  test/test-main.cpp
  test/test.cpp
  )

target_include_directories(test PRIVATE common)

add_executable(bench
  benchmark/benchmark-main.cpp
  benchmark/benchmark.cpp
  common/csv_reporter.hpp
  common/csv_reporter.cpp
  common/logrange_generator.hpp
  )

target_compile_definitions(bench PRIVATE CATCH_CONFIG_ENABLE_BENCHMARKING)
target_include_directories(bench PRIVATE common)

# define target linkage

target_include_directories(scan PUBLIC include)

target_link_libraries(bench PUBLIC scan Catch2 TBB::tbb)

target_link_libraries(test PUBLIC scan Catch2 TBB::tbb)

target_compile_features(bench PRIVATE cxx_std_11)
target_compile_features(test PRIVATE cxx_std_11)
