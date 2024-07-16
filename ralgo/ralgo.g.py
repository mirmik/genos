import licant

licant.module("ralgo.include",
              include_paths=["."])

licant.module("ralgo.log", "console",
              sources=["ralgo/log/log-console.cpp"],
              default=True
              )

licant.module("ralgo.log", "silent",
              sources=["ralgo/log/log-silent.cpp"]
              )
licant.module("ralgo.log", "stub",
              sources=["ralgo/log/log-silent.cpp"]
              )

licant.module("ralgo.signal",
              sources=["ralgo/signal/fft.cpp"]
              )

licant.module("ralgo.terathon",
              sources=["ralgo/third/terathon/*.cpp"],
              )

licant.module("ralgo",
              sources=[
                    "ralgo/cnc/*.cpp",
                  "ralgo/global_protection.cpp",
                  "ralgo/imu/madgwick.cpp",
                  "ralgo/log/log.cpp",
                  "ralgo/trajectory/tsdeform.c",
                  "ralgo/trajectory/trajectory.cpp",
                  "ralgo/trajectory/linetraj.cpp",
                  "ralgo/robo/stepper_controller.cpp",
                  "ralgo/heimer/axisctr.cpp",
                  "ralgo/heimer/velocity_applier.cpp",
                  "ralgo/heimer/signal_processor.cpp",
                  "ralgo/heimer/signal.cpp",
                  "ralgo/heimer/dof6_signal.cpp",
                  "ralgo/heimer/dof6_controller.cpp",
                  "ralgo/heimer/command.cpp",
                  "ralgo/heimer/scalar_signal.cpp",
                  "ralgo/heimer/axis_stub_processor.cpp",
                  "ralgo/heimer/axstate_pid_processor.cpp",
                  "ralgo/heimer/axstate_signal_processor.cpp",
                  "ralgo/heimer/axstate_linear_processor.cpp",
                  "ralgo/heimer/axstate_sincos_processor.cpp",
                  "ralgo/heimer/axstate_pose3_chain_processor.cpp",
                  "ralgo/heimer/convex_zone_approval.cpp",
                  "ralgo/heimer/executor.cpp",
                  "ralgo/heimer/fast_cycle_device.cpp",
                  "ralgo/heimer/axisapi.cpp",
                  "ralgo/clinalg/matops_square_inverse.cpp",
                  "ralgo/clinalg/solve.cpp",
                  "ralgo/linalg/roots_finder.cpp",
                  "ralgo/lp/gradient.c",
                  "ralgo/lp/point_in_hexagon.c",
                  "ralgo/util/index_brute_force.c",
                  "ralgo/kinematic/kinchain.cpp",
              ],
              include_paths=["."],
              mdepends=[
                  "ralgo.include",
                  "ralgo.log",
                  "ralgo.disctime",
                  "ralgo.signal",
                  "ralgo.oldheimer"
              ],
              )

licant.module("ralgo.rxsignal",
              sources=["ralgo/rxsignal/rxpid.cpp"]
              )

licant.module("ralgo.oldheimer",
              sources=["ralgo/oldheimer/*.cpp", ]
              )

licant.module("ralgo.disctime", "chrono-millis",
              sources=["ralgo/disctime-chrono-millis.cpp"], default=True)

licant.module("ralgo.disctime", "manual",
              sources=[])
