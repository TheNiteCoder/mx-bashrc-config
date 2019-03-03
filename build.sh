#!/bin/sh

qmake -o Makefile application/mx-bashrc-config.pro
make
rm .qmake.stash moc_window.cpp moc_window.o window.o Makefile qrc_resource.cpp qrc_resource.o ui_window.h main.o 
