#pragma once

#include "Camera.h"
#include "GLLib.h"
#include "Point.h"

typedef struct grxViewStruct {
	double left, right, top, bottom, nearP, farP;
} grxViewStruct;

void grxCtorViewStruct(grxViewStruct* _view, double _left, double _right, double _top, double _bottom, double _nearP, double _farP) {
	_view->top = _top;
	_view->bottom = _bottom;
	_view->farP = _farP;
	_view->nearP = _nearP;
	_view->right = _right;
	_view->left = _left;
}

void grxDtorViewStruct(grxViewStruct* _view) {
	//empty dtor.
}