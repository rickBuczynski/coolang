project(gc)

include (${CMAKE_SOURCE_DIR}/third_party/cmrc/CMakeRC.cmake)

set(SOURCES
  gc.cpp
)

add_custom_command(OUTPUT gc64.ll
  COMMAND clang++-${COOLANG_LLVM_VERSION} -m64 -S -emit-llvm -fno-exceptions gc.cpp -o gc64.ll
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS gc.cpp
)
add_custom_target(gc64_ll_target DEPENDS gc64.ll)
cmrc_add_resource_library(gc64ll gc64.ll)
add_dependencies(gc64ll gc64_ll_target)

add_custom_command(OUTPUT gc32.ll
  COMMAND clang++-${COOLANG_LLVM_VERSION} -m32 -S -emit-llvm -fno-exceptions gc.cpp -o gc32.ll
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS gc.cpp
)
add_custom_target(gc32_ll_target DEPENDS gc32.ll)
cmrc_add_resource_library(gc32ll gc32.ll)
add_dependencies(gc32ll gc32_ll_target)

add_library(${PROJECT_NAME} ${SOURCES})
