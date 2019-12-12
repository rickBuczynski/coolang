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

add_executable(${PROJECT_NAME}_test test/semantic_test.cpp)
target_link_libraries(${PROJECT_NAME}_test gtest gtest_main ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}_test SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include)
target_include_directories(${PROJECT_NAME}_test SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/third_party/googletest/googlemock/include)
target_compile_definitions(${PROJECT_NAME}_test PRIVATE SEMANTIC_TEST_DATA_PATH="${PROJECT_SOURCE_DIR}/test/semantic_test_data/")
