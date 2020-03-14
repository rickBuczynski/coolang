cmake_minimum_required (VERSION 3.10)

project(coolang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COOLANG_LLVM_VERSION 8)

# on windows we can't run "clang++-8" we have to run "clang++ --version" and make sure it's 8
if(WIN32)
  EXECUTE_PROCESS(COMMAND clang++ --version OUTPUT_VARIABLE CLANG_VERSION_OUTPUT)
  if(NOT CLANG_VERSION_OUTPUT)
    message(FATAL_ERROR "Coolang requires Clang version " ${COOLANG_LLVM_VERSION} " to be installed")
  endif()
  
  string (REGEX REPLACE ".*clang version ([0-9]+)\\.[0-9]+.*" "\\1" COOLANG_CLANG_VERSION ${CLANG_VERSION_OUTPUT})
  if (NOT ${COOLANG_CLANG_VERSION} STREQUAL ${COOLANG_LLVM_VERSION})
    message(FATAL_ERROR "Coolang requires Clang version " ${COOLANG_LLVM_VERSION} " to be installed")
  endif()
  
  set(COOLANG_CLANG_COMMAND clang++)
else()
  EXECUTE_PROCESS(COMMAND clang++-${COOLANG_LLVM_VERSION} --version OUTPUT_VARIABLE CLANG_VERSION_OUTPUT)
  if(NOT CLANG_VERSION_OUTPUT)
    message(FATAL_ERROR "Coolang requires Clang version " ${COOLANG_LLVM_VERSION} " to be installed")
  endif()

  set(COOLANG_CLANG_COMMAND clang++-${COOLANG_LLVM_VERSION})
endif()

find_package(LLVM ${COOLANG_LLVM_VERSION} REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(llvm_libs all)
list(REMOVE_ITEM llvm_libs LLVM)
list(REMOVE_ITEM llvm_libs LTO)
list(REMOVE_ITEM llvm_libs "OptRemarks")

if(MSVC)
  # Prevent GoogleTest from overriding our compiler/linker options
  # when building with Visual Studio
  # see https://github.com/google/googletest/blob/master/googletest/README.md and 
  https://www.testcookbook.com/book/cpp/setting-up-cmake-google-test.html
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(third_party/googletest)
add_subdirectory(third_party/gflags)

# turn on warnings after adding third_party subdirectories
if(MSVC)
  # without /wd4996 we get weird warning from LLVM headers even with external:W0
  # the other /wdXXXX could be fixed/removed
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /wd4267 /wd4244 /wd4996 /experimental:external /external:anglebrackets /external:W0")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

enable_testing()

function(add_coolang_test proj)
  add_executable(${proj}_test test/${proj}_test.cpp)
  target_link_libraries(${proj}_test gtest gtest_main ${PROJECT_NAME})
  # older c++17 compilers reduire linking this to use std::filesystem
  if (NOT MSVC)
    target_link_libraries(${proj}_test stdc++fs)
  endif()
  target_include_directories(${proj}_test SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include)
  target_include_directories(${proj}_test SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/third_party/googletest/googlemock/include)
  add_test(${proj}_test ${proj}_test)
endfunction()

add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(semantic)
add_subdirectory(codegen)
add_subdirectory(stdlib)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} gflags::gflags)
target_link_libraries(${PROJECT_NAME} lexer)
target_link_libraries(${PROJECT_NAME} parser)
target_link_libraries(${PROJECT_NAME} semantic)
target_link_libraries(${PROJECT_NAME} codegen)

  # older c++17 compilers reduire linking this to use std::filesystem
  if (NOT MSVC)
    target_link_libraries(${PROJECT_NAME} stdc++fs)
  endif()
