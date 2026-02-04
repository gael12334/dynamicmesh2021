#pragma once

#include "TLNode.h"
#include "Abstract.h"

typedef struct grxTLIterator {
	grxTLNode* node;
	grxTLNode* following;
} grxTLIterator;

void grxCtorTLIterator(grxTLIterator* _it, grxTLNode* _node) {
	_it->node = _node;
	_it->following = NULL;
}

void grxDtorTLIterator(grxTLIterator* _it) {
	//empty dtor.
}

bool grxTLIterator_HasParent(const grxTLIterator* _it) {
	static bool retVal; 
	retVal = (_it->node->parent != NULL);
	_it->following = (retVal ? _it->node->parent : NULL);
	return retVal;
}

bool grxTLIterator_HasLeft(grxTLIterator* _it) {
	static bool retVal; 
	retVal = (_it->node->left != NULL);
	_it->following = (retVal ? _it->node->left : NULL);
	return retVal;
}

bool grxTLIterator_HasRight(grxTLIterator* _it) {
	static bool retVal; 
	retVal = (_it->node->right != NULL);
	_it->following = (retVal ? _it->node->right : NULL);
	return retVal;
}

void grxTLIterator_Iterate(grxTLIterator* _it) {
	_it->node = (_it->following != NULL ? _it->following : _it->node);
}
