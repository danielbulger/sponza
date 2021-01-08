cmake_minimum_required(VERSION 3.10)
project(Sponza)

set(CMAKE_CXX_STANDARD 17)
set(TARGET_NAME Sponza)

find_package(OpenGL REQUIRED)

add_executable(${TARGET_NAME}
	lib/glad/src/glad.c
	lib/stb/stb_image.h
	src/main.cpp
	src/mesh.h
	src/loader.h
	src/loader.cpp
	src/mesh.cpp
	src/camera.h
	src/camera.cpp
	src/shader.h
	src/skybox.h
	src/skybox.cpp
	src/resource.h
	src/resource.cpp
	src/light.h
	src/hdr.h src/hdr.cpp src/uniforms.h src/uniforms.cpp src/light.cpp)

target_include_directories(${TARGET_NAME} PRIVATE
	src/
	${OPENGL_INCLUDE_DIR}
	${GLFW3_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
	lib/glad/include/
	lib/stb/
)

target_compile_options(${TARGET_NAME} PUBLIC -Wall -Wextra -Werror -Wshadow -Wdouble-promotion -Wno-dev)

add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
target_link_libraries(${TARGET_NAME} PRIVATE glfw)

add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
target_link_libraries(${TARGET_NAME} PRIVATE glm)

target_link_libraries(${TARGET_NAME} PRIVATE ${OPENGL_gl_LIBRARY})
