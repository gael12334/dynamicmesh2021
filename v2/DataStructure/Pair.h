#pragma once

#include "Abstract.h"

typedef struct grxPair {
	grxAbstract first;
	grxAbstract second;
} grxPair;

void grxCtorPair(grxPair* _pair, grxAbstract _first, grxAbstract _second) {
	_pair->first = _first;
	_pair->second = _second;
}

void grxDtorPair(grxPair* _pair) {
	// empty dtor.
}

grxPair* grxCreatePair(grxAbstract _first, grxAbstract _second) {
	grxPair* pair = Malloc(grxPair, 1);
	grxCtorPair(pair, _first, _second);
	return pair;
}

void grxDestroyPair(grxPair** _pair) {
	grxDtorPair(*_pair);
	free(*_pair);
	*_pair = NULL;
}