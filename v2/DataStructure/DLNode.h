#pragma once

#include "Abstract.h"
#include "../Overhaul/Overhaul.h"

typedef struct grxDLNode {
	grxAbstract data;
	struct grxDLNode* prev;
	struct grxDLNode* next;
} grxDLNode;

void grxCtorDLNode(grxDLNode* _node, grxAbstract _data, grxDLNode* _prev, grxDLNode* _next) {
	_node->data = _data;
	_node->prev = _prev;
	_node->next = _next;
}

void grxDtorDLNode(grxDLNode* _node) {
	// empty dtor.
}

grxDLNode* grxCreateDLNode(grxAbstract _data, grxDLNode* _prev, grxDLNode* _next) {
	grxDLNode* node = Malloc(grxDLNode, 1);
	grxCtorDLNode(node, _data, _prev, _next);
	return node;
}

void grxDestroyDLNode(grxDLNode** _node) {
	grxDtorDLNode(*_node);
	free(*_node);
	*_node = NULL;
}
