set -ex

arch=$1
if [ -z "$arch" ]; then
    echo "Usage: $0 <arch>"
    exit 1
fi

libname=nos

docker build . -t $libname:$arch -f tools/dockerfile.arch --build-arg ARCH=$arch
mkdir -p dist/$arch
docker run --rm --entrypoint cat $libname:$arch /root/$libname/lib$libname.so > dist/$arch/lib$libname.so
docker run --rm --entrypoint cat $libname:$arch /root/$libname/lib$libname.a > dist/$arch/lib$libname.a
