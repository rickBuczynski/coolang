project(stdlib)

include (${CMAKE_SOURCE_DIR}/third_party/cmrc/CMakeRC.cmake)

set(SOURCES
  gc.cpp
  io.cpp
  str.cpp
)
add_library(${PROJECT_NAME} ${SOURCES})

macro(add_coolang_stdlib fstem)
  # CMRC requires the files to be in SOURCE_DIR but it would be better if they were in BINARY_DIR
  add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${fstem}64.ll
    COMMAND ${COOLANG_CLANG_COMMAND} -m64 -S -emit-llvm -fno-exceptions ${fstem}.cpp -o ${fstem}64.ll --define-macro=_CRT_SECURE_NO_WARNINGS
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${fstem}.cpp
  )
  add_custom_target(${fstem}64_ll_target DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${fstem}64.ll)
  set(LL_FILES "${fstem}64.ll;${LL_FILES}")
  set(LL_TARGETS "${fstem}64_ll_target;${LL_TARGETS}")
  
  # it's possible to support 32bit on linux but it's not common like it is on windows so we just exclude it
  IF (WIN32)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${fstem}32.ll
      COMMAND ${COOLANG_CLANG_COMMAND} -m32 -S -emit-llvm -fno-exceptions ${fstem}.cpp -o ${fstem}32.ll --define-macro=_CRT_SECURE_NO_WARNINGS
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      DEPENDS ${fstem}.cpp
    )
    add_custom_target(${fstem}32_ll_target DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${fstem}32.ll)
    set(LL_FILES "${fstem}32.ll;${LL_FILES}")
    set(LL_TARGETS "${fstem}32_ll_target;${LL_TARGETS}")

  ENDIF()
endmacro()

add_coolang_stdlib("gc")
add_coolang_stdlib("io")
add_coolang_stdlib("str")

cmrc_add_resource_library(coolstd ${LL_FILES})
add_dependencies(coolstd ${LL_TARGETS})
