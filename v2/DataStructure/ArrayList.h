#pragma once

#include "Abstract.h"
#include "../Overhaul/Overhaul.h"

#define GRX_ARRAYLIST_ALLOC_SIZE 10

/************************
 * Implementation of an
 * array list in C.
 ************************/
typedef struct grxArrayList {
	grxAbstract* data;			
	int64_t elemCount;
	int64_t elemAlloc;	
	int64_t frontIndex;
	int64_t backIndex;
} grxArrayList;

void grxCtorArrayList(grxArrayList* _arrList) {
	memset(_arrList, 0, sizeof(grxArrayList));
}

void grxDtorArrayList(grxArrayList* _arrList) {
	if(_arrList->data) 
		free(_arrList->data);
}

grxArrayList* grxCreateArrayList() {
	grxArrayList* arrList = Malloc(grxArrayList, 1);
	grxCtorArrayList(arrList);
	return arrList;
}

void grxDestroyArrayList(grxArrayList** _arrList) {
	grxDtorArrayList(*_arrList);
	free(*_arrList);
}

void grxAddArrayListBack(grxArrayList* _arrList, grxAbstract _data) {
	if(_arrList->elemCount == INT64_MAX) {
		grxSetError(GRX_ERR_OVERFLOW);
		return;
	}

	if(_arrList->data == NULL) {
		_arrList->data = Calloc(grxAbstract, GRX_ARRAYLIST_ALLOC_SIZE); 
		_arrList->elemAlloc = GRX_ARRAYLIST_ALLOC_SIZE;
		_arrList->frontIndex = -1;
		_arrList->backIndex = 0; // could be removed (unnecessery)
		_arrList->data[_arrList->backIndex] = _data;
		_arrList->backIndex++;
		_arrList->elemCount = 1;
		return;
	}

	if((_arrList->backIndex % GRX_ARRAYLIST_ALLOC_SIZE) == 0) {
		if((INT64_MAX - _arrList->elemAlloc) < GRX_ARRAYLIST_ALLOC_SIZE) {
			grxSetError(GRX_ERR_OVERFLOW);
			return;
		}

		grxAbstract* tempArray = _arrList->data;
		_arrList->data = Calloc(grxAbstract, _arrList->elemAlloc + GRX_ARRAYLIST_ALLOC_SIZE);
		MemCpy(_arrList->data, tempArray, sizeof(grxAbstract), _arrList->elemAlloc);
		free(tempArray);

		_arrList->elemAlloc += GRX_ARRAYLIST_ALLOC_SIZE;
		_arrList->data[_arrList->backIndex] = _data;
		_arrList->backIndex++;
		_arrList->elemCount++;
		return;
	}

	_arrList->data[_arrList->backIndex] = _data;
	_arrList->backIndex++;
	_arrList->elemCount++;
}

void grxAddArrayListFront(grxArrayList* _arrList, grxAbstract _data) {
	if(_arrList->elemCount == INT64_MAX) {
		grxSetError(GRX_ERR_OVERFLOW);
		return;
	}

	if(_arrList->data == NULL) {
		_arrList->data = Calloc(grxAbstract, GRX_ARRAYLIST_ALLOC_SIZE); 
		_arrList->elemAlloc = GRX_ARRAYLIST_ALLOC_SIZE;
		_arrList->frontIndex = -1;
		_arrList->backIndex = 0; // could be removed (unnecessery)
		_arrList->data[_arrList->backIndex] = _data;
		_arrList->backIndex++;
		_arrList->elemCount = 1;
		return;
	}

	if(_arrList->frontIndex == -1) {
		if((INT64_MAX - _arrList->elemAlloc) < GRX_ARRAYLIST_ALLOC_SIZE) {
			grxSetError(GRX_ERR_OVERFLOW);
			return;
		}

		grxAbstract* tempArray = _arrList->data;
		_arrList->data = Calloc(grxAbstract, _arrList->elemAlloc + GRX_ARRAYLIST_ALLOC_SIZE);
		MemCpyEx(_arrList->data, GRX_ARRAYLIST_ALLOC_SIZE, tempArray, 0, sizeof(grxAbstract), _arrList->elemCount);
		free(tempArray);

		_arrList->elemAlloc += GRX_ARRAYLIST_ALLOC_SIZE;
		_arrList->frontIndex = GRX_ARRAYLIST_ALLOC_SIZE - 1;
		_arrList->backIndex += GRX_ARRAYLIST_ALLOC_SIZE;
		_arrList->data[_arrList->frontIndex] = _data;
		_arrList->frontIndex--;
		_arrList->elemCount++;
		return;
	}

	_arrList->data[_arrList->frontIndex] = _data;
	_arrList->frontIndex--;
	_arrList->elemCount++;
}

