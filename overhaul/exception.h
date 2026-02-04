#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "overhaul.h"
#include "datetime.h"
#include "../data-struct/string.h"

#ifdef GRX_HANDLE_EXCEPTIONS
#define __HANDLE_EXCEPTIONS
#endif


typedef struct grxException {
	grxString* _message; 
	grxString* _stackTrace;
	bool _quit;
} grxException;	

grxException* grxCreateException(const char* _message, bool _quit) {
	grxException* ex = (grxException*) malloc(sizeof(grxException));
	ex->_message = grxCreateAndAssignString(_message);
	ex->_stackTrace = StackTrace;
	ex->_quit = _quit;
	return ex;
}

void grxFreeException(grxException** _ex) {
	grxFreeString(&(*_ex)->_message);
	grxFreeString(&(*_ex)->_stackTrace);
	free(*_ex);
	*_ex = NULL;
}

grxException* grxCreateException_IndexOutOfRange(const char* _varName, uint32_t _index, uint32_t _line, bool _quit) Do {
	grxString* varName = Inst(grxString, grxCreateAndAssignString(_varName), grxFreeString);
	grxString* index = Inst(grxString, grxIntegerToString(_index), grxFreeString);
	grxString* line = Inst(grxString, grxIntegerToString(_line), grxFreeString);
	grxString* result = Inst(grxString, grxCreateAndAssignString("Index is invalid or out of range.\n\t"), grxFreeString);
	grxString* result1 = Inst(grxString, grxCreateAndAssignString("Line: "), grxFreeString);
	grxString* result2 = Inst(grxString, grxCreateAndAssignString("Info: "), grxFreeString);

	grxStringJoinBack(result, result1);
	grxStringJoinBack(result, line);
	grxAddStringBack(result, '\n');
	grxAddStringBack(result, '\t');
	grxStringJoinBack(result, result2);
	grxStringJoinBack(result, varName);
	grxAddStringBack(result, ' ');
	grxStringJoinBack(result, index);
	grxAddStringBack(result, '\n');

	grxException* ex = grxCreateException(grxGetStringNative(result), _quit);
	Return ex;
} End;

grxException* grxCreateException_IndexOutOfRange(const char* _varName, uint32_t _line, bool _quit) Do {
	grxString* varName = Inst(grxString, grxCreateAndAssignString(_varName), grxFreeString);
	grxString* line = Inst(grxString, grxIntegerToString(_line), grxFreeString);
	grxString* result = Inst(grxString, grxCreateAndAssignString("The pointer and/or reference was NULL.\n\t"), grxFreeString);
	grxString* result1 = Inst(grxString, grxCreateAndAssignString("Line: "), grxFreeString);
	grxString* result2 = Inst(grxString, grxCreateAndAssignString("Info: "), grxFreeString);

	grxStringJoinBack(result, result1);
	grxStringJoinBack(result, line);
	grxAddStringBack(result, '\n');
	grxAddStringBack(result, '\t');
	grxStringJoinBack(result, result2);
	grxStringJoinBack(result, varName);
	grxAddStringBack(result, '\n');

	grxException* ex = grxCreateException(grxGetStringNative(result), _quit);
	Return ex;
} End;

