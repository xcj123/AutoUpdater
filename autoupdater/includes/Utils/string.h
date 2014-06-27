#ifndef H_STRING
#define H_STRING
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <cstdlib>

using namespace std;

vector<string> str_explode(char szDelimiter, string szString, int iLimit = -1);
string& str_replace(const string &search, const string &replace, string &subject);
string strtolower(string input);
string strtoupper(string input);

#endif