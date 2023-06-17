package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "client",
    srcs = ["app.cc"],
    defines = ["BAZEL_BUILD"],
    deps = [
        "//proto:item_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
)
