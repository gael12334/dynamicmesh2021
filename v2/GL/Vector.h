#pragma once

#include "GLLib.h"

typedef struct grxVector {
	double x, y, z;
} grxVector;

void grxCtorVector(grxVector* _vec, double _x, double _y, double _z) {
	_vec->x = _x;
	_vec->y = _y;
	_vec->z = _z;
}

void grxDtorVector(grxVector* _vec) {
	// empty dtor.
}

grxVector* grxCreateVector(double _x, double _y, double _z) {
	grxVector* point = Malloc(grxVector, 1);
	grxCtorVector(point, _x, _y, _z);
	return point;
}

grxVector* grxCopyVector(const grxVector* _vec) {
	grxVector* copy = Malloc(grxVector, 1);
	memcpy(copy, _vec, sizeof(grxVector));
	return copy;
}

void grxDestroyVector(grxVector** _vec) {
	grxDtorVector(*_vec);
	free(*_vec);
	*_vec = NULL;
}

double grxGetVectorNorm(const grxVector* _vec) {
	return sqrt(_vec->x * _vec->x + _vec->y * _vec->y + _vec->z * _vec->z);
}

double grxGetVectorDotProduct(const grxVector* _vecA, const grxVector* _vecB) {
	return _vecA->x * _vecB->x + _vecA->y * _vecB->y + _vecA->z * _vecB->z;
}

void grxGetVectorCrossProduct(grxVector* _result, const grxVector* _vecA, const grxVector* _vecB) {
	grxVector v;
	v.x = _vecA->y * _vecB->z - _vecA->z * _vecB->y;
	v.y = _vecA->x * _vecB->z - _vecA->z * _vecB->x;
	v.z = _vecA->y * _vecB->x - _vecA->x * _vecB->y;
	*_result = v;
}

inline double grxGetVectorAngle(const grxVector* _vecA, const grxVector* _vecB) {
	return acos(grxGetVectorDotProduct(_vecA, _vecB) / (grxGetVectorNorm(_vecA) * grxGetVectorNorm(_vecB)));
}

void grxAddVector(grxVector* _result, const grxVector* _vecA, const grxVector* _vecB) {
	grxVector v = *_vecA;
	v.x += _vecB->x;
	v.y += _vecB->y;
	v.z += _vecB->z;
	*_result = v;
}

void grxSubVector(grxVector* _result, const grxVector* _vecA, const grxVector* _vecB) {
	grxVector v = *_vecA;
	v.x -= _vecB->x;
	v.y -= _vecB->y;
	v.z -= _vecB->z;
	*_result = v;
}

void grxInverseVector(grxVector* _result, const grxVector* _vec) {
	grxVector v;
	v.x = -_vec->x;
	v.y = -_vec->y;
	v.z = -_vec->z;
	*_result = v;
}

void grxScaleVector(grxVector* _result, const grxVector* _vec, double _factor) {
	grxVector v;
	grxCtorVector(&v, 0.0, 0.0, 0.0);

	v.x = _vec->x * _factor;
	v.y = _vec->y * _factor;
	v.z = _vec->z * _factor;
	*_result = v;
}

void grxNormaliseVector(grxVector* _result, const grxVector* _vec) {
	static double norm;

	grxVector v;
	norm = grxGetVectorNorm(_vec);
	v.x = norm != 0.0 ? _vec->x / norm : 0.0;
	v.y = norm != 0.0 ? _vec->y / norm : 0.0;
	v.z = norm != 0.0 ? _vec->z / norm : 0.0;
	*_result = v;
}