cc_binary (
    name = "main",
    srcs = ["main.cpp",],
    deps = [
        ":game",
    ],
    copts = ["/D", "_UNICODE", "/D", "UNICODE"],
    linkopts = [
        "-SUBSYSTEM:WINDOWS",
        "-DEFAULTLIB:Winmm",
        "-DEFAULTLIB:User32",
    ],
    nocopts = "/J"
)

cc_library (
    name = "game",
    hdrs = ["game.h"],
    srcs = ["game.cpp"],
    deps = [
        "//game:game_lib",
    ],
)

cc_library (
    name = "game_lib",
    hdrs = glob(["game_library/**/*.h"]),
    srcs = glob(["game_library/**/*.cpp"]),
    deps = [
        "//framework:framework",
    ],
    visibility = ["//visibility:public"]
)