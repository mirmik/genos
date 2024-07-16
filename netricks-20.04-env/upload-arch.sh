set -ex

arch=$1
if [ -z "$arch" ]; then
    echo "Usage: $0 <arch>"
    exit 1
fi

docker tag netricks-20.04-env:$arch netricks/netricks-20.04-env:$arch
docker push netricks/netricks-20.04-env:$arch

