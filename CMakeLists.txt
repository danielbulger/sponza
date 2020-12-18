cmake_minimum_required(VERSION 3.10)
project(Sponza)

set(CMAKE_CXX_STANDARD 17)
set(TARGET_NAME Sponza)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${TARGET_NAME} src/main.cpp)

target_include_directories(${TARGET_NAME} PUBLIC ${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR})

target_link_libraries(${TARGET_NAME} ${OPENGL_gl_LIBRARY} ${GLFW3_LIBRARY})

target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
