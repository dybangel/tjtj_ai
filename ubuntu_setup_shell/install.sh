#!/bin/sh
aptpack="openssh-server lrzsz sox vim vim-scripts vim-doc ctags git python-pip swig byacc"

for i in $aptpack
do
	#have=`dpkg -l $i|wc -l`
	#if [ $have -eq 0 ];then
		apt install $i -y
	#else
	#	echo "$i already install don't install"
	#fi

done

cp ./.vimrc ~/
