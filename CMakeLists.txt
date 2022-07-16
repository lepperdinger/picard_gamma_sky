cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
project(gamma_sky)

# libraries ####################################################################

# HDF5 library for reading and writing to HDF5 files
find_package(HDF5 REQUIRED)
include_directories(${HDF5_INCLUDE_DIRS})

# TBB library for parallelization
find_package(TBB REQUIRED)

# GoogleTest framework for testing
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# HealPix
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(HealPix REQUIRED)

# gamma_sky ####################################################################

include_directories(src ${HEALPIX_INCLUDE_DIR})
file(GLOB SRC CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")
add_executable(gamma_sky apps/gamma_sky.cpp ${SRC})
target_link_libraries(gamma_sky ${HDF5_LIBRARIES} ${HEALPIX_LIBRARIES}
                      TBB::tbb)
target_compile_options(gamma_sky PRIVATE -Wall -Wextra -Wpedantic) # -Werror)

# tests ########################################################################

foreach(TEST_NAME test_mathematics test_grids)
  add_executable(${TEST_NAME} test/${TEST_NAME}.cpp ${SRC})
  target_link_libraries(${TEST_NAME} gtest gtest_main ${HDF5_LIBRARIES}
                        ${HEALPIX_LIBRARIES} TBB::tbb)
  add_test(${TEST_NAME} test_mathematics)
endforeach()

enable_testing()

