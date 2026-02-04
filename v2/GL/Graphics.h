#pragma once

#include <stdarg.h>
#include "GLLib.h" 
#include "ViewStruct.h"
#include "Camera.h"

typedef enum grxPerspectiveMode {
	GRX_PM_ORTHO,
	GRX_PM_PERSPECTIVE
} grxPerspectiveMode;

typedef struct grxGraphicsInfo {
	grxViewStruct ortho;
	grxViewStruct perspective;
	grxCamera orthoCam;
	grxCamera perspectiveCam;
	grxPerspectiveMode mode;
} grxGraphicsInfo;

void grxCtorGraphicsInfo(grxGraphicsInfo* _info, grxViewStruct* _ortho, grxViewStruct* _perspective, grxCamera* _orthoCam, grxCamera* _perspectiveCam) {
	_info->ortho = *_ortho;
	_info->orthoCam = *_orthoCam;
	_info->perspective = *_perspective;
	_info->perspectiveCam = *_perspectiveCam;
	_info->mode = GRX_PM_ORTHO;
}

inline void grxDtorGraphicsInfo(grxGraphicsInfo* _info) {
	// empty dtor.
}

void grxGraphics_ApplyOrthogonalView(grxGraphicsInfo* _info ) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(_info->ortho.left, _info->ortho.right,  _info->ortho.bottom, _info->ortho.top, _info->ortho.nearP, _info->ortho.farP);
	glTranslated(_info->orthoCam.position.x, _info->orthoCam.position.y, _info->orthoCam.position.z);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void grxGraphics_ApplyPerspectiveView(grxGraphicsInfo* _info) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(_info->perspective.left, _info->perspective.right,  _info->perspective.bottom, _info->perspective.top, _info->perspective.nearP, _info->perspective.farP);
	glRotated(_info->perspectiveCam.angles.x, 1.0, 0.0, 0.0);
	glRotated(_info->perspectiveCam.angles.y, 0.0, 1.0, 0.0);
	glRotated(_info->perspectiveCam.angles.z, 0.0, 0.0, 1.0);
	glTranslated(_info->perspectiveCam.position.x, _info->perspectiveCam.position.y, _info->perspectiveCam.position.z);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void grxGraphics_ChangeState(bool _enable, uint16_t _argc, ...) {
	static uint16_t i;
	static va_list argv;

	va_start(argv, _argc);
	for(i = 0; i < _argc; i++) {
		_enable ? 
			glEnable(va_arg(argv, uint32_t)) : 
			glDisable(va_arg(argv, uint32_t));
	}
	va_end(argv);
}

void grxGraphics_ChangeClientState(bool _enable, uint16_t _argc, ...) {
	static uint16_t i;
	static va_list argv;

	va_start(argv, _argc);
	for(i = 0; i < _argc; i++) {
		_enable ? 
			glEnableClientState(va_arg(argv, uint32_t)) : 
			glDisableClientState(va_arg(argv, uint32_t));
	}
	va_end(argv);
}

inline void grxGraphics_Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline grxCamera* grxGetGraphics_PerspectiveCamera(grxGraphicsInfo* _info) {
	return &_info->perspectiveCam;
}

inline grxCamera* grxGetGraphics_OrthogonalCamera(grxGraphicsInfo* _info) {
	return &_info->orthoCam;
}

inline void grxGraphics_Present(SDL_Window* _window) {
	SDL_GL_SwapWindow(_window);
}
