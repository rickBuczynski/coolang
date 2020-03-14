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
  include/coolang/codegen/object_codegen.h
  include/coolang/codegen/c_std.h
  include/coolang/codegen/platform.h
  include/coolang/codegen/linux_platform.h
  include/coolang/codegen/windows_platform.h
  include/coolang/codegen/bitness.h
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} 
  semantic
  ${llvm_libs}
)
target_link_libraries(${PROJECT_NAME} coolstd)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/third_party/windows_com/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_coolang_test(${PROJECT_NAME})
target_compile_definitions(${PROJECT_NAME}_test PRIVATE CODEGEN_TEST_DATA_PATH="${CMAKE_SOURCE_DIR}/third_party/gocool/testdata/codegen/")
