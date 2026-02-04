#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "../data-struct/string.h"

typedef const char* grxDateTimeFormat;

#define grxDateTimeFormat_YYYYMMDD "%Y-%m-%d"
#define grxDateTimeFormat_MMDDYYYY "%m-%d-%Y"
#define grxDateTimeFormat_DDMMYYYY "%d-%m-%Y"
#define grxDateTimeFormat_HHMMSS_1 "%H-%M-%S"
#define grxDateTimeFormat_HHMMSS_2 "%H:%M:%S"


typedef struct grxDateTime {
	time_t _time;
	struct tm _info;		
} grxDateTime;

grxDateTime grxGetNowDateTime() {
	grxDateTime dt;
	dt._time = 0;
	time(&dt._time);

	struct tm info;
	localtime_s(&info, &dt._time);
	dt._info = info;
	
	return dt;
}

grxString* grxDateTimeToString(grxDateTime* _dt, grxDateTimeFormat _format) {
	char buffer[100];
	strftime(buffer, sizeof(char) * 100, _format, &_dt->_info);
	return grxCreateAndAssignString(buffer);
}

