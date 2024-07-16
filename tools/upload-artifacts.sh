set -xe

DIR=$POSITIONER_ARTIFACTS_PATH
date=`date +%Y.%m.%d.%H.%M.%S`

cp artifacts.tar.gz "$DIR/artifacts-$date.tar.gz"
cp artifacts.tar.gz "$DIR/artifacts-latest.tar.gz"
cp artifacts.tar.gz "$POSITIONER_ARTIFACTS_PATH_WORK/artifacts.tar.gz"