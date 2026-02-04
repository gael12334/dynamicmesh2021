#pragma once

#include "SLNode.h"

typedef struct grxSLIterator {
	grxSLNode* node;
	grxSLNode* next;
} grxSLIterator;

void grxCtorSLIterator(grxSLIterator* _it, grxSLNode* _startNode) {
	_it->node = _startNode;
	_it->next = NULL;
}

void grxDtorSLIterator(grxSLIterator* _it) {
	// empty dtor.
}

bool grxHasSLIteratorNext(grxSLIterator* _it) {
	if(_it->node == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return false;
	}
	
	if(_it->node->prev) {
		_it->next = _it->node->prev;
		return true;
	}

	return false;
}

void grxSLIteratorNext(grxSLIterator* _it) {
	if((_it->node == NULL) || (_it->next == NULL)) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}
	
	_it->node = _it->next;
	_it->next = NULL;
}

grxAbstract grxGetSLIteratorValue(grxSLIterator* _it) {
	if(_it->node == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return grxNullAbstract();
	}

	return _it->node->data;
}