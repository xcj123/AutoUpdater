#ifndef H_EASYJSON
#define H_EASYJSON

class Json;
class JsonArray;

#include <Utils/EasyJsonArray.h>

#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <map>

class Json
{
private:
	std::string ReadKey();
	JsonValue ReadValue();
	std::string parseJsonArray();
	std::string parseJsonObject();
	unsigned int m_iElements;
public:
	/*
		This value is set if the input value was set
	*/
	char *m_szJsonData;
	/*
		Current json reader position
	*/
	int m_iPointer;
	/*
		Number of json elements in stack
	*/
	int iSize;
	/*
		Json Data Container
	*/
	std::map<std::string, JsonValue> jsonKeyValues;

	// Parse Json String
	Json(const char *szInput);

	// Create a json object
	Json();

	// Produces a json string
	std::string Stringify();

	// Do not call it!
	~Json();
	
	/*
		This method returns an integer from the json object.
	*/
	int getInt(const char *szKey);
	/*
		This method sets an integer in the json object.
	*/
	void setInt(const char *szKey, int iValue);
	
	/*
		This method returns a boolean from the json object.
	*/
	bool getBool(const char *szKey);
	/*
		This method sets a boolean in the json object.
	*/
	void setBool(const char *szKey, bool bValue);

	/*
		This method returns a string from the json object.
	*/
	std::string getString(const char *szKey);

	/*
		This method sets a string in the json object.
	*/
	void setString(const char *szKey, std::string);

	/*
		This method returns a JsonArray object from the json object.
	*/
	JsonArray getArray(const char *szKey);
	/*
		This method sets a JsonArray object in the json object.
	*/
	void setArray(const char *szKey, JsonArray jArray);

	/*
		This method returns a Json object from the json object.
	*/
	Json getObject(const  char *szKey);
	/*
		This method sets a Json object in the json object.
	*/
	void setObject(const  char *szKey, Json jValue);

	/*
		This method returns a float value from the json object.
	*/
	double getFloat(const char *szKey);
	/*
		This method sets a float value in the json object.
	*/
	void setFloat(const char *szKey, double value);

	/*
		This method sets any data in the json object.
	*/
	void setValue(const char *szKey, JsonType jType, std::string szValue);
	
	/*
		This method returns the number of elements in the json object
	*/

	unsigned int getSize() { return m_iElements; }
};

#endif