project(stdlib)

include (${CMAKE_SOURCE_DIR}/third_party/cmrc/CMakeRC.cmake)

set(SOURCES
  gc.cpp
)
add_library(${PROJECT_NAME} ${SOURCES})

function(add_coolang_stdlib fstem)
  add_custom_command(OUTPUT ${fstem}64.ll
    COMMAND ${COOLANG_CLANG_COMMAND} -m64 -S -emit-llvm -fno-exceptions ${fstem}.cpp -o ${fstem}64.ll
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${fstem}.cpp
  )
  add_custom_target(${fstem}64_ll_target DEPENDS ${fstem}64.ll)
  cmrc_add_resource_library(${fstem}64ll ${fstem}64.ll)
  add_dependencies(${fstem}64ll ${fstem}64_ll_target)
  
  # it's possible to support 32bit on linux but it's not common like it is on windows so we just exclude it
  IF (WIN32)
    add_custom_command(OUTPUT ${fstem}32.ll
      COMMAND ${COOLANG_CLANG_COMMAND} -m32 -S -emit-llvm -fno-exceptions ${fstem}.cpp -o ${fstem}32.ll
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      DEPENDS ${fstem}.cpp
    )
    add_custom_target(${fstem}32_ll_target DEPENDS ${fstem}32.ll)
    cmrc_add_resource_library(${fstem}32ll ${fstem}32.ll)
    add_dependencies(${fstem}32ll ${fstem}32_ll_target)
  ENDIF()
endfunction()

add_coolang_stdlib("gc")
