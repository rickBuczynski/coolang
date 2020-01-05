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
# TODO forcing lexer to link with stdc++fs forces the other projects to link it to (since they all depend on lexer). Is there a better way?
# TODO right now even when compiling with clang we're still using gcc's std lib
# see https://stackoverflow.com/questions/52427986/cannot-use-stdfilesystem-even-if-declared-supported
if (NOT MSVC)
  target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(lexer_test test/lexer_test.cpp)
target_link_libraries(lexer_test gtest gtest_main ${PROJECT_NAME})
target_include_directories(lexer_test SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include)
target_include_directories(lexer_test SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/third_party/googletest/googlemock/include)
target_compile_definitions(lexer_test PRIVATE LEXER_TEST_DATA_PATH="${CMAKE_SOURCE_DIR}/third_party/gocool/testdata/lex/")
add_test(${PROJECT_NAME}_test ${PROJECT_NAME}_test)
