#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "exception.h"
#include "overhaul.h"
#include "datetime.h"
#include "../data-struct/string.h"

typedef struct grxLogFile {
	grxString* _filename;
	FILE* _ioHandler;
} grxLogFile;

grxLogFile* grxCreateLogFile(const char* _nativePath, grxString* _strPath) Do {
	grxString* path = (_nativePath == NULL) ? 
		_strPath :
		Inst(grxString, grxCreateAndAssignString(_nativePath), grxFreeString);

	grxDateTime dt = grxGetNowDateTime();
	grxString* dtStr = Inst(grxString, grxDateTimeToString(&dt, grxDateTimeFormat_MMDDYYYY), grxFreeString);
	grxAddStringBack(dtStr, '.');
	grxAddStringBack(dtStr, 't');
	grxAddStringBack(dtStr, 'x');
	grxAddStringBack(dtStr, 't');
	
	grxStringJoinBack(path, dtStr);
	FILE* file = fopen(grxGetStringNative(path), "a");
	fclose(file);

	grxLogFile* logFile = (grxLogFile*) malloc(sizeof(grxLogFile));
	logFile->_filename = grxCopyString(path);
	logFile->_ioHandler = NULL;
	Return logFile;
} End; 

void grxFreeLogFile(grxLogFile** _file) {
	grxLogFile* logFile = *_file; 
	grxFreeString(&logFile->_filename);

	if(logFile->_ioHandler != NULL)
		fclose(logFile->_ioHandler);

	free(logFile);
	*_file = NULL;
}

void grxOpenLogFile(grxLogFile* _file) {
	_file->_ioHandler = fopen(grxGetStringNative(_file->_filename), "a");
}

void grxCloseLogFile(grxLogFile* _file) {
	fclose(_file->_ioHandler);
}

grxLogFile* grxGetUniqueLogFile() {
	static grxLogFile* log = NULL;
	if(log == NULL)
		log = grxCreateLogFile("", NULL);
	return log;
}

void grxWriteLogFile(grxLogFile* _file, const char* _status, const char* _message) Do {
	grxDateTime dt = grxGetNowDateTime();
	grxString* dtStr = Inst(grxString, grxDateTimeToString(&dt, grxDateTimeFormat_HHMMSS_2), grxFreeString);
	grxAddStringFront(dtStr, '[');
	grxAddStringBack(dtStr, ']');
	grxAddStringBack(dtStr, ' ');
	grxOpenLogFile(_file);

	fputs(grxGetStringNative(dtStr), _file->_ioHandler);
	fputs(_status, _file->_ioHandler);
	fputs(": ", _file->_ioHandler);
	fputs(_message, _file->_ioHandler);
	fputc('\n', _file->_ioHandler);
	grxCloseLogFile(_file);
	Return;
} End; 

void grxWriteExceptionLogFile(grxLogFile* _file, grxException* _ex) {
	grxWriteLogFile(_file, "Exception", grxGetStringNative(_ex->_message));
}

void grxWriteInfoLogFile(grxLogFile* _file, grxString* _msg) {
	grxWriteInfoLogFile(_file, _msg);
}

void grxWriteStartLogFile(grxLogFile* _file) {
	grxWriteLogFile(_file, "Info", "=============== Start of logging session ===============");
}