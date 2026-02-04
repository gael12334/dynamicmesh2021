#pragma once

#include "Abstract.h"
#include "../Overhaul/Overhaul.h"

typedef struct grxSLNode {
	grxAbstract data;
	struct grxSLNode* prev;
} grxSLNode;

void grxCtorSLNode(grxSLNode* _node, grxAbstract _data, grxSLNode* _prev) {
	_node->data = _data;
	_node->prev = _prev;
}

void grxDtorSLNode(grxSLNode* _node) {
	// empty dtor.
}

grxSLNode* grxCreateSLNode(grxAbstract _data, grxSLNode* _prev) {
	grxSLNode* node = Malloc(grxSLNode, 1);
	grxCtorSLNode(node, _data, _prev);
	return node;
}

void grxDestroySLNode(grxSLNode** _node) {
	grxDtorSLNode(*_node);
	free(*_node);
	*_node = NULL;
}