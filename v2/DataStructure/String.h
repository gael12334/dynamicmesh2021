#pragma once

#include "Abstract.h"
#include "../Overhaul/Overhaul.h"

#define GRX_STRING_ALLOC_SIZE 10

/************************
 * Implementation of an
 * string in C.
 ************************/
typedef struct grxString {
	char* data;
	int64_t elemCount;
	int64_t elemAlloc;	
	int64_t frontIndex;
	int64_t backIndex;
} grxString;

void grxCtorString(grxString* _str) {
	_str->data = Calloc(char, GRX_STRING_ALLOC_SIZE); 
		_str->elemAlloc = GRX_STRING_ALLOC_SIZE;
		_str->frontIndex = -1;
		_str->backIndex = 0;
		_str->data[_str->backIndex] = '\0';
		_str->backIndex++;
		_str->elemCount = 1;
}

void grxDtorString(grxString* _str) {
	if(_str->data) 
		free(_str->data);
}

grxString* grxCreateString() {
	grxString* str = Malloc(grxString, 1);
	grxCtorString(str);
	return str;
}

void grxDestroyString(grxString** _str) {
	grxDtorString(*_str);
	free(*_str);
}

int64_t grxGetStringCount(grxString* _str) {
	return _str->elemCount;
}

char grxGetStringValue(grxString* _str, int64_t _index) {
	if((_index >= 0) && (_index < _str->elemCount)) 
		return _str->data[_str->frontIndex + 1 + _index];
	
	grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
	return NULL;
}

char grxGetStringValue_Fast(grxString* _str, int64_t _index) {
	return _str->data[_str->frontIndex + 1 + _index];
}

char* grxGetStringNative(grxString* _str) {
	return &_str->data[_str->frontIndex + 1]; // Or potentially "_str->data + _str->frontIndex + 0x1"
}

char grxGetStringFirst(grxString* _str) {
	if(_str->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return NULL;
	}

	return _str->data[_str->frontIndex + 1];
}

char grxGetStringLast(grxString* _str) {
	if(_str->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return NULL;
	}

	return _str->data[_str->backIndex - 1];
}

void grxSetStringValue(grxString* _str, int64_t _index, char _data) {
	if((_index > 0) && (_index < _str->elemCount)) 
		_str->data[_str->frontIndex + 1 + _index] = _data;
	
	else 
		grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
}

void grxSetStringValue_Fast(grxString* _str, int64_t _index, char _data) {
	_str->data[_str->frontIndex + 1 + _index] = _data;
}

void grxSetStringFirst(grxString* _str, char _data) {
	if(_str->data == NULL) 
		grxSetError(GRX_ERR_NULL_REFERENCE);

	_str->data[_str->frontIndex + 1] = _data;
}

void grxSetStringLast(grxString* _str, char _data) {
	if(_str->data == NULL) 
		grxSetError(GRX_ERR_NULL_REFERENCE);

	_str->data[_str->backIndex - 1] = _data;
}

void grxSwapStringElement(grxString* _str, int64_t _src, int64_t _dest) {
	if(_str->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}
	
	if((_src < _str->elemCount) && (_str->elemCount) && (_src > 0) && (_dest > 0)) {
		char a = grxGetStringValue_Fast(_str, _src);
		char b = grxGetStringValue_Fast(_str, _dest);
		grxSetStringValue_Fast(_str, _src, b);
		grxSetStringValue_Fast(_str, _dest, a);
		return;
	}

	grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
}

void grxSwapStringElement_Fast(grxString* _str, int64_t _src, int64_t _dest) {
	char a = grxGetStringValue_Fast(_str, _src);
	char b = grxGetStringValue_Fast(_str, _dest);
	grxSetStringValue_Fast(_str, _src, b);
	grxSetStringValue_Fast(_str, _dest, a);
}

