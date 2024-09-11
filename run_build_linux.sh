#!/bin/bash


cp ./build/Desktop_Qt_6_8_0-Release/appnterm ./linux-pkg/usr/bin/nterm
dpkg-deb --build linux-pkg
sudo dpkg -i linux-pkg.deb