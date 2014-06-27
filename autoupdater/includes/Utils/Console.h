#ifndef H_CCONSOLE
#define H_CCONSOLE

#ifdef _WIN32
	#include <windows.h>
#endif

enum COLORS
{
	Black = 0,
#ifdef _WIN32
	DarkBlue,
	DarkGreen,
	DarkTeal,
	DarkRed,
	Purple,
	Brown,
	Gray,
	DarkGray,
	Blue,
	Green,
	Teal,
	Red,
	Pink,
	Yellow,
	White
#else
	DarkBlue = 64,
	DarkGreen = 2,
	DarkTeal = 6,
	DarkRed = 1,
	Purple = 5,
	Brown = 63,
	Gray = 7,
	DarkGray = 60,
	Blue = 64,
	Green = 62,
	Teal = 66,
	Red = 61,
	Pink = 65,
	Yellow = 63,
	White = 67
#endif
	
};

#include <time.h>
#include <vector>
#include <stdarg.h>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

void PRINT(const char *szFormat, ...);
void DEBUG(const char *szFormat, ...);
void WARNING(const char *szFormat, ...);
void GOOD(const char *szFormat, ...);
void FATAL(const char *szFormat, ...);
void SETCOLOR(COLORS textcolor, COLORS backgroundcolor=Black);

#endif