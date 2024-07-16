#!/bin/bash

mkdir -p /etc/rfdaemon/
if [[ ! -f /etc/rfdaemon/applications.json ]]
then
	cp ../configs/empty-applications.json /etc/rfdaemon/applications.json
fi
cp rfdaemon.service /etc/systemd/system/rfdaemon.service
systemctl daemon-reload
