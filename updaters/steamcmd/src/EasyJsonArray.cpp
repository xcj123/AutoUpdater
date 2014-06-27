#include <Utils/EasyJson.h>
#include <Utils/EasyJsonArray.h>

JsonArray::JsonArray()
{
	iSize = 0;
	m_iPointer = 0;
	m_iArraySize = 0;
}

JsonArray::JsonArray(const char *p_szInput)
{
	iSize = strlen(p_szInput);
	m_szJsonData = new char[iSize+1];
	strcpy(m_szJsonData, p_szInput);
	m_iPointer = 0;
	m_iArraySize = 0;

	while(m_iPointer<iSize)
	{
		if(m_szJsonData[m_iPointer] != '\n' && m_szJsonData[m_iPointer] != '\r' && m_szJsonData[m_iPointer] != ' ' && m_szJsonData[m_iPointer] != ',' && m_szJsonData[m_iPointer] != '	')
		{
			JsonValue jsonValue;
			jsonValue = ReadValue();
			m_vArrayValues.insert(m_vArrayValues.end(), jsonValue);
			++m_iArraySize;
		}
		++m_iPointer;
	}
	
}

JsonArray::~JsonArray()
{
	delete m_szJsonData;
}

JsonArray JsonArray::getArray(int iKey)
{
	return JsonArray(getString(iKey).c_str());
}

int JsonArray::getArraySize()
{
	return m_iArraySize;
}

JsonValue JsonArray::ReadValue()
{
	JsonValue jsonValue;
	while(m_iPointer<iSize)
	{
		if(m_szJsonData[m_iPointer] == '"')
		{
			jsonValue.eType = JsonType::String;
			jsonValue.sValue = ReadString();
			return jsonValue;
		}
		if(isdigit(m_szJsonData[m_iPointer]) || (m_szJsonData[m_iPointer]=='-' && isdigit(m_szJsonData[m_iPointer+1])))
		{
			jsonValue.eType = JsonType::Int;
			jsonValue.sValue = ReadNumber();
			return jsonValue;
		}
		if(m_szJsonData[m_iPointer] == '{')
		{
			jsonValue.eType = JsonType::Object;
			jsonValue.sValue = ReadObject();
			return jsonValue;
		}
		if(m_szJsonData[m_iPointer] == '[')
		{
			jsonValue.eType = JsonType::Array;
			jsonValue.sValue = ReadArray();
			return jsonValue;
		}
		if(m_szJsonData[m_iPointer] == 't')
		{
			m_iPointer+=3;
			jsonValue.eType = JsonType::Bool;
			jsonValue.sValue = "true";
			return jsonValue;
		}
		if(m_szJsonData[m_iPointer] == 'f')
		{
			m_iPointer+=4;
			jsonValue.eType = JsonType::Bool;
			jsonValue.sValue = "false";
			return jsonValue;
		}
		++m_iPointer;
	}
	return jsonValue;
}

std::string JsonArray::ReadString()
{
	std::string szString;
	++m_iPointer;
	while(m_iPointer<iSize)
	{
		if(m_szJsonData[m_iPointer] == '"' && m_szJsonData[m_iPointer-1] != '\\')
		{
			return szString;
		}
		if(m_szJsonData[m_iPointer] == '\\' && m_szJsonData[m_iPointer+1] == '"')
		{
			szString += '"';
			++m_iPointer;
		}else{
			szString += m_szJsonData[m_iPointer];
		}
		++m_iPointer;
	}
	return szString;
}

std::string JsonArray::ReadNumber()
{
	std::string szString;
	if(m_szJsonData[m_iPointer] == '-')
	{
		szString +='-';
		++m_iPointer;
	}
	
	while(m_iPointer<iSize)
	{
		if(!isdigit(m_szJsonData[m_iPointer]) && m_szJsonData[m_iPointer] != '.')
		{
			return szString;
		}
		szString += m_szJsonData[m_iPointer];
		++m_iPointer;
	}
	return szString;
}

std::string JsonArray::ReadArray()
{
	std::string szArray;
	++m_iPointer;
	int iLevel = 1;
	bool inString = false;

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

		if(iLevel == 0)
			return szArray;

		szArray += m_szJsonData[m_iPointer];
		++m_iPointer;

	}
	return szArray;
}

std::string JsonArray::ReadObject()
{
	std::string szObject;
	int iLevel = 0;
	bool inString = false;

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

		szObject += m_szJsonData[m_iPointer];

		if(iLevel == 0)
			return szObject;
		++m_iPointer;
	}
	return szObject;
}


Json JsonArray::getObject(int iKey)
{
	return Json(getString(iKey).c_str());
}

void JsonArray::setObject(int iKey, Json jObject)
{
	setValue(iKey, JsonType::Object, jObject.Stringify());
}

int JsonArray::getInt(int iKey)
{
	return atoi(getString(iKey).c_str());
}

void JsonArray::setInt(int iKey, int iValue)
{
	char szValue[32];
	sprintf(szValue, "%d", iValue);
	setValue(iKey, JsonType::Int, szValue);
}

bool JsonArray::getBool(int iKey)
{
	if(strcmp(getString(iKey).c_str(), "true")==0)
		return true;
	return false;
}

void JsonArray::setBool(int iKey, bool bValue)
{
	if(bValue)
	{
		setValue(iKey, JsonType::Bool, "true");
	}else{
		setValue(iKey, JsonType::Bool, "false");
	}
}

double JsonArray::getFloat(int iKey)
{
	return atof(getString(iKey).c_str());
}

std::string JsonArray::getString(int iKey)
{
	int iSize = m_vArrayValues.size();
	for(int i=0;i<iSize;++i)
	{
		if(i==iKey)
		{
			return m_vArrayValues[i].sValue;
		}
	}
	return "";
}

void JsonArray::setString(int iKey, std::string szValue)
{
	JsonValue jsonValue;
	jsonValue.eType = JsonType::String;
	jsonValue.sValue = szValue;
	setValue(iKey, JsonType::String, szValue);
}


void JsonArray::setValue(int iKey, JsonType jsonType, std::string szValue)
{
	JsonValue jsonValue;
	jsonValue.eType = jsonType;
	jsonValue.sValue = szValue;
	if(m_vArrayValues.size()>iKey)
	{
		m_vArrayValues[iKey]=jsonValue;
	}else{
		m_vArrayValues.push_back(jsonValue);
	}
}


std::string JsonArray::Stringify()
{
	std::string szJsonString;
	int aSize = m_vArrayValues.size();
	for(int i=0;i<aSize;++i)
	{
		switch(m_vArrayValues[i].eType)
		{
		case JsonType::Object:
		case JsonType::Int:
		case JsonType::Float:
		case JsonType::Bool:
			{
				szJsonString += m_vArrayValues[i].sValue;
				break;
			}
		case JsonType::Array:
			{
				szJsonString += "[";
				szJsonString += m_vArrayValues[i].sValue;
				szJsonString += "]";
				break;
			}
		case JsonType::String:
			{
				szJsonString += "\"";
				szJsonString += m_vArrayValues[i].sValue;
				szJsonString += "\"";
				break;
			}
		}
		szJsonString += ",";
	}
	szJsonString = szJsonString.substr(0, szJsonString.size()-1);
	return szJsonString;
}