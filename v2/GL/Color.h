#pragma once

#include "GLLib.h"

typedef struct grxColor {
	uint8_t r, g, b, a;
} grxColor;

void grxCtorColor(grxColor* _color, uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) {
	_color->r = _r;
	_color->g = _g;
	_color->b = _b;
	_color->a = _a;
}

void grxDtorColor(grxColor* _color) {
	// empty dtor.
}

grxColor* grxCreateColor(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) {
	grxColor* color = Malloc(grxColor, 1);
	grxCtorColor(color, _r, _g, _b, _a);
	return color;
}

void grxDestroyColor(grxColor** _color) {
	grxDtorColor(*_color);
	free(*_color);
	*_color = NULL;
}

