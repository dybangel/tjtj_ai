#!/bin/sh
localpath=`pwd`
#echo $localpath
#exit;
mkdir -p /root/catkin_ws/src
cd /root/catkin_ws/src;catkin_create_pkg voice_system roscpp rospy std_msgs
cd /root/catkin_ws;catkin_make
cd $localpath
cp -ravf src /root/catkin_ws/src/voice_system
cp -ravf include/* /root/catkin_ws/src/voice_system/include/
cp -ravf launch /root/catkin_ws/src/voice_system
cp -ravf libs/x86/libmsc.so /usr/lib
cp CMakeLists.txt /root/catkin_ws/src/voice_system
cd /root/catkin_ws;source devel/setup.sh
cd /root/catking_ws;catkin_make
#拷贝代码 src include launch libs(libmsc.so)
#手动修改cmakelist.txt
#source devel/setup.sh
#catkin_make 
