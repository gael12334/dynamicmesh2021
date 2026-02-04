#pragma once

#include "GLLib.h"

typedef struct grxPoint {
	double x, y;
} grxPoint;

void grxCtorPoint(grxPoint* _point, double _x, double _y) {
	_point->x = _x;
	_point->y = _y;
}

inline void grxDtorPoint(grxPoint* _point) {
	// empty dtor.
}

grxPoint* grxCreatePoint(double _x, double _y) {
	grxPoint* point = Malloc(grxPoint, 1);
	grxCtorPoint(point, _x, _y);
	return point;
}

void grxDestroyPoint(grxPoint** _point) {
	grxDtorPoint(*_point);
	free(*_point);
	*_point = NULL;
}