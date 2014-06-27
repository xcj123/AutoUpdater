#include <Utils/string.h>


vector<string> str_explode(char szDelimiter, string szString, int iLimit)
{
	vector<string> parts;
	int i = 1;
	string part;
	for (std::string::size_type x = 0; x < szString.size(); ++x)
	{
		char c = szString[x];
		if (c == szDelimiter && i != iLimit)
		{
			parts.insert(parts.end(), part);
			part.clear();
			++i;
		}
		else{
			part += c;
		}
	}
	parts.insert(parts.end(), part);
	return parts;
}

bool char_replace(const char pattern, const char replace, char* szContext, char *result)
{
	int contextSize = strlen(szContext);
	for (unsigned int i = 0; i < contextSize; ++i)
	{
		char c = szContext[i];
		if (c == pattern)
		{
			if (replace == 0)
				continue;
			result[i] = replace;
		}
		else
		{
			result[i] = c;
		}
	}
	return true;
}

string& str_replace(const string &search, const string &replace, string &subject)
{
	string buffer;

	int sealeng = search.length();
	int strleng = subject.length();

	if (sealeng == 0)
		return subject;//no change

	for (int i = 0, j = 0; i<strleng; j = 0)
	{
		while (i + j<strleng && j<sealeng && subject[i + j] == search[j])
			j++;
		if (j == sealeng)//found 'search'
		{
			buffer.append(replace);
			i += sealeng;
		}
		else
		{
			buffer.append(&subject[i++], 1);
		}
	}
	subject = buffer;
	return subject;
}

string strtolower(string input)
{
	int fSize = input.size();
	for (int i = 0; i < fSize; ++i)
	{
		input[i] = tolower(input[i]);
	}
	return input;
}

string strtoupper(string input)
{
	int fSize = input.size();
	for (int i = 0; i < fSize; ++i)
	{
		input[i] = toupper(input[i]);
	}
	return input;
}