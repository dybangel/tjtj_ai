#include <string>
#include <iostream>
#include <sstream>

#include<ros/ros.h>
#include<std_msgs/String.h>
#include<std_msgs/Int32.h>

#include <jsoncpp/json/json.h>
#include <curl/curl.h>

#define DETECTPOSTURL	"https://api-cn.faceplusplus.com/facepp/v3/detect"
#define SEARCHPOSTURL	"https://api-cn.faceplusplus.com/facepp/v3/search"
#define API_KEY		"N--rRKvP_PWMcenfm3iagDOuAclvplLj"
#define API_SECRET	"_0FurYPeCadidaynJv2C_0oDlr2VDKmB"


using namespace std;

int pubFlag = 0;
string result;


// detectAPIResponse() 函数解析结果
string face_token;	//人脸的标识，根据这个值判断人名
string gender;		//性别
int age;		//年龄
double facequality;	//上传图片的人脸质量
string glass;		//判断眼部是否戴眼镜，分为3种状态 None/Dark/Normal
double smile;		//微笑值

//searchAPIResponse() 函数解析结果
double confidence;
const char *search_face_token;
string rec_face_token;
string user_id;
string rec_person;

//合成json字符串发去qt话题的函数
void makeJson()
{
	//cout << "-- Make Json --" << endl;
	
	char c[10];

	std::string strJson = "{" ;
	strJson += "\"rec_person\":" ;
	strJson += "\"" ;
	strJson += rec_person ;
	strJson += "\"," ;

	strJson += "\"gender\":" ;
	strJson += "\"" ;
	strJson += gender ;
	strJson += "\"," ;

	sprintf(c, "%d", age);
	strJson += "\"age\":" ;
	strJson += "\"" ;
	strJson += c ;
	strJson += "\"," ;

	sprintf(c, "%.3f", smile);
	strJson += "\"smile\":" ;
	strJson += "\"" ;
	strJson += c ;
	strJson += "\"," ;

	strJson += "\"glass\":" ;
	strJson += "\"" ;
	strJson += glass ;
	strJson += "\"," ;

	sprintf(c, "%.3f", confidence);
	strJson += "\"confidence\":" ;
	strJson += "\"" ;
	strJson += c ;
	strJson += "\"" ;

	strJson += "}" ;

	//cout << "strJson string:" << strJson <<endl;

	result = strJson;
	//cout << "发布内容:" << result <<endl;
}


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


/**
	上传并识别图片 detectAPI
*/

int detectAPIResponse(string input)
{
	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(input, root);
	if(!parsingSuccessful)
	{
		cout<<"-- Failed to parse the Detect API Response --" << endl;
		return 1;
	}
	cout << input << endl <<endl;

	Json::Value token = root["faces"][0]["face_token"];
	face_token = token.asString();

	Json::Value attributes = root["faces"][0]["attributes"];

	Json::Value gender_data = attributes["gender"];
	gender = gender_data["value"].asString();

	Json::Value age_data = attributes["age"];
	age = age_data["value"].asInt();
	
	Json::Value facequality_data = attributes["facequality"];
	facequality = facequality_data["value"].asDouble();
	
	Json::Value glass_data = attributes["glass"];
	glass = glass_data["value"].asString();

	Json::Value smile_data = attributes["smile"];
	smile = smile_data["value"].asDouble();

	search_face_token = face_token.c_str();

	cout << "face_token : " << face_token << endl;
	cout << "search_face_token : " << search_face_token << endl;
	cout << "gender : " << gender << endl;
	cout << "age : " << age << endl;
	cout << "facequality : " << facequality << endl;
	cout << "glass : " << glass << endl;
	cout << "smile : " << smile << endl << endl;
	
	return 0;
}

void detectAPI()
{
	cout << "-- 上传并识别图片 detectAPI --" << endl;
	cout << "picture name : pic_demo.jpg" << endl;

	string buffer;

	char *file_data = NULL;
        long file_size = 0;

        FILE* fp = fopen( "pic_demo.jpg" ,"rb");
        if (!fp)
	{
		cout << "-- Failed to open picture --" << endl;
        }
	else
	{
		fseek(fp, 0, SEEK_END);
		file_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		file_data = new char[file_size+1];
		fread(file_data,1,file_size,fp);
		fclose(fp);
	}

	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();

	struct curl_httppost *formpost = 0;
	struct curl_httppost *lastptr = 0;

	curl_formadd(
	&formpost,
	&lastptr,
	CURLFORM_COPYNAME, "api_key",
	CURLFORM_COPYCONTENTS, API_KEY ,
	CURLFORM_END);

        curl_formadd(
	&formpost,
	&lastptr,
	CURLFORM_COPYNAME, "api_secret",
	CURLFORM_COPYCONTENTS, API_SECRET ,
	CURLFORM_END);

	curl_formadd(
	&formpost,
	&lastptr,
	CURLFORM_COPYNAME, "image_file",	//CURLFORM_PTRNAME, "image_file"
	CURLFORM_BUFFER, "image_data",
	CURLFORM_BUFFERPTR, file_data,
	CURLFORM_BUFFERLENGTH, file_size,
	CURLFORM_END);

	curl_formadd(
	&formpost,
	&lastptr,
	CURLFORM_COPYNAME, "return_landmark",
	CURLFORM_COPYCONTENTS, "0",
	CURLFORM_END);

	curl_formadd(
	&formpost,
	&lastptr,
	CURLFORM_COPYNAME, "return_attributes",
	CURLFORM_COPYCONTENTS, "gender,age,smiling,glass,facequality",
	CURLFORM_END);

	curl_easy_setopt(curl, CURLOPT_URL, DETECTPOSTURL);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

	res = curl_easy_perform(curl);
	if(res != CURLE_OK)
	{
		printf("-- curl_easy_perform() failed : %s --\n", curl_easy_strerror(res));
	}

	curl_global_cleanup();

	if(buffer.empty())
	{
		cout << "-- Detect API Response is NULL --" <<endl;
	}
	else
	{
		detectAPIResponse(buffer);
	}
}


