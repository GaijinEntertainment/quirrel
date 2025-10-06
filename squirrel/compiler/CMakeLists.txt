set(COMPILER_SRC
    ast.cpp
    compilationcontext.cpp
    parser.cpp
    codegen.cpp
    constgen.cpp
    sqio.cpp
    compiler.cpp
    sqfuncstate.cpp
    optimizer.cpp
    lexer.cpp
    optimizations/closureHoisting.cpp
    static_analyzer/analyzer.cpp)

add_library(quirrel-compiler STATIC ${COMPILER_SRC})
add_library(quirrel::compiler ALIAS quirrel-compiler)
set_property(TARGET quirrel-compiler PROPERTY EXPORT_NAME compiler)

target_include_directories(quirrel-compiler PUBLIC
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/internal>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/helpers>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/squirrel>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/squirrel/compiler>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
  )
