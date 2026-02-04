#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "../data-struct/string.h"
#include "../data-struct/map.h"
#include "../data-struct/deque.h"
#include "../data-struct/stack.h"

/************************************
 * Handles the registering and removal
 * of memory at the death of a scope. 
 ************************************/
typedef struct grxScopeElement {
	void* inst; 
	void(*dtor)(void**);
} grxScopeElement;

grxScopeElement* __grxCreateScopeElement(void* inst, void(*dtor)(void**)) {
	grxScopeElement* se = (grxScopeElement*) malloc(sizeof(grxScopeElement));
	se->inst = inst; 
	se->dtor = dtor;
	return se;
}

void __grxFreeScopeElement(grxScopeElement** se) {
	(*se)->dtor(&(*se)->inst);
	free(*se);
	*se = NULL;
}

void __grxKillScope(grxStack* stack) {
	#ifdef SCOPE_LIFETIME_DEBUG
		printf("\n\nCleaning the stack... \n");
	#endif

	while(grxGetStackCount(stack)) {
		grxScopeElement* se = (grxScopeElement*)grxGetStackTopValue(stack);
		#ifdef SCOPE_LIFETIME_DEBUG
			printf("%i %p\n", grxGetStackCount(stack), se->inst);
		#endif
		
		__grxFreeScopeElement(&se);
		grxPopStack(stack);
	}
}

void* __grxAddElementToScope(grxStack* stack, void* inst, void(*dtor)(void**)) {
	grxPushStack(stack, __grxCreateScopeElement(inst, dtor));
	#ifdef SCOPE_LIFETIME_DEBUG
		printf("\n\nAdded one element to free: \n%i %p", grxGetStackCount(stack), inst);
	#endif
	return inst;
}


typedef struct grxScopeNode {
	
} grxScopeNode;

/************************************
 * Handle stack tracing for debugging
 * and exception handling.
 ************************************/

grxStack* __grxGetStackTrace() {
	static grxStack* trace = NULL;

	if(trace == NULL) 
		trace = grxCreateStack();
	return trace;
}

void __grxStackTraceAdd(const char* funcName) {
	#ifdef ENABLE_STACK_TRACE
		grxStack* trace = __grxGetStackTrace();
		grxPushStack(trace, (void*)funcName);
	#else
		return;
	#endif
}

void __grxStackTraceRemove() {
	#ifdef ENABLE_STACK_TRACE
		grxStack* trace = __grxGetStackTrace();
		grxPopStack(trace);
	#else
		return;
	#endif
}

grxString* __grxGetStackTraceString() {
	grxString* result = grxCreateString();

	#ifdef ENABLE_STACK_TRACE
		grxStack* trace = __grxGetStackTrace();

		for(grxStackIterator it = grxGetStackIterator(trace); grxHasStackIteratorPrev(&it); grxStackIteratorPrev(&it)) {
			const char* funcName = (const char*) grxGetStackIteratorValue(&it);
			grxString* temp = grxCreateAndAssignString(funcName);
			grxStringJoinBack(result, temp);
			grxAddStringBack(result, '\n');
			grxFreeString(&temp);
		}
	#endif

	return result;
}

/************************************
 * Macros to ease the overhaul usage.
 ************************************/

#define Do \
{ \
	static bool __funcReflected = false;\
	if(__funcReflected == false) \
		__funcReflected = true;\
	__grxStackTraceAdd(__func__);\
	grxStack* __scopeStack = grxCreateStack(); \

#define SubDo \
{ \
	grxStack* __scopeStack = grxCreateStack();

#define Inst(type, expr, dtor) \
	(type*) __grxAddElementToScope(__scopeStack, expr, (void(*)(void**))dtor)

#define Return \
	__grxKillScope(__scopeStack);\
	__grxStackTraceRemove();\
	return

#define End }

#define EndSub \
	__grxKillScope(__scopeStack);\
}

#define StackTrace __grxGetStackTraceString()