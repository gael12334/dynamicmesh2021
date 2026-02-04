#pragma once 

#include "SLIterator.h"

typedef struct grxQueue {
	grxSLNode* front;
	grxSLNode* back;
	uint64_t elemCount;
} grxQueue;

void grxCtorQueue(grxQueue* _queue) {
	memset(_queue, 0, sizeof(grxQueue));
}

void grxDtorQueue(grxQueue* _queue) {
	grxSLNode* temp = NULL;

	while(_queue->front) {
		temp = _queue->front->prev;
		grxDestroySLNode(&_queue->front);
		_queue->front = temp;
	}

	memset(_queue, 0, sizeof(grxQueue));
}

grxQueue* grxCreateQueue() {
	grxQueue* queue = Malloc(grxQueue, 1);
	grxCtorQueue(queue);
	return queue;
}

void grxDestroyQueue(grxQueue** _queue) {
	grxDtorQueue(*_queue);
	free(*_queue);
	*_queue = NULL;
}

void grxQueuePush(grxQueue* _queue, grxAbstract _data) {
	if(_queue->elemCount == UINT64_MAX) {
		grxSetError(GRX_ERR_OVERFLOW);
		return;
	}

	if(_queue->back) 
		_queue->back = grxCreateSLNode(_data, _queue->back);
	else 
		_queue->back = _queue->front = grxCreateSLNode(_data, NULL);
	_queue->elemCount++;
}

void grxQueuePop(grxQueue* _queue) {
	if(_queue->front == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	if(_queue->front == _queue->back) {
		grxDestroySLNode(&_queue->front);
		_queue->back = NULL;
		_queue->elemCount = 0;
		return;
	}

	grxSLNode* temp = _queue->front->prev;
	grxDestroySLNode(&_queue->front);
	_queue->front = temp;
	_queue->elemCount--;
}

uint64_t grxGetQueueCount(grxQueue* _queue) {
	return _queue->elemCount;
}

grxAbstract grxGetQueueFrontValue(grxQueue* _queue) {
	if(_queue->front == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return grxNullAbstract();
	}

	return _queue->front->data;
}

grxAbstract grxGetQueueFrontValue_Fast(grxQueue* _queue) {
	return _queue->front->data;
}

grxSLIterator grxGetQueueFrontIterator(grxQueue* _queue) {
	grxSLIterator it;
	grxCtorSLIterator(&it, _queue->front);
	return it;
}

grxAbstract grxGetQueueBackValue(grxQueue* _queue) {
	if(_queue->back == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return grxNullAbstract();
	}

	return _queue->back->data;
}

grxAbstract grxGetQueueBackValue_Fast(grxQueue* _queue) {
	return _queue->back->data;
}

void grxSetQueueFrontValue(grxQueue* _queue, grxAbstract _data) {
	if(_queue->front == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	_queue->front->data = _data;
}

void grxSetQueueFrontValue_Fast(grxQueue* _queue, grxAbstract _data) {
	_queue->front->data = _data;
}

void grxSetQueueBackValue(grxQueue* _queue, grxAbstract _data) {
	if(_queue->back == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	_queue->back->data = _data;
}

void grxSetQueueBackValue_Fast(grxQueue* _queue, grxAbstract _data) {
	_queue->back->data = _data;
}