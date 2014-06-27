#include <Utils/EasyJson.h>

Json::~Json()
{
	delete m_szJsonData;
}

Json::Json()
{
	m_iPointer = 0;
	iSize = 0;
}

Json::Json(const char *szInput)
{
	iSize = strlen(szInput);
	m_szJsonData = new char[iSize+1];

	strcpy(m_szJsonData, szInput);

	int iState = 0;

	iSize = iSize-1;
	m_iPointer = 1;
	std::string szCurrentKey;
	JsonValue sCurrentValue;
	while(m_iPointer<iSize)
	{
		if(m_szJsonData[m_iPointer] != ',' && m_szJsonData[m_iPointer] != ' ' && m_szJsonData[m_iPointer] != '\n' && m_szJsonData[m_iPointer] != '\r' && m_szJsonData[m_iPointer] != '	')
		{
			switch(iState)
			{
			case 0:
				{
					iState = 1;
					szCurrentKey = ReadKey();
				break;
				}
			case 1:
				{
					iState = 0;
					sCurrentValue = ReadValue();
					++m_iElements;
					jsonKeyValues.insert(jsonKeyValues.end(), std::pair<std::string, JsonValue>(szCurrentKey, sCurrentValue));
				break;
				}
			}
		}
		++m_iPointer;
	}
}

std::string Json::ReadKey()
{
	std::string szKey;
	while(m_iPointer<iSize)
	{
		if((m_szJsonData[m_iPointer] == '"' || m_szJsonData[m_iPointer] == '\'') && m_szJsonData[m_iPointer-1] != '\\' || m_szJsonData[m_iPointer] == ' ' || m_szJsonData[m_iPointer] == '	')
		{
			++m_iPointer;
			continue;
		}
		if(m_szJsonData[m_iPointer] == ':')
		{
			return szKey;
		}
		if(m_szJsonData[m_iPointer+1] == '"' && m_szJsonData[m_iPointer] == '\\')
		{
			szKey += '"';
			++m_iPointer;
		}else{
			szKey += m_szJsonData[m_iPointer];
		}
		++m_iPointer;
	}
	return szKey;
}

JsonValue Json::ReadValue()
{
	JsonValue jsonValue;

	jsonValue.eType = JsonType::None;
	
	bool bFirst = false;
	int iType = 0;
	while(m_iPointer<iSize)
	{
		if(bFirst)
		{
			if(iType == 0)
			{
				printf("[JSON ERROR] Unable to parse value!\n");
				m_iPointer = iSize;
				return jsonValue;
			}
			if(iType == 1)
			{
				if(m_szJsonData[m_iPointer] == ',' || m_szJsonData[m_iPointer] == ' ' || m_szJsonData[m_iPointer] == '}')
					return jsonValue;
				jsonValue.sValue += m_szJsonData[m_iPointer];
			}
			if(iType == 2)
			{
				if(m_szJsonData[m_iPointer] == '"' && m_szJsonData[m_iPointer-1] != '\\')
				{
					return jsonValue;
				}
				if(m_szJsonData[m_iPointer+1] == '"'  && m_szJsonData[m_iPointer] == '\\')
				{
					jsonValue.sValue += '"';
					++m_iPointer;
				}else{
					jsonValue.sValue += m_szJsonData[m_iPointer];
				}
			}
			if(iType == 4)
			{
			}
		}else{
			if(m_szJsonData[m_iPointer] != ' ')
			{
				bFirst = true;
				if(isdigit(m_szJsonData[m_iPointer]) || (m_szJsonData[m_iPointer]=='-' && isdigit(m_szJsonData[m_iPointer+1])))
				{
					iType = 1; // Its a numeric data!
					jsonValue.eType = JsonType::Int;
					continue;
				}
				if(m_szJsonData[m_iPointer] == '"')
				{
					iType = 2; // Its a string data!
					jsonValue.eType = JsonType::String;
					++m_iPointer;
					continue;
				}
				// Is it a boolean?
				if(m_szJsonData[m_iPointer] == 't')
				{
					jsonValue.eType = JsonType::Bool;
					jsonValue.sValue += "true";
					m_iPointer+=3;
					return jsonValue;
				}
				if(m_szJsonData[m_iPointer] == 'f')
				{
					jsonValue.eType = JsonType::Bool;
					jsonValue.sValue += "false";
					m_iPointer+=4;
					return jsonValue;
				}
				if(m_szJsonData[m_iPointer] == '[')
				{
					iType = 3; // Its a json array!
					jsonValue.eType = JsonType::Array;
					jsonValue.sValue = parseJsonArray();
					return jsonValue;
					continue;
				}
				if(m_szJsonData[m_iPointer] == '{')
				{
					iType = 4; // Its a json object!
					jsonValue.eType = JsonType::Object;
					jsonValue.sValue = parseJsonObject();
					return jsonValue;
					continue;
				}
				continue;
			}
		}
		++m_iPointer;
	}
	return jsonValue;
}

