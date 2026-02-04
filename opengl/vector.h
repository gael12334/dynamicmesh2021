#pragma once

#include "includes.h"

typedef struct grx_GFXVector {
	double x, y, z;
} grx_GFXVector;

grx_GFXVector grxBuildVector(double x, double y, double z) {
	return (grx_GFXVector) {x, y, z};
}

grx_GFXVector* grxCreateVector(double x, double y, double z) {
	grx_GFXVector* vec = (grx_GFXVector*) malloc(sizeof(grx_GFXVector));
	*vec = grxBuildVector(x, y, z);
	return vec;
}

void grxFreeVector(grx_GFXVector** _vec) {
	free(*_vec);
	*_vec = NULL;
}

void grxAddVector(grx_GFXVector* _reciever, grx_GFXVector* _va, grx_GFXVector* _vb) {	
	_reciever->x = _va->x + _vb->x;	
	_reciever->y = _va->y + _vb->y;
	_reciever->z = _va->z + _vb->z;
}

void grxSubstractVector(grx_GFXVector* _reciever, grx_GFXVector* _va, grx_GFXVector* _vb) {	
	_reciever->x = _va->x - _vb->x;	
	_reciever->y = _va->y - _vb->y;
	_reciever->z = _va->z - _vb->z;
}

double grx




