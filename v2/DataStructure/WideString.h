#pragma once

#include "Abstract.h"
#include "../Overhaul/Overhaul.h"

#define GRX_STRING_ALLOC_SIZE 10

/************************
 * Implementation of an
 * string in C.
 ************************/
typedef struct grxWideString {
	wchar_t* data;
	int64_t elemCount;
	int64_t elemAlloc;	
	int64_t frontIndex;
	int64_t backIndex;
} grxWideString;

void grxCtorWideString(grxWideString* _str) {
	_str->data = Calloc(wchar_t, GRX_STRING_ALLOC_SIZE); 
		_str->elemAlloc = GRX_STRING_ALLOC_SIZE;
		_str->frontIndex = -1;
		_str->backIndex = 0;
		_str->data[_str->backIndex] = '\0';
		_str->backIndex++;
		_str->elemCount = 1;
}

void grxDtorWideString(grxWideString* _str) {
	if(_str->data) 
		free(_str->data);
}

grxWideString* grxCreateWideString() {
	grxWideString* str = Malloc(grxWideString, 1);
	grxCtorWideString(str);
	return str;
}

void grxDestroyWideString(grxWideString** _str) {
	grxDtorWideString(*_str);
	free(*_str);
}

int64_t grxGetWideStringCount(grxWideString* _str) {
	return _str->elemCount;
}

wchar_t grxGetWideStringValue(grxWideString* _str, int64_t _index) {
	if((_index > 0) && (_index < _str->elemCount)) 
		return _str->data[_str->frontIndex + 1 + _index];
	
	grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
	return NULL;
}

wchar_t grxGetWideStringValue_Fast(grxWideString* _str, int64_t _index) {
	return _str->data[_str->frontIndex + 1 + _index];
}

wchar_t* grxGetWideStringNative(grxWideString* _str) {
	return &_str->data[_str->frontIndex + 1]; // Or potentially "_str->data + _str->frontIndex + 0x1"
}

wchar_t grxGetWideStringFirst(grxWideString* _str) {
	if(_str->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return NULL;
	}

	return _str->data[_str->frontIndex + 1];
}

wchar_t grxGetWideStringLast(grxWideString* _str) {
	if(_str->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return NULL;
	}

	return _str->data[_str->backIndex - 1];
}

void grxSetWideStringValue(grxWideString* _str, int64_t _index, wchar_t _data) {
	if((_index > 0) && (_index < _str->elemCount)) 
		_str->data[_str->frontIndex + 1 + _index] = _data;
	
	else 
		grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
}

void grxSetWideStringValue_Fast(grxWideString* _str, int64_t _index, wchar_t _data) {
	_str->data[_str->frontIndex + 1 + _index] = _data;
}

void grxSetWideStringFirst(grxWideString* _str, wchar_t _data) {
	if(_str->data == NULL) 
		grxSetError(GRX_ERR_NULL_REFERENCE);

	_str->data[_str->frontIndex + 1] = _data;
}

void grxSetWideStringLast(grxWideString* _str, wchar_t _data) {
	if(_str->data == NULL) 
		grxSetError(GRX_ERR_NULL_REFERENCE);

	_str->data[_str->backIndex - 1] = _data;
}

void grxSwapWideStringElement(grxWideString* _str, int64_t _src, int64_t _dest) {
	if(_str->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}
	
	if((_src < _str->elemCount) && (_str->elemCount) && (_src > 0) && (_dest > 0)) {
		wchar_t a = grxGetWideStringValue_Fast(_str, _src);
		wchar_t b = grxGetWideStringValue_Fast(_str, _dest);
		grxSetWideStringValue_Fast(_str, _src, b);
		grxSetWideStringValue_Fast(_str, _dest, a);
		return;
	}

	grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
}

void grxSwapWideStringElement_Fast(grxWideString* _str, int64_t _src, int64_t _dest) {
	wchar_t a = grxGetWideStringValue_Fast(_str, _src);
	wchar_t b = grxGetWideStringValue_Fast(_str, _dest);
	grxSetWideStringValue_Fast(_str, _src, b);
	grxSetWideStringValue_Fast(_str, _dest, a);
}

