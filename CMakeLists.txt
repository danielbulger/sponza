cmake_minimum_required(VERSION 3.10)
project(Sponza)

set(CMAKE_CXX_STANDARD 17)
set(TARGET_NAME Sponza)

find_package(OpenGL REQUIRED)

add_executable(${TARGET_NAME} src/main.cpp src/mesh.h src/material.h src/loader.h src/loader.cpp)

target_include_directories(${TARGET_NAME} PRIVATE
	src/
	${OPENGL_INCLUDE_DIR}
	${GLFW3_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
)

target_compile_options(${TARGET_NAME} PUBLIC -Wall -Wextra -pedantic -Werror -Wshadow -Wdouble-promotion)

add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
target_link_libraries(${TARGET_NAME} PRIVATE glfw)

add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
target_link_libraries(${TARGET_NAME} PRIVATE glm)
