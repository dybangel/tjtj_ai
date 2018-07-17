#include<ros/ros.h>
#include<std_msgs/String.h>
#include<std_msgs/Int32.h>
#include<sstream>
#include<jsoncpp/json/json.h>
#include<curl/curl.h>
#include<string>
#include<iostream>
#include<exception>

using namespace std;

string result;
int pubFlag = 0;
int order = 0;


int writer(char* data, size_t size, size_t nmemb, string* writerData)
{
	if(writerData == NULL)
	{
		return -1;
	}
	int len = size*nmemb;
	writerData -> append(data, len);
	return len;
}

int parseJsonResonse(string input)
{
	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(input, root);
	if(!parsingSuccessful)
	{
		cout<<"!!! Failed to parse the data" << endl;
		return 1;
	}
	const Json::Value code=root["code"];
	const Json::Value text=root["text"];
	result = text.asString();

	pubFlag = 1;

	if(result.compare("前进")==0){
		order = 1;
		pubFlag = 0;
	}
	if(result.compare("后退")==0){
		order = 2;
		pubFlag = 0;
	}
	if(result.compare("向左")==0){
		order = 3;
		pubFlag = 0;
	}
	if(result.compare("向右")==0){
		order = 4;
		pubFlag = 0;
	}
	if(result.compare("停")==0){
		order = 5;
		pubFlag = 0;
	}
	if(result.compare("转圈")==0){
		order = 6;
		pubFlag = 0;
	}
	if(result.compare("截图")==0){
		order = 7;
		pubFlag = 0;
	}
	cout << "response order : " << order <<endl;
	cout << "response pubFlag : " << pubFlag <<endl;
	cout << "response code : " << code <<endl;
	cout << "response text : " << text <<endl;
	//ROS_INFO("A: %s\n",result.c_str());

	return 0;
}


int HttpPostRequest(string input)
{
	string buffer;

	std::string strJson = "{" ;
	strJson += "\"key\":\"b73db23f3a5d49adb5b0c70f6113b63d\"," ;
	strJson += "\"info\":" ;
	strJson += "\"" ;
	strJson += input ;
	strJson += "\"" ;
	strJson += "}" ;
	cout << "strJson string:" << strJson <<endl;

try
{
	CURL *pCurl = NULL;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);

	//get a curl handle
	pCurl = curl_easy_init();
	if(NULL != pCurl)
	{
		//set url timeout
		curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 8);

		//first set the URL that is about to receive our POST
		curl_easy_setopt(pCurl, CURLOPT_URL , "http://www.tuling123.com/openapi/api");

		//set curl http header
		curl_slist *plist = curl_slist_append(NULL, "Content-Type:application/json; charset=UTF-8");
		curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, plist);

		//set curl post content fields
		curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, strJson.c_str());

		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &buffer);
		
		//perform the request, res will get the return code
		res = curl_easy_perform(pCurl);

		//check for errors
		if(res != CURLE_OK)
		{
			printf("curl_easy_perform() failed%s\n", curl_easy_strerror(res));
		}
		//always cleanup
		curl_easy_cleanup(pCurl);
	}
	curl_global_cleanup();
}
catch(std::exception &ex)
{
	printf("!!! curl exception %s.\n", ex.what());
}

	if(buffer.empty())
	{
		cout << "!!! ERROR The TurLing server response NULL" <<endl;
	}
	else
	{
		parseJsonResonse(buffer);
	}

	return 0;
}


void nluCallback(const std_msgs::String::ConstPtr& msg)
{
	std::cout<<"my question: [ "<< msg->data << " ]" <<endl;
	HttpPostRequest(msg->data);
}

int main(int argc,char* argv[])
{
	printf("语义理解模块启动\n");

	ros::init(argc, argv, "tuling_nlu_node");	//创建节点 tuling_nlu_node
	ros::NodeHandle nd;  				//订阅话题 /voice/tuling_nlu_topic
	ros::Subscriber sub = nd.subscribe("/voice/tuling_nlu_topic", 10 , nluCallback);
							//发布消息 /voice/xf_tts_topic
	ros::Publisher pub = nd.advertise<std_msgs::String>("/voice/xf_tts_topic", 10);
	ros::Publisher pub_1 = nd.advertise<std_msgs::Int32>("base_control", 10);

	ros::Rate loop_rate(10);

	while(ros::ok())
	{	
		if(pubFlag==1){
			std_msgs::String msg;
			msg.data=result;
			pub.publish(msg);
			pubFlag=0;
		}

		if(order==1){
			std_msgs::Int32 minglin;
			minglin.data=1;
			pub_1.publish(minglin);
			order=0;
		}
		if(order==2){
			std_msgs::Int32 minglin;
			minglin.data=2;
			pub_1.publish(minglin);
			order=0;
		}
		if(order==3){
			std_msgs::Int32 minglin;
			minglin.data=3;
			pub_1.publish(minglin);
			order=0;
		}
		if(order==4){
			std_msgs::Int32 minglin;
			minglin.data=4;
			pub_1.publish(minglin);
			order=0;
		}
		if(order==5){
			system("kill -2 $(pgrep mplayer)");
			order=0;
		}
		if(order==6){
			std_msgs::Int32 minglin;
			minglin.data=6;
			pub_1.publish(minglin);
			order=0;
		}
		if(order==7){
			std_msgs::Int32 minglin;
			minglin.data=7;
			pub_1.publish(minglin);
			order=0;
		}

		ros::spinOnce();			//调用一次回调函数
		loop_rate.sleep();
	}

	return 0;
}