void grxRemoveArrayListBack(grxArrayList* _arrList) {
	if(_arrList->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	if(_arrList->elemCount == 1) {
		free(_arrList->data);
		memset(_arrList, 0, sizeof(grxArrayList));
		return;
	}

	if((_arrList->elemAlloc - _arrList->elemCount) == GRX_ARRAYLIST_ALLOC_SIZE) {
		grxAbstract* tempArray = _arrList->data;
		_arrList->data = Calloc(grxAbstract, _arrList->elemAlloc - GRX_ARRAYLIST_ALLOC_SIZE);
		MemCpy(_arrList->data, tempArray, sizeof(grxAbstract), _arrList->elemAlloc - GRX_ARRAYLIST_ALLOC_SIZE);
		free(tempArray);

		_arrList->elemAlloc -= GRX_ARRAYLIST_ALLOC_SIZE;
		_arrList->backIndex--;
		_arrList->elemCount--;
		return;
	}

	_arrList->backIndex--;
	_arrList->elemCount--;
}

void grxRemoveArrayListFront(grxArrayList* _arrList) {
	if(_arrList->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	if(_arrList->elemCount == 1) {
		free(_arrList->data);
		memset(_arrList, 0, sizeof(grxArrayList));
		return;
	}

	if(_arrList->frontIndex == (GRX_ARRAYLIST_ALLOC_SIZE - 1)) {
		grxAbstract* tempArray = _arrList->data;
		_arrList->data = Calloc(grxAbstract, _arrList->elemAlloc - GRX_ARRAYLIST_ALLOC_SIZE);
		MemCpyEx(_arrList->data, 0, tempArray, GRX_ARRAYLIST_ALLOC_SIZE, sizeof(grxAbstract), _arrList->elemAlloc - GRX_ARRAYLIST_ALLOC_SIZE);
		free(tempArray);

		_arrList->elemAlloc -= GRX_ARRAYLIST_ALLOC_SIZE;
		_arrList->frontIndex = -1;
		_arrList->backIndex -= GRX_ARRAYLIST_ALLOC_SIZE;
		_arrList->elemCount--;
		return;
	}

	_arrList->frontIndex++;
	_arrList->elemCount--;
}

int64_t grxGetArrayListCount(grxArrayList* _arrList) {
	return _arrList->elemCount;
}

grxAbstract grxGetArrayListValue(grxArrayList* _arrList, int64_t _index) {
	if((_index >= 0) && (_index < _arrList->elemCount)) 
		return _arrList->data[_arrList->frontIndex + 1 + _index];
	
	grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
	return grxNullAbstract();
}

grxAbstract grxGetArrayListValue_Fast(grxArrayList* _arrList, int64_t _index) {
	return _arrList->data[_arrList->frontIndex + 1 + _index];
}

grxAbstract* grxGetArrayListNative(grxArrayList* _arrList) {
	return &_arrList->data[_arrList->frontIndex + 1]; // Or potentially "_arrList->data + _arrList->frontIndex + 0x1"
}

grxAbstract grxGetArrayListFirst(grxArrayList* _arrList) {
	if(_arrList->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return grxNullAbstract();
	}

	return _arrList->data[_arrList->frontIndex + 1];
}

grxAbstract grxGetArrayListLast(grxArrayList* _arrList) {
	if(_arrList->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return grxNullAbstract();
	}

	return _arrList->data[_arrList->backIndex - 1];
}

void grxSetArrayListValue(grxArrayList* _arrList, int64_t _index, grxAbstract _data) {
	if((_index > 0) && (_index < _arrList->elemCount)) 
		_arrList->data[_arrList->frontIndex + 1 + _index] = _data;
	
	else 
		grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
}

void grxSetArrayListValue_Fast(grxArrayList* _arrList, int64_t _index, grxAbstract _data) {
	_arrList->data[_arrList->frontIndex + 1 + _index] = _data;
}

void grxSetArrayListFirst(grxArrayList* _arrList, grxAbstract _data) {
	if(_arrList->data == NULL) 
		grxSetError(GRX_ERR_NULL_REFERENCE);

	_arrList->data[_arrList->frontIndex + 1] = _data;
}

void grxSetArrayListLast(grxArrayList* _arrList, grxAbstract _data) {
	if(_arrList->data == NULL) 
		grxSetError(GRX_ERR_NULL_REFERENCE);

	_arrList->data[_arrList->backIndex - 1] = _data;
}

void grxSwapArrayListElement(grxArrayList* _arrList, int64_t _src, int64_t _dest) {
	if(_arrList->data == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}
	
	if((_src < _arrList->elemCount) && (_arrList->elemCount) && (_src > 0) && (_dest > 0)) {
		grxAbstract a = grxGetArrayListValue_Fast(_arrList, _src);
		grxAbstract b = grxGetArrayListValue_Fast(_arrList, _dest);
		grxSetArrayListValue_Fast(_arrList, _src, b);
		grxSetArrayListValue_Fast(_arrList, _dest, a);
		return;
	}

	grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
}

void grxSwapArrayListElement_Fast(grxArrayList* _arrList, int64_t _src, int64_t _dest) {
	grxAbstract a = grxGetArrayListValue_Fast(_arrList, _src);
	grxAbstract b = grxGetArrayListValue_Fast(_arrList, _dest);
	grxSetArrayListValue_Fast(_arrList, _src, b);
	grxSetArrayListValue_Fast(_arrList, _dest, a);
}