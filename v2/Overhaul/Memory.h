#pragma once

#include "CLib.h"

/****************************
 * Marcos for the ease to use
 * of memory management
 * functions.
 ****************************/

#define Malloc(_type, _count) \
	(_type*) malloc(sizeof(_type) * (_count))

#define Calloc(_type, _count) \
	(_type*) calloc(_count, sizeof(_type))

#define ReAlloc(_type, _count, _ptr) \
	_ptr = (_type*) realloc(_ptr, sizeof(_type) * (_count))

#define MemCpy(_dest, _src, _elemSize, _elemCount) \
	memcpy(_dest, _src, (_elemSize) * (_elemCount))

#define MemCpyEx(_destPtr, _destStart, _srcPtr, _srcStart, _elemSize, _elemCount) \
	memcpy(&_destPtr[_destStart], &_srcPtr[_srcStart], (_elemSize) * (_elemCount))

