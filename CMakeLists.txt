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

## set global compiler options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


find_package(TBB REQUIRED)
IF(TBB_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TBB_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TBB_CXX_FLAGS}")
ENDIF()
find_package(OpenMP REQUIRED)
IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ENDIF()

## define targets

add_library(scan STATIC
  include/scan.hpp
  include/scan.cpp
  include/scan-naive.hpp
  include/scan-openmp.hpp
  include/scan-tbb.hpp
  )
target_include_directories(scan PUBLIC include)

add_executable(test
  test/test-main.cpp
  test/test.cpp
  )

target_compile_features(test PRIVATE cxx_std_20)
target_link_libraries(test PUBLIC scan Catch2 TBB::tbb)
target_include_directories(test PRIVATE common)

if(NOT ARCHSPECIFIC)
  add_executable(bench-memory
	benchmark/benchmark-main.cpp
	benchmark/benchmark-memory.cpp
	common/csv_reporter.hpp
	common/csv_reporter.cpp
	common/logrange_generator.hpp
	)
  target_compile_definitions(bench-memory PRIVATE CATCH_CONFIG_ENABLE_BENCHMARKING)
  target_compile_features(bench-memory PRIVATE cxx_std_20)
  target_include_directories(bench-memory PRIVATE common)
  target_link_libraries(bench-memory PUBLIC scan Catch2 TBB::tbb)

  add_executable(bench-analytical
	benchmark/benchmark-main.cpp
	benchmark/benchmark-analytical.cpp
	common/csv_reporter.hpp
	common/csv_reporter.cpp
	common/logrange_generator.hpp
	)
  target_compile_definitions(bench-analytical PRIVATE CATCH_CONFIG_ENABLE_BENCHMARKING)
  target_compile_features(bench-analytical PRIVATE cxx_std_20)
  target_include_directories(bench-analytical PRIVATE common)
  target_link_libraries(bench-analytical PUBLIC scan Catch2 TBB::tbb)
else()

  ## mp-media specific executables
  add_executable(bench-memory-media
	benchmark/benchmark-main.cpp
	benchmark/benchmark-memory.cpp
	common/csv_reporter.hpp
	common/csv_reporter.cpp
	common/logrange_generator.hpp
	)
  target_compile_definitions(bench-memory-media PRIVATE CATCH_CONFIG_ENABLE_BENCHMARKING)
  target_compile_features(bench-memory-media PRIVATE cxx_std_20)
  target_include_directories(bench-memory-media PRIVATE common)
  target_link_libraries(bench-memory-media PUBLIC scan Catch2 TBB::tbb)
  target_compile_options(
	bench-memory-media
	PRIVATE
	-march=skylake
	-mprefer-vector-width=256
	-static
	)


  add_executable(bench-analytical-media
	benchmark/benchmark-main.cpp
	benchmark/benchmark-analytical.cpp
	common/csv_reporter.hpp
	common/csv_reporter.cpp
	common/logrange_generator.hpp
	)
  target_compile_definitions(bench-analytical-media PRIVATE CATCH_CONFIG_ENABLE_BENCHMARKING)
  target_compile_features(bench-analytical-media PRIVATE cxx_std_20)
  target_include_directories(bench-analytical-media PRIVATE common)
  target_link_libraries(bench-analytical-media PUBLIC scan Catch2 TBB::tbb)
  target_compile_options(
	bench-memory-media
	PRIVATE
	-march=skylake
	-mprefer-vector-width=256
	-static
	)
  
  ## ziti-rome specific executables
  add_executable(bench-memory-rome
	benchmark/benchmark-main.cpp
	benchmark/benchmark-memory.cpp
	common/csv_reporter.hpp
	common/csv_reporter.cpp
	common/logrange_generator.hpp
	)
  target_compile_definitions(bench-memory-rome PRIVATE CATCH_CONFIG_ENABLE_BENCHMARKING)
  target_compile_features(bench-memory-rome PRIVATE cxx_std_20)
  target_include_directories(bench-memory-rome PRIVATE common)
  target_link_libraries(bench-memory-rome PUBLIC scan Catch2 TBB::tbb)
  target_compile_options(
	bench-memory-rome
	PRIVATE
    -march=znver2
    -mprefer-vector-width=256
    -static
	)


  add_executable(bench-analytical-rome
	benchmark/benchmark-main.cpp
	benchmark/benchmark-analytical.cpp
	common/csv_reporter.hpp
	common/csv_reporter.cpp
	common/logrange_generator.hpp
	)
  target_compile_definitions(bench-analytical-rome PRIVATE CATCH_CONFIG_ENABLE_BENCHMARKING)
  target_compile_features(bench-analytical-rome PRIVATE cxx_std_20)
  target_include_directories(bench-analytical-rome PRIVATE common)
  target_link_libraries(bench-analytical-rome PUBLIC scan Catch2 TBB::tbb)
  target_compile_options(
	bench-memory-rome
	PRIVATE
    -march=znver2
    -mprefer-vector-width=256
    -static
	)

  
endif()

# define target linkage


