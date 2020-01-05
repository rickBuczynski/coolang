project(codegen)

set(SOURCES
  src/ast_to_code_map.cpp
  src/codegen.cpp
  src/vtable.cpp
  src/platform.cpp
)

set(HEADERS
  include/coolang/codegen/ast_to_code_map.h
  include/coolang/codegen/codegen.h
  include/coolang/codegen/vtable.h
  include/coolang/codegen/io_codegen.h
  include/coolang/codegen/object_codegen.h
  include/coolang/codegen/string_codegen.h
  include/coolang/codegen/c_std.h
  include/coolang/codegen/platform.h
  include/coolang/codegen/linux_platform.h
  include/coolang/codegen/windows_platform.h
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} 
  semantic
  ${llvm_libs}
)

# TODO windows should support 32 and 64 bit
IF (WIN32)
  target_link_libraries(${PROJECT_NAME} gc32ll)
ELSE()
  target_link_libraries(${PROJECT_NAME} gc64ll)
ENDIF()

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/third_party/windows_com/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}_test test/codegen_test.cpp)
target_link_libraries(${PROJECT_NAME}_test gtest gtest_main ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}_test SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include)
target_include_directories(${PROJECT_NAME}_test SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/third_party/googletest/googlemock/include)
target_compile_definitions(${PROJECT_NAME}_test PRIVATE CODEGEN_TEST_DATA_PATH="${CMAKE_SOURCE_DIR}/third_party/gocool/testdata/codegen/")
add_test(${PROJECT_NAME}_test ${PROJECT_NAME}_test)

# Getting rid of MSVC warnings in 3rd party headers.
# target_include_directories with SYSTEM does this for other compilers but as of cmake 3.9 SYSTEM does nothing for MSVC
if(MSVC)
  # Set warnings to disabled (W0) in third_party headers
  # disable warning C4193. For some reason external:I triggers this warning (see: https://developercommunity.visualstudio.com/content/problem/181006/externali-include-paths-not-working.html)
  # disable warning C4996. For some reason this gets through from LLVM headers even with /external:W0
  target_compile_options(${PROJECT_NAME} PRIVATE /experimental:external /external:I ${LLVM_INCLUDE_DIRS} /external:W0 /wd4193 /wd4996)
endif()
