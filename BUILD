cc_library(
  name = "piece",
  srcs = ["piece.h"],
)

cc_library(
  name = "position",
  hdrs = ["position.h"],
  srcs = ["position.cc"],
  deps = [
    ":piece",
  ],
)

cc_test(
  name = "position_test",
  srcs = ["position_test.cc"],
  deps = [
    ":position",
    "@com_google_googletest//:gtest_main",
  ]
)

cc_library(
  name = "move",
  hdrs = ["move.h"],
  srcs = ["move.cc"],
  deps = [
    ":position",
    "@com_google_absl//absl/strings:str_format",
  ]
)

cc_test(
  name = "move_test",
  srcs = ["move_test.cc"],
  deps = [
    ":move",
    "@com_google_googletest//:gtest_main",
  ]
)

cc_library(
  name = "game_engine",
  hdrs = ["game_engine.h"],
  srcs = ["game_engine.cc"],
  deps = [
    ":move",
    ":position",
  ]
)

cc_test(
  name = "game_engine_test",
  srcs = ["game_engine_test.cc"],
  deps = [
    ":game_engine",
    "@com_google_googletest//:gtest_main",
  ]
)

cc_library(
  name = "notation_parser",
  hdrs = ["notation_parser.h"],
  srcs = ["notation_parser.cc"],
  deps = [
    ":move",
    ":position",
    "@com_google_absl//absl/status:status",
    "@com_google_absl//absl/status:statusor",
    "@com_google_absl//absl/strings:str_format",
  ]
)
