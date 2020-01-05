project(parser)

set(SOURCES
  src/parser.cpp
  src/ast.cpp
)

set(HEADERS
  include/coolang/parser/parser.h
  include/coolang/parser/ast.h
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} lexer)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(parser_test test/parser_test.cpp)
target_link_libraries(parser_test gtest gtest_main ${PROJECT_NAME})
target_include_directories(parser_test SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include)
target_include_directories(parser_test SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/third_party/googletest/googlemock/include)
target_compile_definitions(parser_test PRIVATE PARSER_TEST_DATA_PATH="${CMAKE_SOURCE_DIR}/third_party/gocool/testdata/parse/")
add_test(${PROJECT_NAME}_test ${PROJECT_NAME}_test)
