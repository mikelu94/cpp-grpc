package(default_visibility = ["//visibility:public"])

cc_library(
    name = "service_impl",
    srcs = [
        "service_impl.cc",
        "service_impl.h",
    ],
    deps = [
        "//protos:item_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ]
)

cc_binary(
    name = "server",
    srcs = ["app.cc"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":service_impl",
        "//protos:item_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
)
