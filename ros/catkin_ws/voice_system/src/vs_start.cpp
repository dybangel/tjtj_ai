#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <sstream>


/*
用于控制是语音系统的参数 Flag
Flag = 0 为初始化值，表示等待判断，等待的时间频率与 ros::Rate loop_rate(1)； 有关
Flag = 1 表示只进行 1 次识别
Flag = 2 表示进行多次识别（方案一）
Flag = 3 表示重置 Flag = 0 ，用于当 Flag = 2 进行多次识别时，但要强制返回等待判断
*/
int Flag = 0;		//初始化 Flag


/*
回调函数 startCallback(const std_msgs::Int32::ConstPtr& in_msg)
触发条件：当接收到该节点 vs_start_node 订阅的话题 /voice/vs_start_topic 发布的消息时便会执行一次
功能：根据话题发布的消息内容 in_msg->data 修改 Flag 值，消息类型：std_msgs/Int32

*/
void startCallback(const std_msgs::Int32::ConstPtr& in_msg)
{
	printf("--Use startCallback to change Flag = %d \n",Flag);

	if(in_msg->data == 1)
	{
		Flag = 1;
	}
	if(in_msg->data == 2)
	{
		Flag = 2;
	}
	if(in_msg->data == 3)
	{
		Flag = 3;
	}
}


//主函数
int main(int argc,char* argv[])
{
	int count = 0;		//toshow 计时器的调试参数 
	int i = 0;		//临时计算次数的参数
	int rec_times = 0;	//判断多次识别的次数，用于控制连续识别的次数并重置 Flag 的判断参数

	std_msgs::Int32 out_msg;	//向下一个节点 xf_asr_node 发布的消息 out_msg
	out_msg.data = 1;		//固定值，表示开启语音识别，不用进行更改
	
	//ROS 创建节点以及订阅话题
	ros::init(argc,argv,"vs_start_node");		//创建节点 vs_start_node
	ros::NodeHandle nd;				//节点对象 nd
	
	//订阅话题 /voice/vs_start_topic ，消息队列 1个缓冲队列，回调函数 startCallback
	ros::Subscriber sub = nd.subscribe("/voice/vs_start_topic", 1 , startCallback);

	//发布消息给下一个话题 /voice/xf_asr_topic ，消息队列 5个缓冲队列，消息类型：std_msgs/Int32
	ros::Publisher pub = nd.advertise<std_msgs::Int32>("/voice/xf_asr_topic", 5);

	/*
	ros::Rate对象可以允许你指定自循环的频率。
	它会追踪记录自上一次调用Rate::sleep()后时间的流逝，并休眠直到一个频率周期的时间。*/
	ros::Rate loop_rate(1);		//设置检查频率为10hz（1s/10time）

	while(ros::ok())		//它负责处理Ctrl-C键盘操作——使得ros::ok()返回FALSE
	{
		if(Flag==1)		//进行 1 次识别
		{
			pub.publish(out_msg);	//向下一个话题 /voice/xf_asr_topic 发布消息
			Flag = 0;		//返回等待判断
			printf("Reset(1) : Flag = 0\n");
			
		}

		if(Flag==2)			//进行多次识别（方案一）
		{
			pub.publish(out_msg);
			while(i++ < 15)		//进行多次识别中一次交流的等待时间 15 秒，此处需具体设置
			{
				printf("sleep : %d\n",i);
				sleep(1);
			}
			i = 0;

			rec_times++;		//识别的次数
			if(rec_times > 5)		//如进行多次识别的次数为 5 次，则重置 Flag 返回等待
			{
				rec_times = 0;
				Flag = 0;
				printf("Reset(2) : Flag = 0\n");
			}
		}

		if(Flag==3)		//重置 Flag = 0 ，用于当 Flag = 2 进行多次识别时，但要强制返回等待判断
		{
			rec_times = 0;
			Flag = 0;
			printf("Reset(3) : Flag = 0\n");
		}

		ros::spinOnce();		//ROS接收器固定条件，当作是收到订阅话题发布的消息时，调用一次回调函数
		
		loop_rate.sleep();		//调用ros::Rate对象来休眠一段时间以使得发布频率为1hz（1s/time）。

		++count;			//调试时计时器参数 count
		printf("count : %d Flag : %d times : %d\n",count,Flag,rec_times);
	}
	return 0;
}

