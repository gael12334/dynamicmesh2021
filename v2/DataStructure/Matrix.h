#pragma once

#include "Abstract.h"
#include "../Overhaul/Overhaul.h"

struct grxMatrixCell;
struct grxMatrixRow;
struct grxMatrixColumn;

typedef struct grxMatrixCell {
	grxAbstract data;
	grxMatrixRow* row;
	grxMatrixColumn* column;
	uint64_t rowIndex;
	uint64_t columnIndex;
} grxMatrixCell;

typedef struct grxMatrixRow {
	grxMatrixCell** cells;
	uint64_t index;
	uint64_t count;
} grxMatrixRow;

typedef struct grxMatrixColumn {
	grxMatrixCell** cells;
	uint64_t index;
	uint64_t count;
} grxMatrixColumn;

typedef struct grxMatrix {
	grxMatrixRow** rows;
	grxMatrixColumn** columns;
	uint64_t width;
	uint64_t height;
} grxMatrix;