void grxAddWideStringBack(grxWideString* _str, wchar_t _data) {
	if(_str->elemCount == INT64_MAX) {
		grxSetError(GRX_ERR_OVERFLOW);
		return;
	}

	if((_str->backIndex % GRX_STRING_ALLOC_SIZE) == 0) {
		if((INT64_MAX - _str->elemAlloc) < GRX_STRING_ALLOC_SIZE) {
			grxSetError(GRX_ERR_OVERFLOW);
			return;
		}

		wchar_t* tempArray = _str->data;
		_str->data = Calloc(wchar_t, _str->elemAlloc + GRX_STRING_ALLOC_SIZE);
		MemCpy(_str->data, tempArray, sizeof(wchar_t), _str->elemAlloc);
		free(tempArray);

		_str->elemAlloc += GRX_STRING_ALLOC_SIZE;
		_str->data[_str->backIndex] = _data;
		grxSwapWideStringElement_Fast(_str, _str->backIndex, _str->backIndex - 1);
		_str->backIndex++;
		_str->elemCount++;
		return;
	}

	_str->data[_str->backIndex] = _data;
	grxSwapWideStringElement_Fast(_str, _str->backIndex, _str->backIndex - 1);
	_str->backIndex++;
	_str->elemCount++;
}

void grxAddWideStringFront(grxWideString* _str, wchar_t _data) {
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

		wchar_t* tempArray = _str->data;
		_str->data = Calloc(wchar_t, _str->elemAlloc + GRX_STRING_ALLOC_SIZE);
		MemCpyEx(_str->data, GRX_STRING_ALLOC_SIZE, tempArray, 0, sizeof(wchar_t), _str->elemCount);
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

void grxRemoveWideStringBack(grxWideString* _str) {
	if(_str->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	if(_str->elemCount == 1)
		return;

	if((_str->elemAlloc - _str->elemCount) == GRX_STRING_ALLOC_SIZE) {
		wchar_t* tempArray = _str->data;
		_str->data = Calloc(wchar_t, _str->elemAlloc - GRX_STRING_ALLOC_SIZE);
		MemCpy(_str->data, tempArray, sizeof(wchar_t), _str->elemAlloc - GRX_STRING_ALLOC_SIZE);
		free(tempArray);

		_str->elemAlloc -= GRX_STRING_ALLOC_SIZE;
		_str->backIndex--;
		_str->elemCount--;
		return;
	}

	_str->backIndex--;
	_str->elemCount--;
}

void grxRemoveWideStringFront(grxWideString* _str) {
	if(_str->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	if(_str->elemCount == 1)
		return;

	if(_str->frontIndex == (GRX_STRING_ALLOC_SIZE - 1)) {
		wchar_t* tempArray = _str->data;
		_str->data = Calloc(wchar_t, _str->elemAlloc - GRX_STRING_ALLOC_SIZE);
		MemCpyEx(_str->data, 0, tempArray, GRX_STRING_ALLOC_SIZE, sizeof(wchar_t), _str->elemAlloc - GRX_STRING_ALLOC_SIZE);
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

bool grxCompareWideString(grxWideString* _strA, grxWideString* _strB) {
	if(_strA->elemCount == _strB->elemCount) {
		wchar_t aCh;
		wchar_t bCh;

		for(int64_t i = 0; i < _strA->elemCount; i++) {
			aCh = grxGetWideStringValue_Fast(_strA, i);
			bCh = grxGetWideStringValue_Fast(_strB, i);

			if(aCh != bCh) {
				return false;
			}
		}

		return true;
	}

	return false;
}

#define PushWideString(_expression) Push(grxWideString, _expression, grxDestroyWideString)

/****************************
 * WideString-specific functions 
 ****************************/
grxWideString* grxWideStringFromCSTR(const wchar_t _cstr[]) {
	if(_cstr == NULL) {
		grxSetError(GRX_ERR_INVALID_VALUE);
		return NULL;
	}

	grxWideString* _str = grxCreateWideString();
	uint32_t cstrLenght = wcslen(_cstr) + 1;
	uint32_t blockCount = cstrLenght / GRX_STRING_ALLOC_SIZE;
	uint32_t remainderCount = cstrLenght % GRX_STRING_ALLOC_SIZE;

	if(remainderCount > 0) 
		blockCount++;
	
	_str->data = Calloc(wchar_t, blockCount * GRX_STRING_ALLOC_SIZE);
	MemCpy(_str->data, _cstr, sizeof(wchar_t), blockCount * GRX_STRING_ALLOC_SIZE);
	_str->backIndex = cstrLenght;
	_str->frontIndex = -1;
	_str->elemAlloc = blockCount * GRX_STRING_ALLOC_SIZE;
	_str->elemCount = cstrLenght;

	return _str;
}

void grxJoinWideStringBack(grxWideString* _str, grxWideString* _toJoin) {
	for(uint64_t i = 0; i < _toJoin->elemCount; i++)
		grxAddWideStringBack(_str, grxGetWideStringValue(_toJoin, i));
}

void grxJoinWideStringFront(grxWideString* _str, grxWideString* _toJoin) {
	for(uint64_t i = 0; i < _toJoin->elemCount; i++)
		grxAddWideStringFront(_str, grxGetWideStringValue(_toJoin, i));
}

void grxJointWideStringBack_CSTR(grxWideString* _str, const wchar_t* _toJoin) {
	grxWideString* str = grxWideStringFromCSTR(_toJoin);
	grxJoinWideStringBack(_str, str);
	grxDestroyWideString(&str);
}

void grxJointWideStringFront_CSTR(grxWideString* _str, const wchar_t* _toJoin) {
	grxWideString* str = grxWideStringFromCSTR(_toJoin);
	grxJoinWideStringFront(_str, str);
	grxDestroyWideString(&str);
}

void grxSkipWideString_ConsecutiveChars(grxWideString* _str, const wchar_t _toSkip[], int64_t* _start) {
	if(_start == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	for(int64_t start = *_start; start < _str->elemCount; start++) {
		if(wcschr(_toSkip, grxGetWideStringValue(_str, start)) == NULL) {
			*_start = start;
			return;
		}
	}

	if((*_start) >= _str->elemCount) 
		grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
}

grxWideString* grxScanWideString_Until(grxWideString* _str, const wchar_t _scanUntil[], int64_t* _start) {
	grxWideString* result = grxCreateWideString();

	for(int64_t i = *_start; i < _str->elemCount; i++) {
		if(wcschr(_scanUntil, grxGetWideStringValue(_str, i)) == NULL) {
			grxAddWideStringBack(_str, grxGetWideStringValue_Fast(_str, i));
		}
		else {
			*_start = i;
			return result;
		}
	}

	*_start = _str->elemCount;
	return result;
}

int64_t grxWideStringToInt(grxWideString* _str) {
	int64_t i;
	int result = swscanf(grxGetWideStringNative(_str), L"%li", &i);
	if(result == EOF) {
		grxSetError(GRX_ERR_SPECIFIC_ERROR);
		return 0;
	}
	else {
		return i;
	}
}

grxWideString* grxIntToWideString(int64_t _i) {
	wchar_t buffer[32];
	int result = swprintf(buffer, L"%li", _i);
	if(result == EOF) {
		grxSetError(GRX_ERR_SPECIFIC_ERROR);
		return NULL;
	}
	else {
		return grxWideStringFromCSTR(buffer);
	}
}

double grxWideStringToDouble(grxWideString* _str) {
	double d;
	int result = swscanf(grxGetWideStringNative(_str), L"%lf", &d);
	if(result == EOF) {
		grxSetError(GRX_ERR_SPECIFIC_ERROR);
		return 0;
	} 
	else {
		return d;
	}
}

grxWideString* grxDoubleToWideString(double _d) {
	wchar_t buffer[32];
	int result = swprintf(buffer, 32, L"%lf", _d);
	if(result == EOF) {
		grxSetError(GRX_ERR_SPECIFIC_ERROR);
		return NULL;
	}
	else 
		return grxWideStringFromCSTR(buffer); 
}

uint64_t grxHashWideString(grxWideString* _str) {
	uint64_t result = 0;
	for(int64_t i = 0; i < _str->elemCount; i++) 
		result += grxGetWideStringValue(_str, i);
	return result;
}

#define PushWideStringFromCSTR(_cstr) Push(grxWideString, grxWideStringFromCSTR(_cstr), grxDestroyWideString)
