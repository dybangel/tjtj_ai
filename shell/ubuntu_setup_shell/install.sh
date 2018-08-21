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

##设置无线网卡启动和静态IP
#nano /etc/network/interfaces
##增加下面代码 第一部分是给rj45口设置静态ip 第二部分是给usb无线网卡设置静态ip,无线设置前用iwlist 【dev】 scan看看能不能收到网络
#auto enp2s0
#iface enp2s0 inet static
#address 8.8.8.8
#netmask 255.255.255.0
#gateway 8.8.8.8

#auto enx0013ef3009d4
#allow-hotplug enx0013ef3009d4
#iface enx0013ef3009d4 inet static
#wpa-ssid "treebear"
#wpa-psk "rongwei7501"
#address 1.1.1.1
#netmask 255.255.255.0
#gateway 192.168.123.1                          


#########sqlliite
#wget http://www.sqlite.org/2014/sqlite-autoconf-3080701.tar.gz
#./configure
#make; make install
##编写代码后下面编译
#$gcc test.c -l sqlite3
