#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "pair.h"
#include "deque.h"

#define grxMapArraySize 31U

typedef struct grxMapIterator {
	grxDeque** _array;
	uint32_t _dequeIndex;
	uint32_t _elemIndex;
	uint32_t _nextDequeIndex;
	uint32_t _nextElemIndex;
} grxMapIterator;

bool grxHasMapIteratorNext(grxMapIterator* _it) {
	if (_it->_nextDequeIndex == grxMapArraySize)
		return false;

	_it->_elemIndex = _it->_nextElemIndex;
	_it->_dequeIndex = _it->_nextDequeIndex;
	return true;
}

void grxMapIteratorNext(grxMapIterator* _it) {
	uint32_t dequeCount = grxGetDequeCount(_it->_array[_it->_dequeIndex]);

	if ((_it->_elemIndex + 1) >= dequeCount) {
		uint32_t subDequeElemCount = 0U;

		for (uint32_t i = _it->_dequeIndex + 1; i < grxMapArraySize; i++) {
			subDequeElemCount = grxGetDequeCount(_it->_array[i]);
			if (subDequeElemCount) {
				_it->_nextDequeIndex = i;
				_it->_nextElemIndex = 0U;
				return;
			}
		}

		_it->_nextDequeIndex = grxMapArraySize;
		_it->_elemIndex = 0U;
		return;
	}

	_it->_nextElemIndex = _it->_elemIndex + 1;
}

grxPair* grxGetMapIteratorValue(grxMapIterator* _it) {
	return (grxPair*)grxGetDequeValue(_it->_array[_it->_dequeIndex], _it->_elemIndex);
}

// = = = = = = = = = = = = = = = = = = = = = = = =

typedef int8_t(*CompareFunction)(void* _internal, void* _external);
typedef uint32_t(*HashFunction)(void* _key);

typedef struct grxMap {
	grxDeque* _array[grxMapArraySize];
	uint32_t _count;

	CompareFunction _compareKey;
	HashFunction _hashKey;
} grxMap; 

bool grxDefaultComparer(void* _internal, void* _external) {
	return _internal == _external;
}

uint32_t grxDefaultHasher(void* _key) {
	uint64_t value = (uint64_t)_key;
	return value % grxMapArraySize;
}

grxMap* grxCreateMap(CompareFunction _compare, HashFunction _hash) {
	grxMap* map = (grxMap*)malloc(sizeof(grxMap));
	map->_count = 0U;
	map->_compareKey = (_compare ? _compare : (CompareFunction)grxDefaultComparer);
	map->_hashKey = (_hash ? _hash : grxDefaultHasher);

	for (uint32_t i = 0U; i < grxMapArraySize; i++)
		map->_array[i] = grxCreateDeque();

	return map;
}

void grxFreeMap(grxMap** _map) {
	grxMap* mapInst = *_map;

	for (uint32_t i = 0; i < grxMapArraySize; i++) {
		if (mapInst->_array[i])
			grxFreeDeque(&mapInst->_array[i]);
	}

	free(mapInst);
	*_map = NULL;
	return;
}

uint32_t grxHashMapKey(grxMap* _map, void* _key) {
	uint32_t index = _map->_hashKey(_key);
	return index % grxMapArraySize;
}

grxPair* grxSeekMapPair(grxMap* _map, void* _key, uint32_t _index, uint32_t* _dequeIndex) {
	grxPair* pair;
	for (uint32_t i = 0U, m = grxGetDequeCount(_map->_array[_index]); i < m; i++) {
		pair = (grxPair*) grxGetDequeValue(_map->_array[_index], i);
		if (_map->_compareKey(pair->_key, _key)) {
			if (_dequeIndex)
				*_dequeIndex = i;
			return pair;
		}
	}

	return NULL;
}

bool grxInsertMapValue(grxMap* _map, void* _key, void* _value) {
	uint32_t index = grxHashMapKey(_map, _key);

	if (grxSeekMapPair(_map, _key, index, NULL))
		return false;
	
	grxAddDequeBack(_map->_array[index], grxCreatePair(_key, _value));
	_map->_count++;
	return true;
}

bool grxRemoveMapValue(grxMap* _map, void* _key) {
	uint32_t index = grxHashMapKey(_map, _key);
	uint32_t dequeIndex = 0U;
	grxPair* pair = grxSeekMapPair(_map, _key, index, &dequeIndex);

	if (pair) {
		grxDeque* deque = _map->_array[index];
		uint32_t lastIndex = grxGetDequeCount(deque) - 1U;
		grxDequeSwap(deque, dequeIndex, lastIndex);
		grxRemoveDequeBack(deque);
		free(pair);

		return true;
	}

	return false;
}

void* grxGetMapValue(grxMap* _map, void* _key) {
	uint32_t index = grxHashMapKey(_map, _key);
	grxPair* pair = grxSeekMapPair(_map, _key, index, NULL);
	if (pair)
		return pair->_entry;

	return NULL;
}

bool grxSetMapValue(grxMap* _map, void* _key, void* _value) {
	uint32_t index = grxHashMapKey(_map, _key); 
	grxPair* pair = grxSeekMapPair(_map, _key, index, NULL);

	if (pair) {
		pair->_entry = _value;
		return true;
	}

	return false;
}

uint32_t grxGetMapCount(grxMap* _map) {
	return _map->_count;
}

grxMapIterator grxGetMapIterator(grxMap* _map) {
	grxMapIterator it = { _map->_array, 0U, 0U, 0U, 0U };
	grxMapIteratorNext(&it);
	return it;
}