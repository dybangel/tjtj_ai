#include <stdio.h>
#include <stdlib.h>
#include<ros/ros.h>
#include<std_msgs/String.h>
#include<std_msgs/Int32.h>
#include<sstream>
#include<jsoncpp/json/json.h>
#include<curl/curl.h>
#include<string>
#include<iostream>
#include<exception>
#include <sqlite3.h>
using namespace std;

static std::string key;//tulingkey
static std::string lastquestion,lastquestion2;//最后一次的用户问题
string result;
int pubFlag = 0;
int studyFlag = 0;//0,1
int countFlag = 1;//1,2
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
	//result = "测试测试成功";
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
	//strJson += "\"key\":\"6cbfde5c0ed14a0e842df9b8b41c3b95\"," ;
	strJson += "\"key\":\""+key+"\"," ;
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
	//先检索本地数据库，如果有结果则给result赋值，反之调用httppost
	//static std::string select_query[]="";
	char select_query[120]="";
	int ret = 0;
	sqlite3 *db = 0;
	char *s;
  //打开数据库，不存在，创建数据库db
	 ret = sqlite3_open("/root/catkin_ws/src/voice_system/mydb",&db);
	if(ret != SQLITE_OK)
	{  
		printf("无法打开数据库\n");
	}
	printf("数据库连接成功\n");
   ret = sqlite3_exec(db,"create table if not exists dialog(id int(10),fquestion varchar(100),fanswer varchar(100))",0,0,&s);
         if(ret !=   SQLITE_OK)
		      {   
			     sqlite3_close(db);
			     printf("create error\n");
				      }
	      printf("create success\n");
   //插入一条模拟数据
   // ret = sqlite3_exec(db,"insert into dialog(fquestion,fanswer) values('你好吗','我很好啊哈哈哈')",0,0,&s);
	
        pubFlag=1;
    if(studyFlag==0){
	printf("countflag=%d\n",countFlag);
	if(countFlag==1){
	lastquestion=(msg->data).c_str();
	countFlag=2;
	}else{
	lastquestion2=(msg->data).c_str();
	countFlag=1;
	}
	//如果触发“这样说不对”，机器人直接说那我应该怎么说，并且study标志位为1，下次语言过来时，直接入库并且标志位改为0
    if(msg->data=="这样说不对"){
		result="那我应该怎么说呢?";
        studyFlag=1;
	
  //如果是其他用户语言，则查询数据库，有结果则按照结果说，没结果则调用http
	}else{
			sprintf(select_query,"select fanswer from dialog where fquestion='%s' limit 1",(msg->data).c_str());
			printf("select_query=%s\n",select_query);

			//查询
			int nrow,ncolumn;
			char ** db_result;
			ret = sqlite3_get_table(db,select_query,&db_result,&nrow,&ncolumn,&s);
			 printf("ncolumn=%d\n",ncolumn);
			printf("nrow=%d\n",nrow);
			//查询数据库,有结果
			if(nrow>0){
			    printf("result=%s\n",db_result[1]);
				result = db_result[1];
			//	pubFlag = 1;
			}else{
				HttpPostRequest(msg->data);
			}

	  }
	}else{
	//说明要进入学习模式，语言写入数据库，并且说记住了
	//判断数据库里有没有过往答案，如果有就update，反之insert
	sprintf(select_query,"insert into dialog(fquestion,fanswer) values('%s','%s')",lastquestion.c_str(),(msg->data).c_str());
	printf("select_query=%s\n",select_query);
	//sprintf(select_query,"insert into dialog(fquestion,fanswer) values('%s','%s')",lastquestion,(msg->data).c_str());
    ret = sqlite3_exec(db,select_query,0,0,&s);
	studyFlag=0;
	
	
	}
	printf("lastquestion=%s\n",lastquestion.c_str());
	printf("studyFlag=%d\n",studyFlag);
}

int main(int argc,char* argv[])
{
	printf("语义理解模块启动\n");

	ros::init(argc, argv, "tuling_nlu_node");	//创建节点 tuling_nlu_node
	ros::param::get("~key",key);//从launch tuling_key
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
