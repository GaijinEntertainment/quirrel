add_executable(sq sq.cpp)

target_link_libraries(sq squirrel sqstdlib sqmodules quirrel-compiler)
target_include_directories(sq PRIVATE
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/sqmodules>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/sqrat/include>"
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
  )
