project(compiler)

set(SOURCES
  src/lexer.cpp
)

set(HEADERS
  include/coolang/compiler/lexer.h
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
