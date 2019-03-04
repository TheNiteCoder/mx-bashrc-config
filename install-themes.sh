#!/bin/bash

#if [ "$UID" != "0" ]; then 
	#echo "Needs to be run as root" && exit -1
#fi

WORKING=$PWD
mkdir -p ~/.config/mx-bashrc-config/themes/ && cp -v $WORKING/themes/* ~/.config/mx-bashrc-config/themes/




