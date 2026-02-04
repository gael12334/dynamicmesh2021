#pragma once

#include "../Overhaul/Overhaul.h"

/************************
 * Allows to store all 
 * C defined primitives.
 ************************/
typedef union grxAbstract {
	void* ptr;
	uint64_t u;
	int64_t i;
	double_t d;
} grxAbstract;

grxAbstract grxNullAbstract() {
	return (grxAbstract) { NULL };
}

grxAbstract grxIntAbstract(int64_t i) {
	grxAbstract a; 
	a.i = i;
	return a;
}

grxAbstract grxUIntAbstract(uint64_t u) {
	grxAbstract a; 
	a.u = u;
	return a;
}

grxAbstract grxDoubleAbstract(double_t d) {
	grxAbstract a; 
	a.d = d;
	return a;
}

grxAbstract grxPtrAbstract(void* ptr) {
	grxAbstract a; 
	a.ptr = ptr;
	return a;
}

bool grxIsAbstractNull(grxAbstract _a) {
	return _a.ptr == NULL;
}
