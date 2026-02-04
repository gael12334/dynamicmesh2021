#pragma once 

#include "Vector.h"
#include "../Utilities/Convert.h"

typedef struct grxCamera {
	grxVector position;
	grxVector angles;
	grxVector front;
	grxVector right;
	grxVector up;
} grxCamera;

void grxCtorCamera(grxCamera* _cam) {
	memset(_cam, 0, sizeof(grxCamera));
}

void grxCtorCamera_WithComponents(grxCamera* _cam, const grxVector* _position, const grxVector* _angles) {
	static grxMatrix16 rot;
	_cam->angles = *_angles;
	_cam->position = *_position;
	_cam->front = (grxVector) {0.0, 0.0, 1.0};
	_cam->right = (grxVector) {-1.0, 0.0, 0.0};
	_cam->up = (grxVector) {0.0, 1.0, 0.0};

	grxLoadMatrix16_RotationX(&rot, -grxDegToRad(_angles->x));
	grxMultMatrix16_Vector(&_cam->front, &rot, &_cam->front);
	grxMultMatrix16_Vector(&_cam->right, &rot, &_cam->right);
	grxMultMatrix16_Vector(&_cam->up, &rot, &_cam->up);

	grxLoadMatrix16_RotationY(&rot, -grxDegToRad(_angles->y));
	grxMultMatrix16_Vector(&_cam->front, &rot, &_cam->front);
	grxMultMatrix16_Vector(&_cam->right, &rot, &_cam->right);
	grxMultMatrix16_Vector(&_cam->up, &rot, &_cam->up);

	grxLoadMatrix16_RotationZ(&rot, -grxDegToRad(_angles->z));
	grxMultMatrix16_Vector(&_cam->front, &rot, &_cam->front);
	grxMultMatrix16_Vector(&_cam->right, &rot, &_cam->right);
	grxMultMatrix16_Vector(&_cam->up, &rot, &_cam->up);
}

void grxCtorCamera_WithLookAtInfo(grxCamera* _cam, const grxVector* _position, const grxVector* _lookAt) {
	static grxVector delta;
	static double norm;
	static double invSqrt;

	grxSubVector(&delta, _lookAt, _position);
	norm = grxGetVectorNorm(&delta);
	invSqrt = 1.0 / norm;

	grxSubVector(&delta, _position, _lookAt);
	grxNormaliseVector(&_cam->front, &delta);
	_cam->angles.x = acos(_lookAt->x * invSqrt);
	_cam->angles.y = acos(_lookAt->y * invSqrt);
	_cam->angles.z = acos(_lookAt->z * invSqrt);
	_cam->position = *_position;
}

inline void grxDtorCamera(grxCamera* _cam) {
	//empty dtor.
}

grxCamera* grxCreateCamera() {
	grxCamera* cam = Malloc(grxCamera, 1);
	grxCtorCamera(cam);
	return cam;
}

grxCamera* grxCreateCamera_WithComponent(const grxVector* _position, const grxVector* _angles){
	grxCamera* cam = Malloc(grxCamera, 1);
	grxCtorCamera_WithComponents(cam, _position, _angles);
	return cam;
}

grxCamera* grxCreateCamera_WithLookAtInfo(const grxVector* _position, const grxVector* _lookAt) {
	grxCamera* cam = Malloc(grxCamera, 1);
	grxCtorCamera_WithLookAtInfo(cam, _position, _lookAt);
	return cam;
}

void grxDestroyCamera(grxCamera** _cam) {
	grxDtorCamera(*_cam);
	free(*_cam);
	*_cam = NULL;
}

void grxMoveCameraFoward(grxCamera* _cam, double _speed) {
	static grxVector scaledSpeed = {0.0, 0.0, 0.0};

	grxScaleVector(&scaledSpeed, &_cam->front, _speed);
	grxAddVector(&_cam->position, &_cam->position, &scaledSpeed);
}

void grxMoveCameraSideway(grxCamera* _cam, double _speed) {
	static grxVector scaledSpeed = {0.0, 0.0, 0.0};

	grxScaleVector(&scaledSpeed, &_cam->right, _speed);
	grxAddVector(&_cam->position, &_cam->position, &scaledSpeed);
}

void grxMoveCameraVertically(grxCamera* _cam, double _speed) {
	static grxVector scaledSpeed = {0.0, 0.0, 0.0};

	grxScaleVector(&scaledSpeed, &_cam->up, _speed);
	grxAddVector(&_cam->position, &_cam->position, &scaledSpeed);
}

void grxRotateCamera(grxCamera* _cam, const grxVector* _angles) {
	grxSubVector(&_cam->angles, &_cam->angles, _angles);
	grxCtorCamera_WithComponents(_cam, &_cam->position, &_cam->angles);
}

void grxCameraLookAt(grxCamera* _cam, const grxVector* _lookAt) {
	grxCtorCamera_WithLookAtInfo(_cam, &_cam->position, _lookAt);
}

