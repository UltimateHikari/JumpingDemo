set(ENGINE_SRCS
    shader.cpp
    controls.cpp
    objloader.cpp
    vboindexer.cpp
    stb_image.cpp
    )

add_library(MyEngine ${ENGINE_SRCS})
target_link_libraries(MyEngine assimp)