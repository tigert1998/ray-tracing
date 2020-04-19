cc_library(
    name = "textures",
    srcs = [
        "checker_texture.cc",
        "constant_texture.cc",
        "noise_texture.cc"
    ],
    hdrs = [
        "checker_texture.h",
        "constant_texture.h",
        "noise_texture.h",
        "texture.h",
        "texture"
    ],
    deps = [
        "@glm//:glm",
        "//rt:perlin_noise"
    ],
    visibility = ["//visibility:public"],
)