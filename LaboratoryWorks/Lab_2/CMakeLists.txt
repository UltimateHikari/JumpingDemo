cmake_minimum_required(VERSION 3.0.0)
set(PROJECT_NAME lab1)
project(${PROJECT_NAME} VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(PkgConfig REQUIRED)
#find_package(SDL2 REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)


include_directories(src graphics)
set(SRCS main/main.cpp graphics/commands.cpp main/element.cpp graphics/visualizer.cpp graphics/window.cpp)

add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PUBLIC ${SDL2_CFLAGS_OTHER})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