std::string Json::parseJsonArray()
{
	int iLevel = 1;
	++m_iPointer;
	std::string jsonArray;
	bool inString=false;
	while(m_iPointer<iSize)
	{
		if(m_szJsonData[m_iPointer] == '"' && m_szJsonData[m_iPointer-1] != '\\')
		{
			inString=!inString;
		}
		if(!inString)
		{
			if(m_szJsonData[m_iPointer] == '[')
			{
				++iLevel;
			}
			if(m_szJsonData[m_iPointer] == ']')
			{
				--iLevel;
			}
		}
		if(iLevel==0)
		{
			return jsonArray;
		}
		
		jsonArray += m_szJsonData[m_iPointer];
		++m_iPointer;
	}
	return jsonArray;
}

std::string Json::parseJsonObject()
{
	int iLevel = 1;
	++m_iPointer;
	std::string szJsonObject;
	bool inString=false;
	while(m_iPointer<iSize)
	{
		if(m_szJsonData[m_iPointer] == '"' && m_szJsonData[m_iPointer-1] != '\\')
		{
			inString=!inString;
		}
		if(!inString)
		{
			if(m_szJsonData[m_iPointer] == '{')
			{
				++iLevel;
			}
			if(m_szJsonData[m_iPointer] == '}')
			{
				--iLevel;
			}
		}
		if(iLevel==0)
		{
			return szJsonObject;
		}
		
		szJsonObject += m_szJsonData[m_iPointer];
		++m_iPointer;
	}
	return szJsonObject;
}


JsonArray Json::getArray(const char *szKey)
{
	return JsonArray(getString(szKey).c_str());
}

void Json::setArray(const char *szKey, JsonArray jArray)
{
	setValue(szKey, JsonType::Array, jArray.Stringify());
}


std::string Json::getString(const char *szKey)
{
	std::map<std::string, JsonValue>::iterator pair;
	for (pair = jsonKeyValues.begin(); pair != jsonKeyValues.end(); pair++)
	{
		if(pair->first.compare(szKey)==0)
		{
			return pair->second.sValue;
		}
	}
	return "";
}

void Json::setString(const char *szKey, std::string szInput)
{
	setValue(szKey, JsonType::String, szInput);
}


Json Json::getObject(const char *szKey)
{
	return Json(getString(szKey).c_str());
}

void Json::setObject(const char *szKey, Json jObject)
{
	setValue(szKey, JsonType::Object, jObject.Stringify());
}

int Json::getInt(const char *szKey)
{
	return atoi(getString(szKey).c_str());
}

void Json::setInt(const char *szKey, int iValue)
{
	char szValue[32];
	sprintf(szValue, "%d", iValue);
	setValue(szKey, JsonType::Int, szValue);
}

bool Json::getBool(const char *szKey)
{
	if(strcmp(getString(szKey).c_str(), "true")==0)
		return true;
	return false;
}

void Json::setBool(const char *szKey, bool bValue)
{
	if(bValue)
	{
		setValue(szKey, JsonType::Bool, "true");
	}else{
		setValue(szKey, JsonType::Bool, "false");
	}
}

void Json::setValue(const char *szKey, JsonType jType, std::string szValue)
{
	JsonValue jValue;
	jValue.eType = jType;
	jValue.sValue = szValue;
	jsonKeyValues.insert(std::pair<std::string, JsonValue>(szKey, jValue));
}

double Json::getFloat(const char *szKey)
{
	return atof(getString(szKey).c_str());
}

void Json::setFloat(const char *szKey, double fValue)
{
	char szValue[32];
	sprintf(szValue, "%f", fValue);
	setValue(szKey, JsonType::Float, szValue);
}

std::string Json::Stringify()
{
	std::string szJsonString;
	szJsonString += "{";
	
	std::map<std::string, JsonValue>::iterator pair;
	for (pair = jsonKeyValues.begin(); pair != jsonKeyValues.end(); pair++)
	{
		const char *szKey = pair->first.c_str();
		JsonType jType = pair->second.eType;
		szJsonString += '"';
		szJsonString += szKey;
		szJsonString += '"';
		szJsonString += ':';

		switch(jType)
		{
		case JsonType::Object:
		case JsonType::Int:
		case JsonType::Float:
		case JsonType::Bool:
			{
				szJsonString += pair->second.sValue;
				break;
			}
		case JsonType::Array:
		{
			szJsonString += "[";
			szJsonString += pair->second.sValue;
			szJsonString += "]";
			break;
		}
		case JsonType::String:
			{
				szJsonString += "\"";
				szJsonString += pair->second.sValue;
				szJsonString += "\"";
				break;
			}
		}
		szJsonString += ",";
	}
	szJsonString = szJsonString.substr(0, szJsonString.size()-1);
	szJsonString += "}";
	return szJsonString;
}