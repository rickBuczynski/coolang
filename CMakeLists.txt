cmake_minimum_required (VERSION 3.10)

project(coolang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(LLVM 8 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(llvm_libs all)
list(REMOVE_ITEM llvm_libs LLVM)
list(REMOVE_ITEM llvm_libs LTO)
list(REMOVE_ITEM llvm_libs "OptRemarks")


if(MSVC)
  # see https://github.com/google/googletest/blob/master/googletest/README.md and https://stackoverflow.com/questions/12540970/how-to-make-gtest-build-mdd-instead-of-mtd-by-default-using-cmake
  # TODO MSVC uses shared crt by default but I should find a way to check and only force shared if using it
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(3rd_party/googletest)
add_subdirectory(3rd_party/gflags)
add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(semantic)
add_subdirectory(codegen)
add_subdirectory(gc)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} gflags::gflags)
target_link_libraries(${PROJECT_NAME} lexer)
target_link_libraries(${PROJECT_NAME} parser)
target_link_libraries(${PROJECT_NAME} semantic)
target_link_libraries(${PROJECT_NAME} codegen)
