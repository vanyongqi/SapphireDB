#!/bin/bash
machine=`uname -m | grep -o '64'`
if [ $machine == "64"];then
	bitlevel= 64
else
	bitlevel= 32
fi
cd boost/boost_1_78_0/
./bootstrap.sh --prefix =./
./b2 link =static threading = multi variant = release address-model =$bitlevel tooset =gcc runtime-link-static 
