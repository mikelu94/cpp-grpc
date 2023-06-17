package(default_visibility = ["//visibility:public"])

load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

proto_library(
    name = "item_proto",
    srcs = ["item.proto"],
)

cc_proto_library(
    name = "item_cc_proto",
    deps = [":item_proto"],
)

cc_grpc_library(
    name = "item_cc_grpc",
    srcs = [":item_proto"],
    grpc_only = True,
    deps = [":item_cc_proto"],
)

cc_binary(
    name = "client",
    srcs = ["client/app.cc"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":item_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
)