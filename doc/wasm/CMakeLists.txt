# Builds the in-browser VM used by the "Run this code" button. See build_wasm.sh.
#
# The four libraries are pulled in by add_subdirectory rather than by a source list
# of our own, so the set of files can never drift from the library build.

cmake_minimum_required(VERSION 3.13)
project(quirrel-wasm LANGUAGES CXX)

set(QUIRREL "${PROJECT_SOURCE_DIR}/../..")
set(CMAKE_CXX_STANDARD 17)

# SQ_SYSTEM_STUBS=0 keeps getenv, setenv and system in the system module, so the browser
# signatures match the ones the site documents. system() itself fails at runtime,
# which is the honest answer for a sandbox.
add_compile_definitions(_SQ64 SQ_VAR_TRACE_ENABLED=0 SQ_WATCHDOG_ENABLED=1 SQ_SYSTEM_STUBS=0)

# Exceptions everywhere, not only in the compiler as the library build has it:
# mixing the two models in one wasm link is not worth the kilobytes it saves.
# -Wnontrivial-memcall fires on the VM's own containers, which move SQObjectPtr as
# raw bytes on purpose; silenced so a warning that does matter is not buried.
add_compile_options(-O2 -fno-rtti -fexceptions -fno-strict-aliasing
                    -Wno-unused-parameter -Wno-missing-field-initializers -Wno-nontrivial-memcall)

add_subdirectory("${QUIRREL}/squirrel" squirrel)
add_subdirectory("${QUIRREL}/squirrel/compiler" quirrel-compiler)
add_subdirectory("${QUIRREL}/sqstdlib" sqstdlib)
add_subdirectory("${QUIRREL}/sqmodules" sqmodules)

# The libraries name their include directories relative to PROJECT_SOURCE_DIR, which
# is this project when they are pulled in from here, so every one of them has to be
# given again. They also keep most of them PRIVATE, which is right for their own
# build and not enough for one target that includes headers from all four.
target_include_directories(squirrel PUBLIC
  "${QUIRREL}/include" "${QUIRREL}/internal" "${QUIRREL}/squirrel")
target_include_directories(quirrel-compiler PUBLIC
  "${QUIRREL}/include" "${QUIRREL}/internal" "${QUIRREL}/helpers"
  "${QUIRREL}/squirrel" "${QUIRREL}/squirrel/compiler")
target_include_directories(sqstdlib PUBLIC
  "${QUIRREL}/include" "${QUIRREL}/internal" "${QUIRREL}/squirrel" "${QUIRREL}")
target_include_directories(sqmodules PUBLIC
  "${QUIRREL}/include" "${QUIRREL}/sqmodules" "${QUIRREL}/sqrat/include")

add_executable(quirrel runner.cpp)
target_link_libraries(quirrel sqmodules sqstdlib quirrel-compiler squirrel)

# The compiler recurses over the AST, so it needs far more stack than the 64K
# emcc gives by default.
# -O2 has to be repeated here: emcc optimizes the generated JS and runs wasm-opt at
# link time, and without it the output is half again as large.
target_link_options(quirrel PRIVATE
  -O2 -fexceptions
  -sMODULARIZE=1 -sEXPORT_NAME=QuirrelVM
  -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=33554432 -sSTACK_SIZE=8388608
  -sEXPORTED_FUNCTIONS=_quirrel_eval,_quirrel_version
  -sEXPORTED_RUNTIME_METHODS=ccall,FS)

set_target_properties(quirrel PROPERTIES
  SUFFIX ".js"
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")
