set -ex
mkdir -p mnt
sudo sshfs -o allow_other rfmeas@$1:/ mnt