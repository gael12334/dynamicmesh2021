#pragma once

#include "Abstract.h"
#include "DLIterator.h"
#include "Array.h"
#include "../Overhaul/Overhaul.h"

typedef struct grxList {
	grxDLNode* front;
	grxDLNode* back;
	uint64_t elemCount;
} grxList;

void grxCtorList(grxList* _list) {
	memset(_list, 0, sizeof(grxList));
}

void grxDtorList(grxList* _list) {
	grxDLNode* temp = NULL;

	while(_list->back) {
		temp = _list->back->prev;
		grxDestroyDLNode(&_list->back);
		_list->back = temp;
	}
}

grxList* grxCreateList() {
	grxList* list = Malloc(grxList, 1);
	grxCtorList(list);
	return list;
}

void grxDestroyList(grxList** _list) {
	grxDtorList(*_list);
	free(*_list);
	*_list = NULL;
}

void grxPushListBack(grxList* _list, grxAbstract _data) {
	if(_list->elemCount == UINT64_MAX) {
		grxSetError(GRX_ERR_OVERFLOW);
		return;
	}

	if(_list->back) 
		_list->back = _list->back->next = grxCreateDLNode(_data, _list->back, NULL);
	else 
		_list->front = _list->back = grxCreateDLNode(_data, NULL, NULL);
	_list->elemCount++;
}

void grxPushListFront(grxList* _list, grxAbstract _data) {
	if(_list->elemCount == UINT64_MAX) {
		grxSetError(GRX_ERR_OVERFLOW);
		return;
	}

	if(_list->front) 
		_list->front = grxCreateDLNode(_data, NULL, _list->front);
	else 
		_list->front = _list->back = grxCreateDLNode(_data, NULL, NULL);
	_list->elemCount++;
}

void grxPopListBack(grxList* _list) {
	if(_list->back == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	if(_list->front == _list->back) {
		grxDestroyDLNode(&_list->back);
		_list->front = NULL;
		_list->elemCount = 0;
		return;
	}

	grxDLNode* temp = _list->back->prev;
	grxDestroyDLNode(&_list->back);
	_list->back = temp;
	_list->elemCount--;
}

void grxPopListFront(grxList* _list) {
	if(_list->front == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	if(_list->front == _list->back) {
		grxDestroyDLNode(&_list->front);
		_list->back = NULL;
		_list->elemCount = 0;
		return;
	}

	grxDLNode* temp = _list->front->next;
	grxDestroyDLNode(&_list->front);
	_list->front = temp;
	_list->elemCount--;
}

grxDLIterator grxGetListIterator(grxList* _list, uint64_t _index) {
	if(_index >= _list->elemCount) {
		grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
		grxDLIterator it;
		grxCtorDLIterator(&it, grxPtrAbstract(_list), NULL);
		return it;
	}
	grxDLNode* temp = _list->front;
	for(uint64_t i = 0; i < _index; i++) 
		temp = temp->next;

	grxDLIterator it;
	grxCtorDLIterator(&it, grxPtrAbstract(_list), temp);
	return it;
}

void grxInsertList_WithIterator(grxList* _list, grxDLIterator* _it, grxAbstract _data) {
	if(_list->elemCount == UINT64_MAX) {
		grxSetError(GRX_ERR_OVERFLOW);
		return;
	}

	if(_it->node != NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	if(_it->container.ptr != _list) {
		grxSetError(GRX_ERR_INVALID_VALUE);
		return;
	}

	if(_it->node->prev == NULL) {
		grxPushListFront(_list, _data);
		return;
	}
	
	_it->node->next = grxCreateDLNode(_data, _it->node->prev, _it->node);
	_it->node->next->prev = _it->node;
	_it->node->prev->next = _it->node;
	_list->elemCount++;
}

void grxInsertList_WithIndex(grxList* _list, uint64_t _index, grxAbstract _data) {
	if(_index == 0) 
		grxPushListFront(_list, _data);
	else if(_index >= _list->elemCount) 
		grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
	else {
		grxDLIterator it = grxGetListIterator(_list, _index);
		grxInsertList_WithIterator(_list, &it, _data);
	}
}

void grxRemoveList_WithIterator(grxList* _list, grxDLIterator* _it) {
	if(_it->node != NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return;
	}

	if(_it->container.ptr != _list) {
		grxSetError(GRX_ERR_INVALID_VALUE);
		return;
	}

	if(_it->node->prev == NULL) {
		grxPopListFront(_list);
		return;
	}

	grxDLNode* node = _it->node;
	_it->node->prev->next = _it->node->next;
	_it->node->next->prev = _it->node->prev;
	_it->node = _it->node->next;
	grxDestroyDLNode(&node);
}

void grxRemoveList_WithIndex(grxList* _list, uint64_t _index) {
	if(_index == 0) 
		grxPopListFront(_list);
	else if(_index >= _list->elemCount) 
		grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
	else {
		grxDLIterator it = grxGetListIterator(_list, _index);
		grxRemoveList_WithIterator(_list, &it);
	}
}

uint64_t grxGetListCount(grxList* _list) {
	return _list->elemCount;
}

grxDLIterator grxGetListFrontIterator(grxList* _list) {
	grxDLIterator it;
	grxCtorDLIterator(&it, grxPtrAbstract(_list), _list->front);
	return it;
}

grxDLIterator grxGetListBackIterator(grxList* _list) {
	grxDLIterator it;
	grxCtorDLIterator(&it, grxPtrAbstract(_list), _list->back);
	return it;
}

grxAbstract grxGetListFrontValue(grxList* _list) {
	if(_list->front == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return grxNullAbstract();
	}

	return _list->front->data;
}

grxAbstract grxGetListFrontValue_Fast(grxList* _list) {
	return _list->front->data;
}

grxAbstract grxGetListBackValue(grxList* _list) {
	if(_list->back == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return grxNullAbstract();
	}

	return _list->back->data;
}

grxAbstract grxGetListBackValue_Fast(grxList* _list) {
	return _list->back->data;
}

void grxSetListFrontValue(grxList* _list, grxAbstract _data) {
	if(_list->front == NULL) 
		grxSetError(GRX_ERR_NULL_REFERENCE);

	_list->front->data = _data;
}

void grxSetListFrontValue_Fast(grxList* _list, grxAbstract _data) {
	_list->front->data = _data;
}

void grxSetListBackValue(grxList* _list, grxAbstract _data) {
	if(_list->back == NULL) 
		grxSetError(GRX_ERR_NULL_REFERENCE);

	_list->back->data = _data;
}

void grxSetListBackValue_Fast(grxList* _list, grxAbstract _data) {
	_list->back->data = _data;
}

void grxSwapListElement(grxList* _list, grxDLIterator* _a, grxDLIterator* _b) {
	if((_a->container.ptr == _list) && (_b->container.ptr == _list)) {
		grxAbstract temp = _a->node->data;
		_a->node->data = _b->node->data;
		_b->node->data = temp;
		return;
	}

	grxSetError(GRX_ERR_INVALID_VALUE);
}

grxArray* grxListToArray(grxList* _list) {
	grxArray* arr = grxCreateArray(_list->elemCount);
	uint64_t index = 0;
	for(grxDLIterator it = grxGetListFrontIterator(_list); grxHasDLIteratorNext(&it); grxDLIteratorNext(&it)) {
		grxSetArrayValue_Fast(arr, index, grxGetDLIteratorValue(&it));
		index++;
	}
	return arr;
}
