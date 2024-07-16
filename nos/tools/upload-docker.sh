set -ex

arch=$1
if [ -z "$arch" ]; then
    echo "Usage: $0 <arch>"
    exit 1
fi

libname=nos

docker tag $libname:$arch netricks/$libname:$arch
docker push netricks/$libname:$arch
