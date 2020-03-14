project(lexer)

set(SOURCES
  src/lexer.cpp
  src/token.cpp
  src/char_stream.cpp
)

set(HEADERS
  include/coolang/lexer/lexer.h
  include/coolang/lexer/token.h
  include/coolang/lexer/char_stream.h
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_coolang_test(${PROJECT_NAME})
target_compile_definitions(lexer_test PRIVATE LEXER_TEST_DATA_PATH="${CMAKE_SOURCE_DIR}/third_party/gocool/testdata/lex/")
