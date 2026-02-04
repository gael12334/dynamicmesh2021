#pragma once

#include "Abstract.h"
#include "../Overhaul/Overhaul.h"

typedef struct grxTLNode {
	grxAbstract data;
	struct grxTLNode* parent;
	struct grxTLNode* left;
	struct grxTLNode* right;
} grxTLNode;

void grxCtorTLNode(grxTLNode* _node, grxAbstract _data, grxTLNode* _parent, grxTLNode* _left, grxTLNode* _right) {
	_node->data = _data;
	_node->parent = _parent;
	_node->left = _left;
	_node->right = _right;
}

void grxDtorTLNode(grxTLNode* _node) {
	//empty dtor;
}

grxTLNode* grxCreateTLNode(grxAbstract _data, grxTLNode* _parent, grxTLNode* _left, grxTLNode* _right) {
	grxTLNode* node = Malloc(grxTLNode, 1);
	grxCtorTLNode(node, _data, _left, _right);
	return node;
}

void grxDestroyTLNode(grxTLNode** _node) {
	grxDtorTLNode(*_node);
	free(*_node);
	*_node = NULL;
}


