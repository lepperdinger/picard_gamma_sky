cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
project(gamma_sky)

find_package(HDF5 REQUIRED)
include_directories(${HDF5_INCLUDE_DIRS})

find_package(TBB REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(src)
file(GLOB SRC CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")
add_executable(gamma_sky ${SRC})
target_link_libraries(gamma_sky ${HDF5_LIBRARIES} TBB::tbb)
target_compile_options(gamma_sky PRIVATE -Wall -Wextra -Wpedantic) #-Werror)

enable_testing()

