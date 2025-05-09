#!/bin/bash


cp ./build/build_release/appnterm ./linux-pkg/usr/bin/nterm
dpkg-deb --build linux-pkg
sudo dpkg -i linux-pkg.deb
