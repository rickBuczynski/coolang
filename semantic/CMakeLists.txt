project(semantic)

set(SOURCES
  src/semantic.cpp
  src/type_checker.cpp
)

set(HEADERS
  include/coolang/semantic/semantic.h
  include/coolang/semantic/inheritance_checker.h
  include/coolang/semantic/semantic_error.h
  include/coolang/semantic/type_checker.h
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} parser)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_coolang_test(${PROJECT_NAME})
target_compile_definitions(${PROJECT_NAME}_test PRIVATE SEMANTIC_TEST_DATA_PATH="${CMAKE_SOURCE_DIR}/third_party/gocool/testdata/typecheck/")
