project(gc)

include (${CMAKE_SOURCE_DIR}/third_party/cmrc/CMakeRC.cmake)

set(SOURCES
  gc.cpp
)

add_custom_command(OUTPUT gc64.ll
  COMMAND ${COOLANG_CLANG_COMMAND} -m64 -S -emit-llvm -fno-exceptions gc.cpp -o gc64.ll
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS gc.cpp
)
add_custom_target(gc64_ll_target DEPENDS gc64.ll)
cmrc_add_resource_library(gc64ll gc64.ll)
add_dependencies(gc64ll gc64_ll_target)

# it's possible to support 32bit on linux but it's not common like it is on windows so we just exclude it
IF (WIN32)
  add_custom_command(OUTPUT gc32.ll
    COMMAND ${COOLANG_CLANG_COMMAND} -m32 -S -emit-llvm -fno-exceptions gc.cpp -o gc32.ll
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS gc.cpp
  )
  add_custom_target(gc32_ll_target DEPENDS gc32.ll)
  cmrc_add_resource_library(gc32ll gc32.ll)
  add_dependencies(gc32ll gc32_ll_target)
ENDIF()

add_library(${PROJECT_NAME} ${SOURCES})
