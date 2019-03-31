from licant.modules import implementation

module("gxx.diag", "stub", sources = ["diag_stub.c"])
module("gxx.diag", "impl", sources = ["diag_impl.c"], default=True)
