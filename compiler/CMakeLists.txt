project(compiler)

set(SOURCES
  src/lexer.cpp
  src/token.cpp
)

set(HEADERS
  include/coolang/compiler/lexer.h
  include/coolang/compiler/token.h
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(lexer_test test/lexer_test.cpp)
target_link_libraries(lexer_test gtest gtest_main ${PROJECT_NAME})
target_include_directories(lexer_test SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/3rd_party/googletest/googletest/include)
target_include_directories(lexer_test SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/3rd_party/googletest/googlemock/include)
target_compile_definitions(lexer_test PRIVATE LEXER_TEST_DATA_PATH="${PROJECT_SOURCE_DIR}/test/lexer_test_data")