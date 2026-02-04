#pragma once 

#include "Abstract.h"
#include "ArrayList.h"
#include "MapIterator.h"
#include "Pair.h"

/**********************************
 * 
 **********************************/
typedef uint64_t(*grxMapHashFunction)(grxAbstract _data);
typedef bool(*grxMapCompareFunction)(grxAbstract _a, grxAbstract _b);

uint64_t grxDefaultMapHasher(grxAbstract _key) {
	return _key.u;
}

bool grxDefaultMapComparer(grxAbstract _internal, grxAbstract _external) {
	return _internal.i == _external.i;
}

/**********************************
 * 
 **********************************/
typedef struct grxMap {
	grxArrayList* arrLists[GRX_MAP_ARRAY_SIZE];
	uint64_t elemCount;
	grxMapHashFunction hashFn;
	grxMapCompareFunction compareFn;
} grxMap;

grxMap* grxCreateMap(grxMapCompareFunction _compare, grxMapHashFunction _hash) {
	grxMap* map = (grxMap*)malloc(sizeof(grxMap));
	map->elemCount = 0U;
	map->compareFn = (_compare ? _compare : (grxMapCompareFunction)grxDefaultMapComparer);
	map->hashFn = (_hash ? _hash : grxDefaultMapHasher);

	for (uint32_t i = 0U; i < GRX_MAP_ARRAY_SIZE; i++)
		map->arrLists[i] = grxCreateArrayList();

	return map;
}

void grxFreeMap(grxMap** _map) {
	grxMap* mapInst = *_map;

	for (uint32_t i = 0; i < GRX_MAP_ARRAY_SIZE; i++) {
		if (mapInst->arrLists[i])
			grxDestroyArrayList(&mapInst->arrLists[i]);
	}

	free(mapInst);
	*_map = NULL;
	return;
}

uint64_t grxHashMapKey(grxMap* _map, grxAbstract _key) {
	uint64_t index = _map->hashFn(_key);
	return index % GRX_MAP_ARRAY_SIZE;
}

grxPair* grxSeekMapPair(grxMap* _map, grxAbstract _key, uint32_t _index, uint32_t* _arrListIndex) {
	grxPair* pair;
	for (uint32_t i = 0U, m = grxGetArrayListCount(_map->arrLists[_index]); i < m; i++) {
		pair = (grxPair*) grxGetArrayListValue(_map->arrLists[_index], i).ptr;
		if (_map->compareFn(pair->first, _key)) {
			if (_arrListIndex)
				*_arrListIndex = i;
			return pair;
		}
	}

	return NULL;
}

bool grxInsertMapValue(grxMap* _map, grxAbstract _key, grxAbstract _value) {
	uint64_t index = grxHashMapKey(_map, _key);

	if (grxSeekMapPair(_map, _key, index, NULL))
		return false;
	
	grxAddArrayListBack(_map->arrLists[index], grxPtrAbstract(grxCreatePair(_key, _value)));
	_map->elemCount++;
	return true;
}

bool grxRemoveMapValue(grxMap* _map, grxAbstract _key) {
	uint32_t index = grxHashMapKey(_map, _key);
	uint32_t arrListIndex = 0U;
	grxPair* pair = grxSeekMapPair(_map, _key, index, &arrListIndex);

	if (pair) {
		grxArrayList* arrList = _map->arrLists[index];
		uint32_t lastIndex = grxGetArrayListCount(arrList) - 1U;
		grxSwapArrayListElement_Fast(arrList, arrListIndex, lastIndex);
		grxRemoveArrayListBack(arrList);
		free(pair);

		return true;
	}

	return false;
}

grxAbstract grxGetMapValue(grxMap* _map, grxAbstract _key) {
	uint64_t index = grxHashMapKey(_map, _key);
	grxPair* pair = grxSeekMapPair(_map, _key, index, NULL);
	if (pair)
		return pair->second;

	return grxNullAbstract();
}

bool grxSetMapValue(grxMap* _map, grxAbstract _key, grxAbstract _value) {
	uint32_t index = grxHashMapKey(_map, _key); 
	grxPair* pair = grxSeekMapPair(_map, _key, index, NULL);

	if (pair) {
		pair->second = _value;
		return true;
	}

	return false;
}

uint32_t grxGetMapCount(grxMap* _map) {
	return _map->elemCount;
}

grxMapIterator grxGetMapIterator(grxMap* _map) {
	grxMapIterator it = { _map->arrLists, 0U, 0U, 0U, 0U };
	grxMapIteratorNext(&it);
	return it;
}