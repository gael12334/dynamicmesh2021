#pragma once

#include "GLLib.h"
#include "Point.h"

typedef struct grxRect {
	double x, y, w, h;
} grxRect;

void grxCtorRect(grxRect* _rect, double _x, double _y, double _w, double _h) {
	_rect->x = _x;
	_rect->y = _y;
	_rect->w = _w;
	_rect->h = _h;
}

inline void grxDtorRect(grxRect* _rect) {
	// empty dtor.
}

grxRect* grxCreateRect(double _x, double _y, double _w, double _h) {
	grxRect* rect = Malloc(grxRect, 1);
	grxCtorRect(rect, _x, _y, _w, _h);
	return rect;
}

void grxDestroyRect(grxRect** _rect) {
	grxDtorRect(*_rect);
	free(*_rect);
	*_rect = NULL;
}

