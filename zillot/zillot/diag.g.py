import licant

licant.module("zillot.diag", "stub", sources = ["diag_stub.c"])
licant.module("zillot.diag", "impl", sources = ["diag_impl.c"], default=True)
