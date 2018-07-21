#!/bin/sh
aptpack="openssh-server lrzsz sox vim vim-scripts vim-doc ctags git python-pip swig byacc vnc4server libasound2-dev"

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
#允许root ssh登录
#nano /etc/ssh/sshd_config
#在Authentication部分，注释掉“PermitRootLogin without-password”
#在Authentication部分，添加“PermitRootLogin yes”

#关掉自动登录
# vi /etc/lightdm/lightdm.conf 去掉自动登录 #autologin-user=dyb 
