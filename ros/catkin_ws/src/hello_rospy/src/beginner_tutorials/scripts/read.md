尝试了下：

    sudo apt-get install python-rospkg
    pip install python-rospkg

没有效果，确定路径没有问题后，发现是因为装了Anaconda的原因。

    $ conda install setuptools
    $ pip install -U rosdep rosinstall_generator wstool rosinstall six vcstools

-------
ROS主要的编程语言不光是C++，也有Python。这里讲解如何利用Python实现ROS节点的编写。教程参考官网rospy_tutorials。
1. 编写一个简单的发布者和订阅者
1创建工作空间

建立文件夹hello_rospy,再在该目录下建立子目录src，cd到该src目录，运行如下命令创建工作包

catkin_create_pkg beginner_tutorials std_msgs rospy roscpp


cd到src上层目录(实际上世道catkin_ws目录下)，编译并且source

cd ..
catkin_make
. devel/setup.bash


2编写发布者程序和订阅者程序

roscd beginner_tutorials/ 
mkdir scripts
cd scripts


在scripts目录下新建talker.py文件，填写如下内容：



记得更改权限为可执行文件
3. 编译

修改Cmakelist.txt为如下：

cmake_minimum_required(VERSION 2.8.3)
project(beginner_tutorials)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)

catkin_package()


直接在工作空间运行catkin_make即可。
4. 运行

在3个终端中分别执行如下命令

roscore

    1

rosrun beginner_tutorials talker.py

    1

rosrun beginner_tutorials listener.py
