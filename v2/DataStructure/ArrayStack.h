#pragma once

#include "Abstract.h"
#include "../Overhaul/Overhaul.h"

#define GRX_ARRAYSTACK_ALLOC_SIZE 10

typedef struct grxArrayStack {
	grxAbstract* data;
	uint64_t topIndex;
	uint64_t elemAlloc;
} grxArrayStack;

void grxCtorArrayStack(grxArrayStack* _arrStack) {
	memset(_arrStack, 0, sizeof(grxArrayStack));
}

void grxDtorArrayStack(grxArrayStack* _arrStack) {
	if(_arrStack->data)
		free(_arrStack->data);
}

grxArrayStack* grxCreateArrayStack() {
	grxArrayStack* arrStack = Malloc(grxArrayStack, 1);
	grxCtorArrayStack(arrStack);
	return arrStack;
}

void grxDestroyArrayStack(grxArrayStack** _arrStack) {
	grxDtorArrayStack(*_arrStack);
	free(*_arrStack);
	*_arrStack = NULL;
}

void grxPushArrayStack(grxArrayStack* _arrStack, grxAbstract _data) {
	if(_arrStack->data == NULL) {
		_arrStack->data = Calloc(grxAbstract, GRX_ARRAYSTACK_ALLOC_SIZE);
		_arrStack->topIndex = 1;
		_arrStack->elemAlloc = GRX_ARRAYSTACK_ALLOC_SIZE;
		_arrStack->data[0] = _data;
		return;
	}

	// to prevent an integer overflow.
	if(_arrStack->topIndex == UINT64_MAX) {
		grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
		return;
	}

	if((_arrStack->topIndex % GRX_ARRAYSTACK_ALLOC_SIZE) == 0) {
		if((INT64_MAX - _arrStack->elemAlloc) < GRX_ARRAYSTACK_ALLOC_SIZE) {
			grxSetError(GRX_ERR_OVERFLOW);
			return;
		}

		grxAbstract* tempArray = _arrStack->data;
		_arrStack->data = Calloc(grxAbstract, _arrStack->elemAlloc + GRX_ARRAYSTACK_ALLOC_SIZE);
		MemCpy(_arrStack->data, tempArray, sizeof(grxAbstract), _arrStack->elemAlloc);
		free(tempArray);

		_arrStack->elemAlloc += GRX_ARRAYSTACK_ALLOC_SIZE;
		_arrStack->data[_arrStack->topIndex] = _data;
		_arrStack->topIndex++;
		return;
	}

	_arrStack->data[_arrStack->topIndex] = _data;
	_arrStack->topIndex++;
}

void grxPopArrayStack(grxArrayStack* _arrStack) {
	if(_arrStack->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	if(_arrStack->topIndex == 1) {
		free(_arrStack->data);
		_arrStack->data = NULL;
		_arrStack->elemAlloc = 0;
		_arrStack->topIndex = 0;
		return;
	}

	if((_arrStack->elemAlloc - _arrStack->topIndex) == GRX_ARRAYSTACK_ALLOC_SIZE) {
		grxAbstract* tempArr = _arrStack->data;
		_arrStack->data = Calloc(grxAbstract, _arrStack->elemAlloc - GRX_ARRAYSTACK_ALLOC_SIZE);
		MemCpy(_arrStack->data, tempArr, sizeof(grxAbstract), _arrStack->elemAlloc - GRX_ARRAYSTACK_ALLOC_SIZE);
		free(tempArr);

		_arrStack->topIndex--;
		_arrStack->elemAlloc -= GRX_ARRAYSTACK_ALLOC_SIZE;
		return;
	}

	_arrStack->topIndex--;
}

uint64_t grxGetArrayStackCount(grxArrayStack* _arrStack) {
	return _arrStack->topIndex;
}

grxAbstract* grxGetArrayStackNative(grxArrayStack* _arrStack) {
	return _arrStack->data;
}

grxAbstract grxGetArrayStackTop(grxArrayStack* _arrStack) {
	if(_arrStack->data == NULL)
		return grxNullAbstract();
	return _arrStack->data[_arrStack->topIndex - 1];
}

grxAbstract grxGetArrayStackValue(grxArrayStack* _arrStack, uint64_t _index) {
	if(_index < _arrStack->topIndex) 
		return _arrStack->data[_index];
	
	grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
	return grxNullAbstract();
}

grxAbstract grxGetArrayStackValue_Fast(grxArrayStack* _arrStack, uint64_t _index) {
	return _arrStack->data[_index];
}

void grxSetArrayStackTop(grxArrayStack* _arrStack, grxAbstract _data) {
	if(_arrStack->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	_arrStack->data[_arrStack->topIndex - 1] = _data;
}

void grxSetArrayStackValue(grxArrayStack* _arrStack, uint64_t _index, grxAbstract _data) {
	if(_index < _arrStack->topIndex) 
		_arrStack->data[_index] = _data;
	
	grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
}

void grxSetArrayStackValue_Fast(grxArrayStack* _arrStack, uint64_t _index, grxAbstract _data) {
	_arrStack->data[_index] = _data;
}
