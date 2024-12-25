project(final)
file(GLOB source_file "*.cpp")
add_executable(${PROJECT_NAME}
  ${source_file}
)