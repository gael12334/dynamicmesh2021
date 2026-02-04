#pragma once

#include "Vector.h"

typedef struct grxEdge {
	grxVector from;
	grxVector to;
} grxEdge; 

void grxCtorEdge(grxEdge* _edge, double x1, double y1, double z1, double x2, double y2, double z2) {
	grxCtorVector(&_edge->from, x1, y1, z1);
	grxCtorVector(&_edge->to, x2, y2, z2);
}

void grxCtorEdge_WithVector(grxEdge* _edge, grxVector* _from, grxVector* _to) {
	_edge->from = *_from; 
	_edge->to = *_to;
}

void grxDtorEdge(grxEdge* _edge) {
	// empty dtor.
}

grxEdge* grxCreateEdge(double x1, double y1, double z1, double x2, double y2, double z2) {
	grxEdge* edge = Malloc(grxEdge, 1);
	grxCtorEdge(edge, x1, y1, z1, x2, y2, z2);
	return edge;
}

grxEdge* grxCreateEdge_WithRadians(double x, double y, double z, double angleX, double angleY, double angleZ, double width) {
	static grxVector from;
	static grxVector to;
	static grxVector middle;
	from = (grxVector){-width / 2.0, 0.0, 0.0};
	to = (grxVector) {width / 2.0, 0.0, 0.0};
	middle = (grxVector) {x, y, z};

	static grxMatrix16 xRotMat;
	static grxMatrix16 yRotMat;
	static grxMatrix16 zRotMat;
	grxCtorMatrix16(&xRotMat);
	grxCtorMatrix16(&yRotMat);
	grxCtorMatrix16(&zRotMat);

	grxLoadMatrix16_RotationX(&xRotMat, angleX);
	grxLoadMatrix16_RotationY(&yRotMat, angleY);
	grxLoadMatrix16_RotationZ(&zRotMat, angleZ);

	grxMultMatrix16_Matrix(&xRotMat, &xRotMat, &yRotMat);
	grxMultMatrix16_Matrix(&xRotMat, &xRotMat, &zRotMat);

	grxMultMatrix16_Vector(&from, &xRotMat, &from);
	grxMultMatrix16_Vector(&to, &xRotMat, &to);

	grxAddVector(&from, &from, &middle);
	grxAddVector(&to, &to, &middle);

	return grxCreateEdge(from.x, from.y, from.z, to.x, to.y, to.z);
}

void grxDestroyEdge(grxEdge** _edge) {
	grxDtorEdge(*_edge);
	free(*_edge);
	*_edge = NULL;
}

grxVector grxGetEdge_MiddlePoint(grxEdge* _edge) {
	grxVector result;

	result.x = (_edge->to.x + _edge->from.x) / 2.0;
	result.y = (_edge->to.y + _edge->from.y) / 2.0;
	result.z = (_edge->to.z + _edge->from.z) / 2.0;

	return result;
}


