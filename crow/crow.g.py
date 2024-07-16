import licant
import licant.modules

DEFINES = []
licant.cli.add_argument("-o", "--oldheader",
                        action="store_true", default=False)
opts, args = licant.cli.parse()
DEFINES = ["OLD_HEADER=1"] if opts.oldheader else []

licant.module("crow.select", "impl",
              sources=["crow/src/asyncio.cpp"],
              defines=["CROW_USE_ASYNCIO=1"],
              default=True
              )

licant.module("crow.select", "stub",
              defines=[]
              )

licant.module("crow.diagnostic", "nos",
              sources=["crow/src/variants/crow_print.cpp"],
              default=True
              )

licant.module("crow.threads", "linux", default=True,
              sources=["crow/src/threads-posix.cpp"]
              )

licant.modules.module("crow.minimal",
                      srcdir="crow",
                      sources=[
                          "src/packet.cpp",
                          "src/variants/warn.cpp",
                          "src/packet_ptr.cpp",
                          "src/tower.cpp",
                          "src/gateway.cpp",
                          "src/functional.cpp",
                          "src/hexer.cpp",
                          "src/address.cpp",
                          "src/hostaddr_view.cpp",
                          "src/hostaddr.cpp",
                          "proto/node.cpp",
                          "proto/node_protocol.cpp",
                          "proto/keepalive.cpp",
                          "proto/node-sync.cpp",
                          "proto/channel.cpp",
                          "nodes/subscriber_node.cpp",
                          "nodes/publisher_node.cpp",
                          "nodes/service_node.cpp",
                          "nodes/requestor_node.cpp",
                      ],
                      mdepends=["crow.include", "crow.diagnostic"]
                      )

licant.modules.module("crow",
                      srcdir="crow",
                      sources=[
                          "src/iter.cpp",
                          "proto/acceptor.cpp",
                          # "proto/socket.cpp",
                          "proto/msgbox.cpp",
                          "proto/channel-sync.cpp",
                          # "proto/rpc.cpp",
                          # "addons/noslogger.cpp",
                          "nodes/spammer.cpp",
                      ],

                      mdepends=[
                          "crow.allocator",
                          "crow.threads",
                          "crow.select",
                          "crow.crowker",
                          "crow.minimal",
                          "crow.protocol.pubsub",
                      ],

                      defines=[] + DEFINES
                      )

licant.module("crow.crowker",
              srcdir="crow",
              sources=[
                  "brocker/crowker.cpp",
                  "brocker/theme.cpp",
                  "brocker/client.cpp",
                  "brocker/crow_client.cpp",
                  "brocker/tcp_client.cpp",
                  "brocker/crowker_api.cpp",
                  "brocker/crowker_pubsub_node.cpp",
              ]
              )

licant.modules.module("crow.diagnostic", "debug",
                      sources=["crow/src/variants/print-debug.cpp"]
                      )

licant.modules.module("crow.diagnostic", "stub",
                      sources=["crow/src/variants/print-stub.cpp"]
                      )

licant.modules.module("crow.include",
                      include_paths=["."])

licant.modules.module("crow.allocator", "malloc",
                      sources=["crow/src/variants/allocation_malloc.cpp"], default=True)

licant.modules.module("crow.allocator", "pool",
                      sources=["crow/src/variants/allocation_pool.cpp"])

#######################################GATES#########################################

licant.modules.module("crow.udpgate",
                      sources=["crow/gates/udpgate.cpp"])

licant.modules.module("crow.tcpgate",
                      sources=["crow/gates/tcpgate.cpp"])

licant.modules.module("crow.serial_gstuff",
                      sources=[
                          "crow/gates/serial_gstuff.cpp"
                      ],
                      mdepends=[
                          # "igris.protocols.gstuff"
                      ])

licant.module("crow.protocol.pubsub", "impl", default=True,
              sources=[
                  "crow/pubsub/pubsub.cpp",
                  "crow/pubsub/crowker_support.cpp",
              ],
              defines=["CROW_PUBSUB_PROTOCOL_SUPPORTED=1"]
              )

licant.module("crow.realtime_threads",
              defines=["CROW_REALTIME_THREADS=1"]
              )
