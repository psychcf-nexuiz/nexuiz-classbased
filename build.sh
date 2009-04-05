#!/bin/sh

echo "Building Nexuiz..."

echo "*** MENU ***"
cd ./data/qcsrc/menu
~/.nexuiz/Nexuiz_2.5/fteqcc/fteqcc-linux-x86_64
#fteqcc

echo "*** CLIENT ***"
cd ../client
~/.nexuiz/Nexuiz_2.5/fteqcc/fteqcc-linux-x86_64
#fteqcc

echo "*** SERVER ***"
cd ../server
~/.nexuiz/Nexuiz_2.5/fteqcc/fteqcc-linux-x86_64
#fteqcc

echo "*** Making pk3 ***"
cd ../../

zip -r ../data-svn * -u -1 -x .git #-i *.dat *.cfg *.lno
cp ../data-svn.zip ~/.nexuiz/data/data-svn.pk3
