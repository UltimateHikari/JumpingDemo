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
    main.cpp
    )

add_executable(lab3 ${SRCS})
target_include_directories(lab3 PUBLIC engine)
target_link_libraries(lab3 ${ALL_LIBS} MyEngine)