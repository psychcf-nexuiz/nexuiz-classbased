#!/bin/sh

echo "Building Nexuiz..."

echo "*** MENU ***"
cd ./data/qcsrc/menu
#~/.nexuiz/Nexuiz/sources/fteqcc/fteqcc-linux-x86_64
fteqcc

echo "*** CLIENT ***"
cd ../client
#~/.nexuiz/Nexuiz/sources/fteqcc/fteqcc-linux-x86_64
fteqcc

echo "*** SERVER ***"
cd ../server
#~/.nexuiz/Nexuiz/sources/fteqcc/fteqcc-linux-x86_64
fteqcc

echo "*** Making pk3 ***"
cd ../../
zip -r data-svn * -u -1 -x *.svn-base #-i *.dat *.cfg *.lno
cp data-svn.zip ~/.nexuiz/data/data-svn.pk3