/**
	人脸搜索 searchAPI
*/

int searchAPIResponse(string input)
{
	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(input, root);
	if(!parsingSuccessful)
	{
		cout<<"-- Failed to parse the SEARCH API Response --" << endl;
		return 1;
	}
	cout << input << endl <<endl;

	Json::Value results = root["results"][0];

	confidence = results["confidence"].asDouble();
	
	user_id = results["user_id"].asString();

	rec_face_token = results["face_token"].asString();

	if(confidence > 60){
		if(rec_face_token == "ed6bf63f4cd74adfd58727748ef2ca6b")rec_person="甘国柱";
		if(rec_face_token == "441efaf3bae3823f2136361a3d4903eb")rec_person="钟泽宇";
	}
	else{
		rec_person="陌生人";
	}

	cout << "rec_face_token : " << rec_face_token << endl;
	cout << "confidence : " << confidence << endl;
	cout << "rec_person : " << rec_person << endl;
	
	return 0;
}

void searchAPI()
{
	cout << "-- 人脸搜索 searchAPI --" << endl;
	cout << "search_face_token : " << search_face_token << endl;
	cout << "outer_id : SiYuan" << endl;
	cout << "-- result --" << endl;

	string buffer;

/*	上传文件的方式

	char *file_data = NULL;  
        long file_size = 0;  
          
        FILE* fp = fopen( argv[1] ,"rb");
        if (!fp)
	{
		cout << "-- Failed to open file --" << endl;
        }
	else
	{
		fseek(fp, 0, SEEK_END);
		file_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		file_data = new char[file_size+1];
		fread(file_data,1,file_size,fp);
		fclose(fp);  
	}
*/

	CURL *curl;  
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();

	struct curl_httppost *formpost = 0;  
	struct curl_httppost *lastptr = 0; 

	curl_formadd(
	&formpost,  
	&lastptr,  
	CURLFORM_COPYNAME, "api_key",   
	CURLFORM_COPYCONTENTS, API_KEY ,   
	CURLFORM_END);  

        curl_formadd(
	&formpost,  
	&lastptr,  
	CURLFORM_COPYNAME, "api_secret",   
	CURLFORM_COPYCONTENTS, API_SECRET ,  
	CURLFORM_END);

/*	上传文件的方式

	curl_formadd(
	&formpost, 
	&lastptr,
	CURLFORM_COPYNAME, "image_file" ,	//CURLFORM_PTRNAME, "image_file"
	CURLFORM_BUFFER, "image_data",
	CURLFORM_BUFFERPTR, file_data,
	CURLFORM_BUFFERLENGTH, file_size,
	CURLFORM_END);
*/


//	上传 face_token 的方式
	curl_formadd(
	&formpost,  
	&lastptr,  
	CURLFORM_COPYNAME, "face_token",   
	CURLFORM_COPYCONTENTS, search_face_token ,
	CURLFORM_END);

	curl_formadd(
	&formpost, 
	&lastptr, 
	CURLFORM_COPYNAME, "outer_id" ,
	CURLFORM_COPYCONTENTS, "SiYuan",
	CURLFORM_END);

	curl_easy_setopt(curl, CURLOPT_URL, SEARCHPOSTURL);  
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);  
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer); 
	
	res = curl_easy_perform(curl);
	if(res != CURLE_OK)
	{
		printf("-- curl_easy_perform() failed : %s --\n", curl_easy_strerror(res));
	}

	curl_global_cleanup();
	
	if(buffer.empty())
	{
		cout << "-- SEARCH API Response is NULL --" <<endl;
	}
	else
	{
		searchAPIResponse(buffer);
	}
}

void searchCallback(const std_msgs::Int32::ConstPtr& in_msg)
{
	if(in_msg->data == 1)
	{
		detectAPI();
		searchAPI();
		makeJson();
		pubFlag = 1 ;
	}
}

int main(int argc, char *argv[])  
{
	ros::init(argc, argv, "face_search_node");	//创建节点 face_search_node
	ros::NodeHandle nd;  				//订阅话题 /voice/face_search_topic
	ros::Subscriber sub = nd.subscribe("/face/face_search_topic", 1 , searchCallback);
							//发布消息去话题 qt_msg_node
	ros::Publisher pub = nd.advertise<std_msgs::String>("xf_tts_node", 10);
	ros::Rate loop_rate(10);

	while(ros::ok())
	{
		if(pubFlag == 1)
		{
			std_msgs::String out_msg;
			out_msg.data = result;
			pub.publish(out_msg);
			pubFlag = 0 ;//重置
		}
		ros::spinOnce();			//调用一次回调函数

		loop_rate.sleep();	
	}

	return 0;
}





