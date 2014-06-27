#ifndef H_EASYJSONARRAY
#define H_EASYJSONARRAY

#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <map>

class Json;
class JsonArray;

enum JsonType
{
	None,
	String,
	Int,
	Float,
	Bool,
	Array,
	Object
};

struct JsonValue
{
	JsonType eType;
	std::string sValue;
};


class JsonArray
{
private:
	JsonValue ReadValue();
	std::string ReadString();
	std::string ReadNumber();
	std::string ReadArray();
	std::string ReadObject();
	std::vector<JsonValue> m_vArrayValues;
public:
	/*
		This value is set if the input value was set
	*/
	char *m_szJsonData;
	/*
		Number of json elements in stack
	*/
	int iSize;
	/*
		Current json reader position
	*/
	int m_iPointer;
	/*
		Number of elements in the stack
	*/
	int m_iArraySize;

	/*
		This method is used to create a json array
	*/
	JsonArray();

	/*
		This method is used to parse a json array input
	*/
	JsonArray(const char *p_szInput);

	/*
		This method is not used
	*/
	~JsonArray();

	/*
		This method returns the number of elements in json array
	*/
	int getArraySize();

	/*
		This method returns an integer from the JsonArray
	*/
	int getInt(int iKey);
	/*
		This method sets an integer in the JsonArray
	*/
	void setInt(int iKey, int iValue);

	/*
		This method returns a string from the JsonArray
	*/
	std::string getString(int iKey);
	/*
		This method sets a string in the JsonArray
	*/
	void setString(int iKey, std::string szValue);

	/*
		This method returns a Boolean value from the JsonArray
	*/
	bool getBool(int iKey);
	/*
		This method sets a Boolean value in the JsonArray
	*/
	void setBool(int iKey, bool bValue);

	/*
		This method returns a float value from the JsonArray
	*/
	double getFloat(int iKey);
	/*
		This method sets a float value in the JsonArray
	*/
	void setFloat(int iKey, double fValue);

	/*
		This method returns a JsonArray from the JsonArray
	*/
	JsonArray getArray(int iKey);
	/*
		This method sets a JsonArray object in the JsonArray
	*/
	void setArray(int iKey, JsonArray jArray);

	/*
		This method returns a Json object from the JsonArray
	*/
	Json getObject(int iKey);
	/*
		This method sets a Json object in the JsonArray object
	*/
	void setObject(int iKey, Json jObject);

	/*
		This method sets any data in the JsonArray
	*/
	void setValue(int iKey, JsonType type, std::string szValue);

	/*
		This method produces a json string
	*/
	std::string Stringify();
};

#endif

