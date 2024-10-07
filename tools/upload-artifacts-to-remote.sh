set -ex

./tools/collect-artifacts-auto.sh
scp ./artifacts.tar.gz rfmeas@$1:/home/rfmeas/artifacts.tar.gz
scp ./tools/expand.sh rfmeas@$1:/home/rfmeas/expand.sh

# execute expand.sh on remote host from sudo user
ssh rfmeas@$1 "echo rfmeas | sudo -S systemctl stop rfdaemon.service"
ssh rfmeas@$1 "echo rfmeas | sudo -S bash /home/rfmeas/expand.sh"
ssh rfmeas@$1 "echo rfmeas | sudo -S systemctl daemon-reload"
ssh rfmeas@$1 "echo rfmeas | sudo -S systemctl start rfdaemon.service"
