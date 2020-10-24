cmake_minimum_required (VERSION 3.0)
project (Lab3)

find_package(OpenGL REQUIRED)

add_subdirectory(engine)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW
)

set(SRCS
    src/main.cpp
    engine/shader.hpp
    )

set(SHADERS
    shaders/FShader.fragmentshader
    shaders/VShader.vertexshader
    )

add_executable(lab3 ${SRCS} ${SHADERS})
target_link_libraries(lab3 ${ALL_LIBS} MyEngine)