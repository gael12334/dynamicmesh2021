#pragma once
#define SCOPE_LIFETIME_DEBUG
#define ENABLE_STACK_TRACE

#include "logger.h"

void DoSomething() Do
	grxLogFile* logger = grxGetUniqueLogFile();
	grxException* ex = Inst(grxException, grxCreateException_IndexOutOfRange("bruh", 9, __LINE__, false), grxFreeException);
	grxWriteExceptionLogFile(logger, ex);

	Return;
End;
