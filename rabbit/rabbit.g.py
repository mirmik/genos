import licant

licant.module("rabbit",
              srcdir="rabbit",
              sources=[
                  "inctest.cpp",
                  "intersect.cpp",
              ],
              mdepends=["rabbit.mesh"],
              include_paths=["."]
              )

licant.module("rabbit.opengl",
              sources=[
                  "rabbit/opengl/*.cpp",
                  "rabbit/font/naive.cpp",
              ]
              )

licant.module("rabbit.qt_opengl",
              sources=[
                  "rabbit/opengl/qt_drawer.cpp",
              ]
              )

licant.module("rabbit.mesh",
              sources=[
                  "rabbit/mesh/*.cpp"
              ]
              )

licant.module("rabbit.gazebo", sources=["rabbit/space/gazebo.cpp"])
