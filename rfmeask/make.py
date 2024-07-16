#!/usr/bin/env python3

import licant
import licant.install
import os
import subprocess
import git

repo = git.Repo(os.getcwd())
repo.head.object.hexsha
last_commit_datetime = repo.head.object.authored_datetime

mitsubishi_board = False
correction_table = False
modbus_enabled = False

#############################
# Deprecated params
lazer = False
lazer_emulation = False
netan = False
#############################

extern_libs = [
    'stdc++fs'
]

use_submodules = False
licant.include("libscpi", "lib/libscpi/libscpi.g.py")


def ircc(tgt, src):
    def self_need():
        with subprocess.Popen(f"ircc {src} -o {tgt} --is-rebuild-needed",
                              shell=True,
                              stdout=subprocess.PIPE) as proc:
            ans = str(proc.communicate()[0].decode("utf-8")).strip()
            if ans == "yes":
                return True
            else:
                return False

    src = os.path.expanduser(src)
    tgt = os.path.expanduser(tgt)
    licant.source(src)
    licant.default_core().add(
        licant.make.FileTarget(
            tgt=tgt,
            build=licant.make.Executor("ircc {src} -o {tgt}"),
            src=src,
            deps=[src],
            message="IRCC {src} {tgt}",
            self_need=self_need
        )
    )
    return tgt


ircc("./build/ircc_resource.gen.cpp", "resources.txt")

SOURCES = """
		build/ircc_resource.gen.cpp
		src/*.cpp
		src/initialization/*.cpp
		src/ndmath/*.cpp
		src/utilxx/*.cpp
		src/cankollm/*.cpp
		src/devices/*.cpp
		src/groups/*.cpp
		src/servos/*.cpp
		src/comm/*.cpp
		src/moveapi/*.cpp
		src/nos-shell/*.cpp
		""".split()
SOURCES = [s.strip() for s in SOURCES]

if modbus_enabled:
    SOURCES.extend("""
		src/MitsuModbusServo.cpp
	""")

if lazer or lazer_emulation:
    SOURCES.extend("""
		src/deprecated/lazerproxy.cpp
	""".split())
    # deprecated/dataproxy.cpp

if mitsubishi_board:
    SOURCES.extend("""
		src/MitsubishiBoard.cpp
	""".split())

defopts = ["-DSCPI_USER_CONFIG",
           f"-DLAST_COMMIT_DATETIME='{last_commit_datetime}'"]

opt_table = {
    "HAVE_CORRECTION_TABLE": correction_table,
    "HAVE_NETWORK_ANALYZER_PROXY": netan,
    "HAVE_LAZER_PROXY": lazer,
    "LAZER_EMULATION": lazer_emulation,
    "HAVE_MITSUBISHI_BOARD": mitsubishi_board,
    "MODBUS_ENABLED": modbus_enabled
}

if mitsubishi_board:
    extern_libs.append("mc2xx")

for k, v in opt_table.items():
    defopts.append("-D{}={}".format(k, 1 if v else 0))

licant.makedir("build/proto")


licant.cxx_application("rfmeas",
                       sources=SOURCES + [
                           "src/app/main.cpp",
                           "src/app/doctest_support.cpp"],
                       include_paths=[
                           "src",
                           "src/deprecated",
                           "build",
                           "tests",
                       ],
                       mdepends=[
                           "libscpi",
                       ],
                       cxxstd="c++20",
                       cxx_flags=" ".join(defopts) +
                       "-O0 -Werror=all -pedantic -fPIE -fPIC -g3  -fmax-errors=10",
                       cc_flags="-O0 -DSCPI_USER_CONFIG -Werror=all -fPIE -fPIC -g3 -fmax-errors=10",
                       # -s mc2xxstd_x64.lib mc2xxstd_x64.dll",
                       ld_flags=" -fPIC -fPIE -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -g3",
                       libs=["igris", "nos", "crow",
                             "ralgo", "rabbit"] + extern_libs,
                       defines=["RABBIT_REAL_TYPE=double"],
                       )

licant.cxx_application("runtests",
                       sources=SOURCES + [
                           "tests/*.cpp",
                           "tests/nd/*.cpp",
                       ],
                       include_paths=[
                           "src",
                           "tests",
                           "src/deprecated",
                           "build",
                       ],
                       mdepends=[
                           "libscpi",
                       ],
                       cxxstd="c++20",
                       cxx_flags=" ".join(defopts) +
                       "-O0 -Werror=all -pedantic -g3 -fPIC -fPIE",
                       cc_flags="-O0 -DSCPI_USER_CONFIG -Werror=all -fPIC -fPIE -g3",
                       # -s mc2xxstd_x64.lib mc2xxstd_x64.dll",
                       ld_flags=" -fPIC -fPIE -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -g3",
                       libs=["igris", "nos", "crow",
                             "ralgo", "rabbit", "pthread"] + extern_libs,
                       defines=["RABBIT_REAL_TYPE=double"],

                       )


@licant.routine(deps=["rfmeas"])
def rfmeas_setcap():
    os.system("sudo setcap 'cap_sys_nice=eip' rfmeas")


@licant.routine(deps=["rfmeas"])
def upload_remote(ip):
    licant.system(f"scp rfmeas {ip}:/home/rfmeas/rfmeas")


@licant.routine
def start_remote(ip):
    print("start_remote")
    licant.system(f"scp rfmeas rfmeas@{ip}:/home/rfmeas/rfmeas")
    licant.system(
        f"ssh rfmeas@{ip} 'echo rfmeas | sudo -S /home/rfmeas/rfmeas --config /home/rfmeas/project'")


@licant.routine(deps=["rfmeas"])
def remote_gdbserver(ip):
    licant.system(f"scp rfmeas rfmeas@{ip}:/home/rfmeas/rfmeas")
    licant.system(
        f"ssh rfmeas@{ip} 'echo rfmeas | sudo -S gdbserver localhost:2000 /home/rfmeas/rfmeas --config /home/rfmeas/project'")


licant.install.install_application(tgt="install", src="rfmeas", dst="rfmeas")
licant.fileset("all", targets=["rfmeas", "runtests"])

licant.ex(default="all")
