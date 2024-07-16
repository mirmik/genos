set -ex

arch=$1
if [ -z "$arch" ]; then
    echo "Usage: $0 <arch>"
    exit 1
fi

arches=(amd64 arm32v7 arm64v8)
libdirs=(x86_64-linux-gnu arm-linux-gnueabihf aarch64-linux-gnu)

# if arch in list, get libdir. Else exit.
founded=0
for i in "${!arches[@]}"; do
    if [ "$arch" = "${arches[$i]}" ]; then
        libdir=${libdirs[$i]}
        founded=1
        break
    fi
done

if [ $founded -eq 0 ]; then
    echo "Architecture $arch is not supported"
    exit 1
fi

libc="libc.so.6"
libstdcpp="libstdc++.so.6"
docker build . -t netricks-20.04-env:$arch -f Dockerfile --build-arg ARCH=$arch
docker tag netricks-20.04-env:$arch netricks/netricks-20.04-env:$arch

mkdir -p dist/$arch
docker cp $(docker create netricks-20.04-env:$arch):/usr/lib/$libdir/$libc dist/$arch/
docker cp $(docker create netricks-20.04-env:$arch):/usr/lib/$libdir/$libstdcpp dist/$arch/
