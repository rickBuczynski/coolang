project(stdlib)

# TODO this project is a placeholder so I can edit gc.cpp within VS2017
# should come up with some build system for gc.cpp->gc.ll->llvm module in coolang

set(SOURCES
  gc.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
