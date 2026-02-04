#pragma once

#include "CLib.h"

/**********************************
 * Contains the scoped-pointer
 * to delete at the end of the scope's
 * lifetime.
 **********************************/
typedef struct grxScopeObjectInfo {
	void* inst; 
	void(*destroyer)(void**);
} grxScopeObjectInfo; 

void grxCtorScopeObjectInfo(grxScopeObjectInfo* _info, void* _inst, void(*_destroyer)(void**)) {
	_info->inst = _inst;
	_info->destroyer = _destroyer;
}

void grxDtorScopeObjectInfo(grxScopeObjectInfo* _info) {
	// Empty
}

grxScopeObjectInfo* grxCreateScopeObjectInfo(void* _inst, void(*_destroyer)(void**)) {
	grxScopeObjectInfo* info = (grxScopeObjectInfo*) malloc(sizeof(grxScopeObjectInfo));
	grxCtorScopeObjectInfo(info, _inst, _destroyer);
	return info;
}

void grxDestroyScopeObjectInfo(grxScopeObjectInfo** _info) {
	grxDtorScopeObjectInfo(*_info);
	free(*_info);
	*_info = NULL;
}

/**********************************
 * The Simply-Linked node for the 
 * scope stack.
 **********************************/
typedef struct grxScopeStackNode {
	void* data;
	struct grxScopeStackNode* prev;
} grxScopeStackNode;

void grxCtorScopeStackNode(grxScopeStackNode* _node, void* _data, grxScopeStackNode* _prev) {
	*_node = (grxScopeStackNode) {_data, _prev};
}

void grxDtorScopeStackNode(grxScopeStackNode* _node) { 
	grxScopeObjectInfo* info = (grxScopeObjectInfo*) _node->data;
	grxDestroyScopeObjectInfo(&info);
}

grxScopeStackNode* grxCreateScopeStackNode(void* _data, grxScopeStackNode* _prev) {
	grxScopeStackNode* node = (grxScopeStackNode*) malloc(sizeof(grxScopeStackNode));
	grxCtorScopeStackNode(node, _data, _prev);
	return node;
}

void grxDestroyScopeStackNode(grxScopeStackNode** _node) {
	grxDtorScopeStackNode(*_node);
	free(*_node);
	  	*_node = NULL;
}

/**********************************
 * A scope stack for auto deletion 
 * of scope-lived pointers.
 **********************************/
typedef struct grxScopeStack {
	grxScopeStackNode* top;
} grxScopeStack;

void grxCtorScopeStack(grxScopeStack* _stack, grxScopeStackNode* _top) {
	_stack->top = _top;
}

void grxDtorScopeStack(grxScopeStack* _stack) {
	grxScopeStackNode* node = _stack->top;
	grxScopeStackNode* temp = NULL;
	while(node != NULL) {
		temp = node->prev;
		grxDestroyScopeStackNode(&node);
		node = temp;
	}
}

grxScopeStack* grxCreateScopeStack() {
	grxScopeStack* stack = (grxScopeStack*) malloc(sizeof(grxScopeStack));
	grxCtorScopeStack(stack, NULL); 
	return stack;
}

void grxDestroyScopeStack(grxScopeStack** _stack) {
	grxDtorScopeStack(*_stack);
	free(*_stack);
	*_stack = NULL;
}

void* grxPushScopeStackElement(grxScopeStack* _stack, void* _inst, void(*_destroyer)(void**)) {
	_stack->top = grxCreateScopeStackNode(grxCreateScopeObjectInfo(_inst, _destroyer), _stack->top);
	return _inst;
}

void grxPopScopeStackElement(grxScopeStack* _stack) {
	if(_stack->top) {
		grxScopeStackNode* temp = _stack->top->prev;
		grxDestroyScopeObjectInfo((grxScopeObjectInfo**)&_stack->top->data);
		grxDestroyScopeStackNode(&_stack->top);
		_stack->top = temp;
	}
}

/**********************************
 * The unique function name stack.
 **********************************/
grxScopeStack* grxGetUniqueScopeStack() {
	static grxScopeStack* stack = NULL;
	if(stack == NULL) 
		stack = grxCreateScopeStack();

	return stack;
}

/**********************************
 * Macros for the ease of use.
 **********************************/
#define Do { grxScopeStack* __grx_oss = grxCreateScopeStack();

#define Push(_type, _expr, _dtor) (_type*) grxPushScopeStackElement(__grx_oss, _expr, (void(*)(void**))_dtor)

#define EndDo grxDestroyScopeStack(&__grx_oss); }

#define Return grxDestroyScopeStack(&__grx_oss); return

#define End }
