#pragma once

#include "Abstract.h"
#include "ArrayList.h"
#include "Pair.h"

#define GRX_MAP_ARRAY_SIZE 31

typedef struct grxMapIterator {
	grxArrayList** arrList;
	uint32_t arrListIndex;
	uint32_t elemIndex;
	uint32_t nextArrListIndex;
	uint32_t nextElemIndex;
} grxMapIterator;

void grxCtorMapIterator(grxMapIterator* _it, grxArrayList** _arrOfArrList) {
	memset(_it, 0, sizeof(grxMapIterator));
	_it->arrList = _arrOfArrList;
}

void grxDtorMapIterator(grxMapIterator* _it) {
	//empty dtor.
}

bool grxHasMapIteratorNext(grxMapIterator* _it) {
	if (_it->nextArrListIndex == GRX_MAP_ARRAY_SIZE)
		return false;

	_it->elemIndex = _it->nextElemIndex;
	_it->arrListIndex = _it->nextArrListIndex;
	return true;
}

void grxMapIteratorNext(grxMapIterator* _it) {
	uint32_t dequeCount = grxGetArrayListCount(_it->arrList[_it->arrListIndex]);

	if ((_it->elemIndex + 1) >= dequeCount) {
		uint32_t subDequeElemCount = 0U;

		for (uint32_t i = _it->arrListIndex + 1; i < GRX_MAP_ARRAY_SIZE; i++) {
			subDequeElemCount = grxGetArrayListCount(_it->arrList[i]);
			if (subDequeElemCount) {
				_it->nextArrListIndex = i;
				_it->nextElemIndex = 0U;
				return;
			}
		}

		_it->nextArrListIndex = GRX_MAP_ARRAY_SIZE;
		_it->elemIndex = 0U;
		return;
	}

	_it->nextElemIndex = _it->elemIndex + 1;
}

grxPair* grxGetMapIteratorValue(grxMapIterator* _it) {
	grxAbstract a = grxGetArrayListValue_Fast(_it->arrList[_it->arrListIndex], _it->elemIndex);
	return (grxPair*) a.ptr;
}
