#pragma once

#include "Abstract.h"
#include "../Overhaul/Overhaul.h"

/****************************
 * An array of void* to 
 * contain any primitive 
 * type.
 ****************************/
typedef struct grxArray {
	grxAbstract* data;
	uint32_t count;
} grxArray;

void grxCtorArray(grxArray* _arr, uint32_t _count) {
	if(_count == 0) {
		_arr->data = NULL;
		_arr->count = 0;
		grxSetError(GRX_ERR_INVALID_VALUE);
		return;
	}
	_arr->data = Calloc(grxAbstract, _count);
	_arr->count = _count;
}

void grxDtorArray(grxArray* _arr) {
	free(_arr->data);
}

grxArray* grxCreateArray(uint32_t _count) {
	grxArray* arr = Malloc(grxArray, 1);
	grxCtorArray(arr, _count);
	return arr;
}

grxArray* grxCopyArray(grxArray* _arr) {
	grxArray* copy = Malloc(grxArray, _arr->count);
	MemCpy(copy->data, _arr->data, sizeof(grxAbstract), _arr->count);
	copy->count = _arr->count;
	return copy;
}

void grxDestroyArray(grxArray** _arr) {
	grxDtorArray(*_arr);
	free(*_arr);
	*_arr = NULL;
}

grxAbstract grxGetArrayValue(grxArray* _arr, uint32_t _index) {
	if(_index < _arr->count)
		return _arr->data[_index];
	
	grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
	return grxNullAbstract();
}

grxAbstract grxGetArrayValue_Fast(grxArray* _arr, uint32_t _index) {
	return _arr->data[_index];
}

grxAbstract* grxGetArrayNative(grxArray* _arr) {
	return _arr->data;
}

uint32_t grxGetArrayCount(grxArray* _arr) {
	return _arr->count;
}

void grxSetArrayValue(grxArray* _arr, uint32_t _index, grxAbstract _data) {
	if(_index < _arr->count)
		_arr->data[_index] = _data;
	
	grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
}

void grxSetArrayValue_Fast(grxArray* _arr, uint32_t _index, grxAbstract _data) {
	_arr->data[_index] = _data;
}

