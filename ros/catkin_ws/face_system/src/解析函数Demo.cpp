#include <jsoncpp/json/json.h>
#include <string>
#include <iostream>

int tranToString(string input)
{
	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(input, root);
	if(!parsingSuccessful)
	{
		cout<<"-- Failed to parse the data --" << endl;
		return 1;
	}

	Json::Value rec_person = root["rec_person"];
	rec_person_1 = rec_person.asString();

	Json::Value gender = root["gender"];
	gender_1 = gender.asString();
	
	Json::Value age = root["age"];
	age_1 = age.asInt();

	Json::Value smile = root["smile"];
	smile_1 = smile.asDouble();

	Json::Value glass = root["glass"];
	glass_1 = glass.asString();

	Json::Value confidence = root["confidence"];
	confidence_1 = confidence.asString();

	return 0;
}
