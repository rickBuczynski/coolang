project(codegen)

set(SOURCES
  src/ast_to_code_map.cpp
  src/codegen.cpp
  src/vtable.cpp
)

set(HEADERS
  include/coolang/codegen/ast_to_code_map.h
  include/coolang/codegen/codegen.h
  include/coolang/codegen/vtable.h
  include/coolang/codegen/io_codegen.h
  include/coolang/codegen/object_codegen.h
  include/coolang/codegen/string_codegen.h
  include/coolang/codegen/c_std.h
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} 
  semantic
  ${llvm_libs}
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}_test test/codegen_test.cpp)
target_link_libraries(${PROJECT_NAME}_test gtest gtest_main ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}_test SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/3rd_party/googletest/googletest/include)
target_include_directories(${PROJECT_NAME}_test SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/3rd_party/googletest/googlemock/include)
target_compile_definitions(${PROJECT_NAME}_test PRIVATE CODEGEN_TEST_DATA_PATH="${PROJECT_SOURCE_DIR}/test/codegen_test_data/")

# Getting rid of MSVC warnings in 3rd party headers.
# target_include_directories with SYSTEM does this for other compilers but as of cmake 3.9 SYSTEM does nothing for MSVC
if(MSVC)
  # Set warnings to disabled (W0) in 3rd_party headers
  # disable warning C4193. For some reason external:I triggers this warning (see: https://developercommunity.visualstudio.com/content/problem/181006/externali-include-paths-not-working.html)
  # disable warning C4996. For some reason this gets through from LLVM headers even with /external:W0
  target_compile_options(${PROJECT_NAME} PRIVATE /experimental:external /external:I ${LLVM_INCLUDE_DIRS} /external:W0 /wd4193 /wd4996)
endif()