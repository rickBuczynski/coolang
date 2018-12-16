project(stdlib)

include (${CMAKE_SOURCE_DIR}/3rd_party/cmrc/CMakeRC.cmake)

# TODO this project is a placeholder so I can edit gc.cpp within VS2017
# should come up with some build system for gc.cpp->gc.ll->llvm module in coolang

# TODO if supporting 32 bit vs 64 bit need to choose 64 bit vs 32 bit version of clang to compile
# seems like by default 32 bit clang compiles a 32 bit program and 64 bit clang compiles a 64 bit program
# using 64 bit clang with -m32 flag seems to make it switch to 32 bit?
# maybe 32 bit clang can use a -m64 flag so I can just rely on flags not which clang a coolang dev has installed

set(SOURCES
  gc.cpp
)

add_custom_command(OUTPUT gc64.ll
  COMMAND clang++ -m64 -S -emit-llvm -fno-exceptions gc.cpp -o gc64.ll
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS gc.cpp
)
add_custom_target(gc64_ll_target DEPENDS gc64.ll)
cmrc_add_resource_library(gc64ll gc64.ll)
add_dependencies(gc64ll gc64_ll_target)

add_custom_command(OUTPUT gc32.ll
  COMMAND clang++ -m32 -S -emit-llvm -fno-exceptions gc.cpp -o gc32.ll
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS gc.cpp
)
add_custom_target(gc32_ll_target DEPENDS gc32.ll)
cmrc_add_resource_library(gc32ll gc32.ll)
add_dependencies(gc32ll gc32_ll_target)

add_library(${PROJECT_NAME} ${SOURCES})
