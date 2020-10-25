cmake_minimum_required (VERSION 3.0)
project (Lab3)

find_package(OpenGL REQUIRED)

add_subdirectory(engine)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW
    assimp
)

set(SRCS
    src/main.cpp
    engine/shader.hpp
    src/window.cpp
    src/model.cpp
    )

set(SHADERS
    shaders/FShader.fragmentshader
    shaders/VShader.vertexshader
    )

add_executable(lab3 ${SRCS} ${SHADERS})
target_link_libraries(lab3 ${ALL_LIBS} MyEngine)