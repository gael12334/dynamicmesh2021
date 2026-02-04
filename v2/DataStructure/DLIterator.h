#pragma once

#include "DLNode.h"

typedef struct grxDLIterator {
	grxAbstract container;
	grxDLNode* node;
	grxDLNode* next;
} grxDLIterator;

void grxCtorDLIterator(grxDLIterator* _it, grxAbstract container, grxDLNode* _node) {
	_it->node = _node;
	_it->container = container;
	_it->next = NULL;
}

void grxDtorDLIterator(grxDLIterator* _it) {
	// empty dtor.
}

bool grxHasDLIteratorNext(grxDLIterator* _it) {
	if(_it->node) {
		_it->next = _it->node->next;
		return true;
	}

	return false;
}

bool grxHasDLIteratorPrev(grxDLIterator* _it) {
	if(_it->node) {
		_it->node = _it->node->prev;
		return true;
	}

	return false;
}

void grxDLIteratorNext(grxDLIterator* _it) {
	_it->node = _it->next;
}

void grxDLIteratorPrev(grxDLIterator* _it) {
	_it->node = _it->next;
}

grxAbstract grxGetDLIteratorValue(grxDLIterator* _it) {
	return _it->node->data;
}
