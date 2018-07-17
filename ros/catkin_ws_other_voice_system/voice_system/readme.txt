voice_system 软件包 存放位置限制：<你的 ROS 工作目录>/src/voice_system ，因为有些音频文件的存放路径是相对目录

环境配置：
1)添加讯飞库文件
从讯飞下载的SDK包中的 libmsc.so 库文件的目录是 libs/x64/libmsc.so 若要使用自己的库文件，需要修改全部代码的讯飞id
注意！ 每账号的库文件是不一样的，所以若要使用 voice_system 软件包，需先拷贝本项目的库文件
进入目录： <你的 ROS 工作目录>/src/voice_system/libs
sudo cp libmsc.so /usr/lib

注意：
科大讯飞语音识别 SDK 向树莓派移植的问题
sudo file /bin/ls 
ls: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-armhf.so.3, for GNU/Linux 2.6.32, BuildID[sha1]=5e052e8de057d379ab51d4af510ad9318fe77b46, stripped

由上可得树莓派的架构是 ARM 32-bit 如果要移植需要另外的 libmsc.so
下载地址：http://download.csdn.net/detail/yanghuan313/9616763?web=web


2）安装 sox 或者 mplayer 软件，就可以直接在命令行播放文件： 
sudo apt-get install mplayer(推荐)

sudo apt-get install sox
sudo apt-get install sox libsox-fmt-all


3）添加 c++ 的json，post 库文件：
sudo apt install libcurl3 libcurl4-openssl-dev
sudo apt install libjsoncpp0 libjsoncpp-dev

4）声卡
sudo apt install libasound2-dev

5）检查 /voice_system/CmakeList.txt 文件
-添加头文件目录，找到 include_directories 作如下修改
include_directories(
  ${catkin_INCLUDE_DIRS}
  include
)

-添加语音的所有节点，在文件最底下添加如下
add_executable(xf_tts_node src/xf_tts.cpp)
target_link_libraries(xf_tts_node ${catkin_LIBRARIES} -lmsc -lrt -ldl -lpthread)

add_executable(tuling_nlu_node src/tuling_nlu.cpp)
target_link_libraries(tuling_nlu_node ${catkin_LIBRARIES} -ljsoncpp -lcurl)

add_executable(xf_asr_node src/xf_asr.cpp src/linuxrec.cpp src/speech_recognizer.cpp)
target_link_libraries(xf_asr_node ${catkin_LIBRARIES} -lmsc -lrt -ldl -lpthread -lasound)

add_executable(vs_start_node src/vs_start.cpp)
target_link_libraries(vs_start_node ${catkin_LIBRARIES} )



话题节点看图 1.1
rostopic list -v

Published topics:
 * /voice/tuling_nlu_topic [std_msgs/String] 1 publisher
 * /voice/xf_asr_topic [std_msgs/Int32] 1 publisher
 * /rosout [rosgraph_msgs/Log] 4 publishers
 * /voice/xf_tts_topic [std_msgs/String] 1 publisher
 * /rosout_agg [rosgraph_msgs/Log] 1 publisher

Subscribed topics:
 * /voice/xf_asr_topic [std_msgs/Int32] 1 subscriber
 * /rosout [rosgraph_msgs/Log] 1 subscriber
 * /voice/xf_tts_topic [std_msgs/String] 1 subscriber
 * /voice/tuling_nlu_topic [std_msgs/String] 1 subscriber
 * /voice/vs_start_topic [std_msgs/Int32] 1 subscriber




执行命令：1 2 3 4 5 6 表示用新的终端打开，这样打开方便看调试信息
1.开启 ROS 主机
catkin_make
roscore

2.开启 xf_tts_node 节点，语音合成
rosrun voice_system xf_tts_node 

3.开启 tuling_nlu_node 节点，语音理解
rosrun voice_system tuling_nlu_node

4.开启 xf_asr_node 节点，语音识别，单句麦克风连续识别
rosrun voice_system xf_asr_node

5.开启控制节点：vs_start_node 
rosrun voice_system vs_start_node

！如果只是测试的话可以不用开启这个节点，只需像第6步 rostopic pub -1 /voice/xf_asr_topic std_msgs/Int32 1


**6.开启系统的话题为：/voice/vs_start_topic ，具体看 vs_start.cpp 的注释
开启单次交流：用于单次按键开启
rostopic pub -1 /voice/vs_start_topic std_msgs/Int32 1        --  1 表示单次交流


rostopic pub -1 /voice/vs_start_topic std_msgs/Int32 2        --  2 表示有限次数，按一定等待时间，多次交流（这是方案一如果弄成多次循环也可以来实现多次交流，但交流间隔时间，和交流次数需按具体情况设置，后面还需与人脸结合完成完美的多次交流。例如 tts 节点完成后判断人是否还再机器人周围，如果人在，再次调用单次交流，这样就可以衔接不间断交流；如果人不在，让控制节点返回等待状态（待机））


rostopic pub -1 /voice/vs_start_topic std_msgs/Int32 3         -- 3 表示停止多次识别，让控制节点返回等待状态（待机）









