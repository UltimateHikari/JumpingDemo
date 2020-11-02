cc_library (
    name = "texture_manager",
    hdrs = ["texture_manager.h",],
    srcs = ["texture_manager.cpp",],
    deps = [":texture",],
    visibility = ["//visibility:public"],
)

cc_library (
    name = "texture",
    hdrs = ["texture.h",],
    srcs = ["texture.cpp",],
    deps = [
        "@SDL2//:SDL2",
        "@SDL2//:SDL2image",
    ],
)