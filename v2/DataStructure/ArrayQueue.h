#pragma once

#include "../Overhaul/Overhaul.h"
#include "Abstract.h"

#define GRX_ARRAYQUEUE_ALLOC_SIZE 10

/*****************************
 * Implementation of an array
 * queue in C.
 *****************************/
typedef struct grxArrayQueue {
	grxAbstract* data;
	int64_t headIndex; 
	int64_t tailIndex; 
	int64_t elemCount;
	int64_t elemAlloc;
} grxArrayQueue;

void grxCtorArrayQueue(grxArrayQueue* _arrQueue) {
	memset(_arrQueue, 0, sizeof(grxArrayQueue));
}

void grxDtorArrayQueue(grxArrayQueue* _arrQueue) {
	if(_arrQueue->data)
		free(_arrQueue->data);
}

grxArrayQueue* grxCreateArrayQueue() {
	grxArrayQueue* arrQueue = Malloc(grxArrayQueue, 1);
	grxCtorArrayQueue(arrQueue);
	return arrQueue;
}

void grxDestroyArrayQueue(grxArrayQueue** _arrQueue) {
	grxDtorArrayQueue(*_arrQueue);
	free(*_arrQueue);
	*_arrQueue = NULL;
}

void grxPushArrayQueueBack(grxArrayQueue* _arrQueue, grxAbstract _data) {
	if(_arrQueue->elemCount == INT64_MAX) {
		grxSetError(GRX_ERR_OVERFLOW);
		return;
	}
	
	if(_arrQueue->data == NULL) {
		_arrQueue->data = Calloc(grxAbstract, GRX_ARRAYQUEUE_ALLOC_SIZE);
		_arrQueue->elemAlloc = GRX_ARRAYQUEUE_ALLOC_SIZE;
		_arrQueue->elemCount = 1;
		_arrQueue->tailIndex = 1;
		_arrQueue->headIndex = -1;
		_arrQueue->data[0] = _data;
		return;
	}

	if((_arrQueue->headIndex % GRX_ARRAYQUEUE_ALLOC_SIZE) == 0) {
		if((INT64_MAX - _arrQueue->elemAlloc) < GRX_ARRAYQUEUE_ALLOC_SIZE) {
			grxSetError(GRX_ERR_OVERFLOW);
			return;
		}

		grxAbstract* tempArray = _arrQueue->data;
		_arrQueue->data = Calloc(grxAbstract, _arrQueue->elemAlloc + GRX_ARRAYQUEUE_ALLOC_SIZE);
		MemCpy(_arrQueue->data, tempArray, sizeof(grxAbstract), _arrQueue->elemAlloc);
		free(tempArray);

		_arrQueue->elemAlloc += GRX_ARRAYQUEUE_ALLOC_SIZE;
		_arrQueue->data[_arrQueue->tailIndex] = _data;
		_arrQueue->tailIndex++;
		_arrQueue->elemCount++;
		return;
	}

	_arrQueue->data[_arrQueue->tailIndex] = _data;
	_arrQueue->tailIndex++;
	_arrQueue->elemCount++;
}

void grxPopArrayQueueFront(grxArrayQueue* _arrQueue) {
	if(_arrQueue->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	if(_arrQueue->elemCount == 1) {
		free(_arrQueue->data);
		memset(_arrQueue, 0, sizeof(grxArrayQueue));
		return;
	}

	if(_arrQueue->headIndex == (GRX_ARRAYQUEUE_ALLOC_SIZE - 1)) {
		grxAbstract* tempArray = _arrQueue->data;
		_arrQueue->data = Calloc(grxAbstract, _arrQueue->elemAlloc - GRX_ARRAYQUEUE_ALLOC_SIZE);
		MemCpyEx(_arrQueue->data, 0, tempArray, GRX_ARRAYQUEUE_ALLOC_SIZE, sizeof(grxAbstract), _arrQueue->elemAlloc - GRX_ARRAYQUEUE_ALLOC_SIZE);
		free(tempArray);

		_arrQueue->elemAlloc -= GRX_ARRAYQUEUE_ALLOC_SIZE;
		_arrQueue->headIndex = -1;
		_arrQueue->tailIndex -= GRX_ARRAYQUEUE_ALLOC_SIZE;
		_arrQueue->elemCount--;
		return;
	}

	_arrQueue->headIndex++;
	_arrQueue->elemCount--;
}

int64_t grxGetArrayQueueCount(grxArrayQueue* _arrQueue) {
	return _arrQueue->elemCount;
}

grxAbstract* grxGetArrayQueueNative(grxArrayQueue* _arrQueue) {
	return &_arrQueue->data[_arrQueue->headIndex];
}

grxAbstract grxGetArrayQueueValue(grxArrayQueue* _arrQueue, int64_t _index) {
	if(_index < _arrQueue->elemCount) 
		return _arrQueue->data[_arrQueue->headIndex + _index];
	
	grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
	return grxNullAbstract();
}

grxAbstract grxGetArrayQueueValue_Fast(grxArrayQueue* _arrQueue, int64_t _index) {
	return _arrQueue->data[_index];
}

grxAbstract grxGetArrayQueueFront(grxArrayQueue* _arrQueue) {
	if(_arrQueue->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return grxNullAbstract();
	}

	return _arrQueue->data[_arrQueue->headIndex + 1];
}

grxAbstract grxGetArrayQueueBack(grxArrayQueue* _arrQueue) {
	if(_arrQueue->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return grxNullAbstract();
	}

	return _arrQueue->data[_arrQueue->tailIndex - 1];
}

void grxSetArrayQueueValue(grxArrayQueue* _arrQueue, int64_t _index, grxAbstract _data) {
	if(_index < _arrQueue->elemCount) 
		_arrQueue->data[_arrQueue->headIndex + _index] = _data;
	
	grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
}

void grxSetArrayQueueValue_Fast(grxArrayQueue* _arrQueue, int64_t _index, grxAbstract _data) {
	_arrQueue->data[_index] = _data;
}

void grxSetArrayQueueFront(grxArrayQueue* _arrQueue, grxAbstract _data) {
	if(_arrQueue->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	_arrQueue->data[_arrQueue->headIndex + 1] = _data;
}

void grxSetArrayQueueBack(grxArrayQueue* _arrQueue, grxAbstract _data) {
	if(_arrQueue->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	_arrQueue->data[_arrQueue->tailIndex - 1] = _data;
}
