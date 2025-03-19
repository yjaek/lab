package(default_visibility = ["//visibility:public"])

cc_library(
    name = "map-order-book",
    hdrs = ["MapOrderBook.h"],
    deps = ["order"]
)

cc_library(
    name = "order",
    hdrs = ["Order.h"],
    deps = [
        "side",
        "types"
    ]
)

cc_library(
    name = "side",
    hdrs = ["Side.h"]
)

cc_library(
    name = "spsc-queue",
    hdrs = ["SPSCQueue.h"]
)

cc_library(
    name = "vector-order-book",
    hdrs = ["VectorOrderBook.h"],
    deps = ["order"]
)

cc_library(
    name = "types",
    hdrs = ["types.h"]
)
