#pragma once

#include "Abstract.h"
#include "SLNode.h"
#include "SLIterator.h"

typedef struct grxStack {
	grxSLNode* top;
	uint64_t elemCount;
} grxStack;

void grxCtorStack(grxStack* _stack) {
	memset(_stack, 0, sizeof(grxStack));
}

void grxDtorStack(grxStack* _stack) {
	grxSLNode* temp = NULL;
	while(_stack->top) {
		temp = _stack->top->prev;
		grxDestroySLNode(&_stack->top);
		_stack->top = temp;
	}
}

grxStack* grxCreateStack() {
	grxStack* stack = Malloc(grxStack, 1);
	grxCtorStack(stack);
	return stack;
}

void grxDestroyStack(grxStack** _stack) {
	grxDtorStack(*_stack);
	free(*_stack);
	*_stack = NULL;
}

void grxPushStack(grxStack* _stack, grxAbstract _data) {
	if(_stack->elemCount == UINT64_MAX) {
		grxSetError(GRX_ERR_OVERFLOW);
		return;
	}

	if(_stack->top)
		_stack->top = grxCreateSLNode(_data, _stack->top);
	else 
		_stack->top = grxCreateSLNode(_data, NULL);
	_stack->elemCount++;
}

void grxPopStack(grxStack* _stack) {
	if(_stack->top == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	grxSLNode* node = _stack->top->prev;
	grxDestroySLNode(&_stack->top);
	_stack->top = node;
	_stack->elemCount--;
}

uint64_t grxGetStackCount(grxStack* _stack) {
	return _stack->elemCount;
}

grxSLIterator grxGetStackTopIterator(grxStack* _stack) {
	grxSLIterator it;
	grxCtorSLIterator(&it, _stack->top);
	return it;
}

grxAbstract grxGetStackTopValue(grxStack* _stack) {
	if(_stack->top == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return grxNullAbstract();
	}

	return _stack->top->data;
}

grxAbstract grxGetStackTopValue_Fast(grxStack* _stack) {
	return _stack->top->data;
}

void grxSetStackTopValue(grxStack* _stack, grxAbstract _data) {
	if(_stack->top == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	_stack->top->data = _data;
}

void grxSetStackTopValue_Fast(grxStack* _stack, grxAbstract _data) {
	_stack->top->data = _data;
}
