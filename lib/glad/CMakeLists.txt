add_library(glad src/gl.c)

target_include_directories(glad PUBLIC
                           "${PROJECT_SOURCE_DIR}/lib/glad/include"
                           )