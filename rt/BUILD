cc_library(
    name = "perlin_noise",
    srcs = ["perlin_noise.cc"],
    hdrs = ["perlin_noise.h"],
    deps = [
        "@glm//:glm"
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "bump",
    srcs = ["bump.cc"],
    hdrs = ["bump.h"],
    deps = [
        "@glm//:glm",
        ":hitable",
        ":perlin_noise"
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "diffuse_light",
    srcs = ["diffuse_light.cc"],
    hdrs = ["diffuse_light.h"],
    deps = [
        "@glm//:glm",
        ":hitable",
        "//rt/textures:textures"
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "lambertian",
    srcs = ["lambertian.cc"],
    hdrs = ["lambertian.h"],
    deps = [
        "@glm//:glm",
        ":hitable",
        "//rt/textures:textures"
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "metal",
    srcs = ["metal.cc"],
    hdrs = ["metal.h"],
    deps = [
        "@glm//:glm",
        ":hitable",
        "//rt/textures:textures"
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "dielectric",
    srcs = ["dielectric.cc"],
    hdrs = ["dielectric.h"],
    deps = [
        "@glm//:glm",
        ":hitable",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "parallelepiped",
    srcs = ["parallelepiped.cc"],
    hdrs = ["parallelepiped.h"],
    deps = [
        ":hitable",
        ":parallelogram"
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "parallelogram",
    srcs = ["parallelogram.cc"],
    hdrs = ["parallelogram.h"],
    deps = [
        ":hitable",
        ":triangle"
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "triangle",
    srcs = ["triangle.cc"],
    hdrs = ["triangle.h"],
    deps = [
        ":hitable",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "sphere",
    srcs = ["sphere.cc"],
    hdrs = ["sphere.h"],
    deps = [
        ":hitable",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "hitable",
    srcs = [
        "hitable_list.cc",
        "material.cc",
        "ray.cc"
    ],
    hdrs = [
        "hitable.h",
        "hitable_list.h",
        "material.h",
        "ray.h"
    ],
    deps = [
        "@glm//:glm",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "camera",
    srcs = ["camera.cc"],
    hdrs = ["camera.h"],
    deps = [
        "@glm//:glm",
        ":hitable"
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "main",
    srcs = [
        "main.cc",
    ],
    copts = [],
    deps = [
        "//rt/textures:textures",
        ":bump",
        ":dielectric",
        ":hitable",
        ":camera",
        ":diffuse_light",
        ":lambertian",
        ":metal",
        ":parallelepiped",
        ":sphere"
    ],
    linkopts = ["-lpthread"],
)