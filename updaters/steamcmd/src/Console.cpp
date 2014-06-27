#include <Utils/Console.h>

void PRINT(const char *szFormat, ...)
{
	SETCOLOR(COLORS::White);
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	
	va_list args;
	va_start(args, szFormat);
	
	char szBuffer[512];
	vsnprintf(szBuffer, sizeof(szBuffer), szFormat, args);
	
	printf("[%d:%d:%d] %s\n", tm->tm_hour, tm->tm_min, tm->tm_sec, szBuffer);
	SETCOLOR(COLORS::Gray, COLORS::Black);
	va_end(args);
}

void DEBUG(const char *szFormat, ...)
{
#ifdef _DEBUG
	SETCOLOR(COLORS::Gray);
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	
	va_list args;
	va_start(args, szFormat);
	
	char szBuffer[512];
	int iNeed = vsnprintf(szBuffer, sizeof(szBuffer), szFormat, args)+1;
	
	printf("[%d:%d:%d] [DEBUG] %s\n", tm->tm_hour, tm->tm_min, tm->tm_sec, szBuffer);
	SETCOLOR(COLORS::Gray, COLORS::Black);
	va_end(args);
#endif
}


void WARNING(const char *szFormat, ...)
{
	SETCOLOR(COLORS::Yellow);
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	
	va_list args;
	va_start(args, szFormat);
	
	char szBuffer[512];
	vsnprintf(szBuffer, sizeof(szBuffer), szFormat, args);
	
	printf("[%d:%d:%d] [WARNING] %s\n", tm->tm_hour, tm->tm_min, tm->tm_sec, szBuffer);
	SETCOLOR(COLORS::Gray, COLORS::Black);
	va_end(args);
}

void GOOD(const char *szFormat, ...)
{
	SETCOLOR(COLORS::Green);
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	
	va_list args;
	va_start(args, szFormat);
	
	char szBuffer[512];
	vsnprintf(szBuffer, sizeof(szBuffer), szFormat, args);
	
	printf("[%d:%d:%d] %s\n", tm->tm_hour, tm->tm_min, tm->tm_sec, szBuffer);
	SETCOLOR(COLORS::Gray, COLORS::Black);
	va_end(args);
}

void FATAL(const char *szFormat, ...)
{
	SETCOLOR(COLORS::Red);
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	
	va_list args;
	va_start(args, szFormat);
	
	char szBuffer[512];
	vsnprintf(szBuffer, sizeof(szBuffer), szFormat, args);
	
	printf("[%d:%d:%d] [FATAL ERROR] %s\n", tm->tm_hour, tm->tm_min, tm->tm_sec, szBuffer);
	SETCOLOR(COLORS::Gray, COLORS::Black);
	va_end(args);
}


void SETCOLOR(COLORS textcolor, COLORS backgroundcolor)
{
#ifdef _WIN32
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), (textcolor+(backgroundcolor*16)));
#else
	printf("\033[0;%d;%dm", textcolor+30, backgroundcolor+40);
#endif
}
