#!/bin/sh
mkdir -p /root/catkin_ws/src
cd /root/catkin_ws/src;catkin_create_pkg voice_system roscpp rospy std_msgs
cd /root/catkin_ws;catkin_make
#拷贝代码 src include launch libs(libmsc.so)
#手动修改cmakelist.txt /root/catkin_ws/src/voice_system
#195 add_executable(xf_tts_node src/xf_tts.cpp)
#196 target_link_libraries(xf_tts_node ${catkin_LIBRARIES} -lmsc -lrt -ldl -lpthread)
#197 
#198 add_executable(tuling_nlu_node src/tuling_nlu.cpp)
#199 target_link_libraries(tuling_nlu_node ${catkin_LIBRARIES} -ljsoncpp -lcurl -lsqlite3)
#200 
#201 add_executable(xf_asr_node src/xf_asr.cpp src/linuxrec.cpp src/speech_recognizer.cpp)
#202 target_link_libraries(xf_asr_node ${catkin_LIBRARIES} -lmsc -lrt -ldl -lpthread -lasound)
#203 
#204 add_executable(vs_start_node src/vs_start.cpp)
#205 target_link_libraries(vs_start_node ${catkin_LIBRARIES} )


#source devel/setup.sh
#catkin_make 
