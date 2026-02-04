#pragma once

#include "GLLib.h"
#include "Vector.h"

#define GRX_PI 3.14159

typedef struct grxMatrix16 {
	double element[16]; 
} grxMatrix16;

void grxCtorMatrix16(grxMatrix16* _mat) {
	memset(_mat->element, 0, sizeof(grxMatrix16));
}

inline void grxDtorMatrix16(grxMatrix16* _mat) {
	// empty dtor.
}

grxMatrix16* grxCreateMatrix16() {
	grxMatrix16* mat = Malloc(grxMatrix16, 1);
	grxCtorMatrix16(mat);
	return mat;
}

void grxDestroyMatrix16(grxMatrix16** _mat) {
	grxDtorMatrix16(*_mat);
	free(*_mat);
	*_mat = NULL;
}

void grxLoadMatrix16_Null(grxMatrix16* _mat) {
	memset(_mat->element, 0, sizeof(grxMatrix16));
}

void grxLoadMatrix16_Identity(grxMatrix16* _mat) {
	grxLoadMatrix16_Null(_mat);
	for(int i = 0; i < 4; i++) 
		_mat->element[i * 5] = 1.0;
}

void grxLoadMatrix16_Translation(grxMatrix16* _mat, grxVector* _vec) {
	_mat->element[3] = _vec->x;
	_mat->element[7] = _vec->y;
	_mat->element[11] = _vec->z;
}

void grxLoadMatrix16_Reference(grxMatrix16* _mat, grxVector* _vec) {
	_mat->element[12] = _vec->x;
	_mat->element[13] = _vec->y;
	_mat->element[14] = _vec->z;
}

void grxLoadMatrix16_RotationX(grxMatrix16* _mat, double _angleRad) {
	_mat->element[0] = 1.0; _mat->element[1] = 0.0;            _mat->element[2] = 0.0;
	_mat->element[4] = 0.0; _mat->element[5] = cos(_angleRad); _mat->element[6] = -sin(_angleRad);
	_mat->element[8] = 0.0; _mat->element[9] = sin(_angleRad); _mat->element[10] = cos(_angleRad);
}

void grxLoadMatrix16_RotationY(grxMatrix16* _mat, double _angleRad) {
	_mat->element[0] = cos(_angleRad);  _mat->element[1] = 0.0; _mat->element[2] = sin(_angleRad);
	_mat->element[4] = 0.0;             _mat->element[5] = 1.0; _mat->element[6] = 0.0;
	_mat->element[8] = -sin(_angleRad); _mat->element[9] = 0.0; _mat->element[10] = cos(_angleRad);
}

void grxLoadMatrix16_RotationZ(grxMatrix16* _mat, double _angleRad) {
	_mat->element[0] = cos(_angleRad); _mat->element[1] = -sin(_angleRad); _mat->element[2] = 0.0;
	_mat->element[4] = sin(_angleRad); _mat->element[5] = cos(_angleRad);  _mat->element[6] = 0.0;
	_mat->element[8] = 0.0;            _mat->element[9] = 0.0;             _mat->element[10] = 1.0;
} 

void grxLoadMatrix16_ScalingFactor(grxMatrix16* _mat, double _factor) {
	memset(_mat->element, 0, sizeof(grxMatrix16));
	for(int i = 0; i < 4; i++) 
		_mat->element[i * 5] = _factor;
}

void grxMultMatrix16_Matrix(grxMatrix16* _result, grxMatrix16* _matA, grxMatrix16* _matB) {
	static grxMatrix16 mat; 

	mat.element[0] = _matA->element[0] * _matB->element[0] + _matA->element[1] * _matB->element[4] + _matA->element[2] * _matB->element[8];
	mat.element[1] = _matA->element[0] * _matB->element[1] + _matA->element[1] * _matB->element[5] + _matA->element[2] * _matB->element[9];
	mat.element[2] = _matA->element[0] * _matB->element[2] + _matA->element[1] * _matB->element[6] + _matA->element[2] * _matB->element[10];

	mat.element[4] = _matA->element[4] * _matB->element[0] + _matA->element[5] * _matB->element[4] + _matA->element[6] * _matB->element[8];
	mat.element[5] = _matA->element[4] * _matB->element[1] + _matA->element[5] * _matB->element[5] + _matA->element[6] * _matB->element[9];
	mat.element[6] = _matA->element[4] * _matB->element[2] + _matA->element[5] * _matB->element[6] + _matA->element[6] * _matB->element[10];

	mat.element[8] = _matA->element[8] * _matB->element[0] + _matA->element[9] * _matB->element[4] + _matA->element[10] * _matB->element[8];
	mat.element[9] = _matA->element[8] * _matB->element[1] + _matA->element[9] * _matB->element[5] + _matA->element[10] * _matB->element[9];
	mat.element[10]= _matA->element[8] * _matB->element[2] + _matA->element[9] * _matB->element[6] + _matA->element[10] * _matB->element[10];

	*_result = mat;
}

void grxMultMatrix16_Vector(grxVector* _result, grxMatrix16* _mat, grxVector* _vec) {
	static grxVector vec;
	static grxVector proc; 

	proc = *_vec;
	proc.x -= _mat->element[12];
	proc.y -= _mat->element[13];
	proc.z -= _mat->element[14];

	vec.x = _mat->element[0] * proc.x + _mat->element[1] * proc.y + _mat->element[2] * proc.z;
	vec.y = _mat->element[4] * proc.x + _mat->element[5] * proc.y + _mat->element[6] * proc.z;
	vec.z = _mat->element[8] * proc.x + _mat->element[9] * proc.y + _mat->element[10] * proc.z;

	vec.x += _mat->element[12] + _mat->element[3];
	vec.y += _mat->element[13] + _mat->element[7];
	vec.z += _mat->element[14] + _mat->element[11];
	*_result = vec;
}