void grxAddStringBack(grxString* _str, char _data) {
	if(_str->elemCount == INT64_MAX) {
		grxSetError(GRX_ERR_OVERFLOW);
		return;
	}

	if((_str->backIndex % GRX_STRING_ALLOC_SIZE) == 0) {
		if((INT64_MAX - _str->elemAlloc) < GRX_STRING_ALLOC_SIZE) {
			grxSetError(GRX_ERR_OVERFLOW);
			return;
		}

		char* tempArray = _str->data;
		_str->data = Calloc(char, _str->elemAlloc + GRX_STRING_ALLOC_SIZE);
		MemCpy(_str->data, tempArray, sizeof(char), _str->elemAlloc);
		free(tempArray);

		_str->elemAlloc += GRX_STRING_ALLOC_SIZE;
		_str->data[_str->backIndex] = _data;
		grxSwapStringElement_Fast(_str, _str->backIndex, _str->backIndex - 1);
		_str->backIndex++;
		_str->elemCount++;
		return;
	}

	_str->data[_str->backIndex] = _data;
	grxSwapStringElement_Fast(_str, _str->backIndex, _str->backIndex - 1);
	_str->backIndex++;
	_str->elemCount++;
}

void grxAddStringFront(grxString* _str, char _data) {
	if(_data == NULL) {
		grxSetError(GRX_ERR_INVALID_VALUE);
		return;
	}

	if(_str->elemCount == INT64_MAX) {
		grxSetError(GRX_ERR_OVERFLOW);
		return;
	}

	if(_str->frontIndex == -1) {
		if((INT64_MAX - _str->elemAlloc) < GRX_STRING_ALLOC_SIZE) {
			grxSetError(GRX_ERR_OVERFLOW);
			return;
		}

		char* tempArray = _str->data;
		_str->data = Calloc(char, _str->elemAlloc + GRX_STRING_ALLOC_SIZE);
		MemCpyEx(_str->data, GRX_STRING_ALLOC_SIZE, tempArray, 0, sizeof(char), _str->elemCount);
		free(tempArray);

		_str->elemAlloc += GRX_STRING_ALLOC_SIZE;
		_str->frontIndex = GRX_STRING_ALLOC_SIZE - 1;
		_str->backIndex += GRX_STRING_ALLOC_SIZE;
		_str->data[_str->frontIndex] = _data; 
		_str->frontIndex--;
		_str->elemCount++;
		return;
	} 

	_str->data[_str->frontIndex] = _data;
	_str->frontIndex--;
	_str->elemCount++;
}

