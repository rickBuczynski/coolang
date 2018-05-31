cmake_minimum_required (VERSION 3.10)

project(coolang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  # see https://github.com/google/googletest/blob/master/googletest/README.md and https://stackoverflow.com/questions/12540970/how-to-make-gtest-build-mdd-instead-of-mtd-by-default-using-cmake
  # TODO MSVC uses shared crt by default but I should find a way to check and only force shared if using it
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(3rd_party/googletest)
add_subdirectory(lexer)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} lexer)
