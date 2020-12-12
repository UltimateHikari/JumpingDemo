set(ENGINE_SRCS
    shader.cpp
    controls.cpp
    model.cpp
    window.cpp
    stb_image.cpp
    light.cpp
    rmanager.cpp
    physics.cpp
    entity.cpp
    game.cpp
    )

add_library(MyEngine ${ENGINE_SRCS})
target_link_libraries(MyEngine assimp)