void grxRemoveStringBack(grxString* _str) {
	if(_str->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	if(_str->elemCount == 1)
		return;

	if((_str->elemAlloc - _str->elemCount) == GRX_STRING_ALLOC_SIZE) {
		char* tempArray = _str->data;
		_str->data = Calloc(char, _str->elemAlloc - GRX_STRING_ALLOC_SIZE);
		MemCpy(_str->data, tempArray, sizeof(char), _str->elemAlloc - GRX_STRING_ALLOC_SIZE);
		free(tempArray);

		_str->elemAlloc -= GRX_STRING_ALLOC_SIZE;
		_str->backIndex--;
		_str->elemCount--;
		return;
	}

	_str->backIndex--;
	_str->elemCount--;
}

void grxRemoveStringFront(grxString* _str) {
	if(_str->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	if(_str->elemCount == 1)
		return;

	if(_str->frontIndex == (GRX_STRING_ALLOC_SIZE - 1)) {
		char* tempArray = _str->data;
		_str->data = Calloc(char, _str->elemAlloc - GRX_STRING_ALLOC_SIZE);
		MemCpyEx(_str->data, 0, tempArray, GRX_STRING_ALLOC_SIZE, sizeof(char), _str->elemAlloc - GRX_STRING_ALLOC_SIZE);
		free(tempArray);

		_str->elemAlloc -= GRX_STRING_ALLOC_SIZE;
		_str->frontIndex = -1;
		_str->backIndex -= GRX_STRING_ALLOC_SIZE;
		_str->elemCount--;
		return;
	}

	_str->frontIndex++;
	_str->elemCount--;
}

bool grxCompareString(grxString* _strA, grxString* _strB) {
	if(_strA->elemCount == _strB->elemCount) {
		char aCh;
		char bCh;

		for(int64_t i = 0; i < _strA->elemCount; i++) {
			aCh = grxGetStringValue_Fast(_strA, i);
			bCh = grxGetStringValue_Fast(_strB, i);

			if(aCh != bCh) {
				return false;
			}
		}

		return true;
	}

	return false;
}

#define PushString(_expression) Push(grxString, _expression, grxDestroyString)

/****************************
 * String-specific functions 
 ****************************/
grxString* grxStringFromCSTR(const char _cstr[]) {
	if(_cstr == NULL) {
		grxSetError(GRX_ERR_INVALID_VALUE);
		return NULL;
	}

	grxString* _str = grxCreateString();
	uint32_t cstrLenght = strlen(_cstr) + 1;
	uint32_t blockCount = cstrLenght / GRX_STRING_ALLOC_SIZE;
	uint32_t remainderCount = cstrLenght % GRX_STRING_ALLOC_SIZE;

	if(remainderCount > 0) 
		blockCount++;
	
	_str->data = Calloc(char, blockCount * GRX_STRING_ALLOC_SIZE);
	MemCpy(_str->data, _cstr, sizeof(char), blockCount * GRX_STRING_ALLOC_SIZE);
	_str->backIndex = cstrLenght;
	_str->frontIndex = -1;
	_str->elemAlloc = blockCount * GRX_STRING_ALLOC_SIZE;
	_str->elemCount = cstrLenght;

	return _str;
}

void grxJoinStringBack(grxString* _str, grxString* _toJoin) {
	for(uint64_t i = 0; i < _toJoin->elemCount; i++)
		grxAddStringBack(_str, grxGetStringValue(_toJoin, i));
}

void grxJoinStringFront(grxString* _str, grxString* _toJoin) {
	for(uint64_t i = 0; i < _toJoin->elemCount; i++)
		grxAddStringFront(_str, grxGetStringValue(_toJoin, i));
}

void grxJointStringBack_CSTR(grxString* _str, const char* _toJoin) {
	grxString* str = grxStringFromCSTR(_toJoin);
	grxJoinStringBack(_str, str);
	grxDestroyString(&str);
}

void grxJointStringFront_CSTR(grxString* _str, const char* _toJoin) {
	grxString* str = grxStringFromCSTR(_toJoin);
	grxJoinStringFront(_str, str);
	grxDestroyString(&str);
}

void grxSkipString_ConsecutiveChars(grxString* _str, const char _toSkip[], int64_t* _start) {
	if(_start == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	for(int64_t start = *_start; start < _str->elemCount; start++) {
		if(strchr(_toSkip, grxGetStringValue(_str, start)) == NULL) {
			*_start = start;
			return;
		}
	}

	if((*_start) >= _str->elemCount) 
		grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
}

grxString* grxScanString_Until(grxString* _str, const char _scanUntil[], int64_t* _start) {
	grxString* result = grxCreateString();

	for(int64_t i = *_start; i < _str->elemCount; i++) {
		if(strchr(_scanUntil, grxGetStringValue(_str, i)) == NULL) {
			grxAddStringBack(result, grxGetStringValue_Fast(_str, i));
		}
		else {
			*_start = i + 1;
			return result;
		}
	}

	*_start = _str->elemCount;
	return result;
}

int64_t grxStringToInt(grxString* _str) {
	return _atoi64(grxGetStringNative(_str));
}

grxString* grxIntToString(int64_t _i) {
	char buffer[32];
	_i64toa(_i, buffer, 10);
	return grxStringFromCSTR(buffer);
}

double grxStringToDouble(grxString* _str) {
	double d;
	int result = sscanf(grxGetStringNative(_str), "%lf", &d);
	if(result == EOF) 
		grxSetError(GRX_ERR_SPECIFIC_ERROR);
	return d;
}

grxString* grxDoubleToString(double _d) {
	char buffer[32];
	int result = sprintf(buffer, "%lf", _d);
	if(result == EOF) {
		grxSetError(GRX_ERR_SPECIFIC_ERROR);
		return NULL;
	}
	else {
		return grxStringFromCSTR(buffer);
	}
}

uint64_t grxHashString(grxString* _str) {
	uint64_t result = 0;
	for(int64_t i = 0; i < _str->elemCount; i++) 
		result += grxGetStringValue(_str, i);
	return result;
}

bool grxIsStringNullOrEmpty(grxString* _str) {
	return (_str->data == NULL) || (_str->elemCount == 1);
}

#define PushStringFromCSTR(_cstr) Push(grxString, grxStringFromCSTR(_cstr), grxDestroyString)
