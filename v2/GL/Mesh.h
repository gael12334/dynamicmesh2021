#pragma once

#include "GLLib.h"
#include "Vector.h"
#include "Point.h"
#include "Rect.h"
#include "Matrix.h"
#include "Edge.h"
#include "Splines.h"
#include "Color.h"

#define GRX_TEXCOORDS_SIZE 2
#define GRX_COLOR_SIZE 4

/************************
 * 
 ************************/
typedef struct grxMeshStruct {
	double* vertices;
	double* texcoords;
	double* normals;
	uint8_t* colors;
	int vertexSize;
	int texcoordSize;
	int colorSize;
	GLenum drawingMode;
	uint64_t vertexCount;
} grxMeshStruct;

void grxCtorMeshStruct(grxMeshStruct* _mesh, GLenum _drawingMode, int _vertexSize, bool _hasTexCoords, bool _hasColor, bool _hasNormal, uint64_t _vertexCount) {
	if((_vertexSize == 0) || (_vertexCount == 0)) {
		grxSetError(GRX_ERR_INVALID_VALUE);
		memset(_mesh, 0, sizeof(grxMeshStruct));
		return;
	}

	_mesh->texcoordSize = GRX_TEXCOORDS_SIZE;
	_mesh->colorSize = GRX_COLOR_SIZE;
	_mesh->vertexSize = _vertexSize;
	_mesh->vertexCount = _vertexCount;
	_mesh->drawingMode = _drawingMode;

	_mesh->vertices = Malloc(double, _vertexCount * _vertexSize);

	_mesh->texcoords = (_hasTexCoords) ?
		Malloc(double, _vertexCount * GRX_TEXCOORDS_SIZE) :
		NULL;

	_mesh->colors = (_hasColor) ? 
		Malloc(uint8_t, _vertexCount * GRX_COLOR_SIZE) : 
		NULL;

	_mesh->normals = (_hasNormal) ? 
		Malloc(double, _vertexCount * _vertexSize) : 
		NULL;
}

void grxDtorMeshStruct(grxMeshStruct* _mesh) {
	free(_mesh->vertices);

	if(_mesh->texcoords)
		free(_mesh->texcoords);
	if(_mesh->colors) 
		free(_mesh->colors);
	if(_mesh->normals)
		free(_mesh->normals);
}

grxMeshStruct* grxCreateMeshStruct(GLenum _drawingMode, int _vertexSize, bool _hasTexCoords, bool _hasColor, bool _hasNormal, uint64_t _vertexCount) {
	grxMeshStruct* mesh = Malloc(grxMeshStruct, 1);
	grxCtorMeshStruct(mesh, _drawingMode, _vertexSize, _hasTexCoords, _hasColor, _hasNormal, _vertexCount);
	return mesh;
}

void grxDestroyMeshStruct(grxMeshStruct** _mesh) {
	grxDtorMeshStruct(*_mesh);
	free(*_mesh);
	*_mesh = NULL;
}

/************************
 * 
 ************************/
typedef void(*grxMeshRenderDelegate)(grxMeshStruct*);

#define GRX_MESH_TEXCOORDS   0b00000001
#define GRX_MESH_COLOR       0b00000010
#define GRX_MESH_NORMAL      0b00000100

/************************
 * Contains a mesh's
 * information.
 ************************/
typedef struct grxMesh {
	grxVector center;
	grxMeshStruct* info;
	grxMeshRenderDelegate render;
} grxMesh;

void grxCtorMesh(grxMesh* _mesh, grxMeshRenderDelegate _render, GLenum _drawingMode, uint8_t _flags, int _vertexSize, uint64_t _vertexCount) {
	_mesh->info = grxCreateMeshStruct(
		_drawingMode, 
		_vertexSize, 
		(_flags & GRX_MESH_TEXCOORDS), 
		(_flags & GRX_MESH_COLOR), 
		(_flags & GRX_MESH_NORMAL),
		_vertexCount
	);
	_mesh->render = _render;
	_mesh->center = (grxVector){0.0, 0.0};
}

void grxDtorMesh(grxMesh* _mesh) {
	void* arr;
	if(arr = _mesh->info->vertices)
		free(arr);
	if(arr = _mesh->info->texcoords)
		free(arr);
	if(arr = _mesh->info->colors)
		free(arr);
	if(arr = _mesh->info->normals) 
		free(arr);
} 

grxMesh* grxCreateMesh(grxMeshRenderDelegate _render, GLenum _drawingMode, uint8_t _flags, int _vertexSize, uint64_t _vertexCount) {
	grxMesh* mesh = Malloc(grxMesh, 1);
	grxCtorMesh(mesh, _render, _drawingMode, _flags, _vertexSize, _vertexCount);
	return mesh;
}

void grxDestroyMesh(grxMesh** _mesh) {
	grxDtorMesh(*_mesh);
	free(*_mesh);
	*_mesh = NULL;
}

grxMesh* grxCopyMesh(grxMesh* _mesh) {
	uint8_t flags = 
		((_mesh->info->colorSize != 0) * GRX_MESH_TEXCOORDS) |
		((_mesh->info->colorSize != 0) * GRX_MESH_COLOR) |
		((_mesh->info->colorSize != 0) * GRX_MESH_NORMAL);
	
	grxMesh* copy = grxCreateMesh(_mesh->render, _mesh->info->drawingMode, flags, _mesh->info->vertexSize, _mesh->info->vertexCount);
	memcpy(copy->info->vertices, _mesh->info->vertices, sizeof(double) * _mesh->info->vertexSize * _mesh->info->vertexCount);
	memcpy(copy->info->texcoords, _mesh->info->texcoords, sizeof(double) * _mesh->info->texcoordSize * _mesh->info->vertexCount);
	memcpy(copy->info->colors, _mesh->info->colors, sizeof(double) * _mesh->info->colorSize * _mesh->info->vertexCount);
	memcpy(copy->info->normals, _mesh->info->vertices, sizeof(double) * _mesh->info->vertexSize * _mesh->info->vertexCount);

	return copy;
}

void grxProcessMeshCenter(grxMesh* _mesh) {
	if(_mesh->info->vertexCount == 0) {
		grxSetError(GRX_ERR_INVALID_VALUE);
		return;
	}

	_mesh->center = (grxVector) {0.0, 0.0, 0.0};

	for(uint64_t i = 0, m = _mesh->info->vertexCount * _mesh->info->vertexCount; i < m; i += 3) {
		_mesh->center.x = _mesh->info->vertices[  i  ];
		_mesh->center.y = _mesh->info->vertices[i + 1];
		_mesh->center.z = _mesh->info->vertices[i + 2];
	}

	_mesh->center.x /= _mesh->info->vertexCount * _mesh->info->vertexCount;
	_mesh->center.y /= _mesh->info->vertexCount * _mesh->info->vertexCount;
	_mesh->center.z /= _mesh->info->vertexCount * _mesh->info->vertexCount;
}

grxMesh* grxLoadMeshFrom_WaveFrontFile(const char _filename[], grxMeshRenderDelegate _render) Do {
	FILE* file = fopen(_filename, "r");
	if(file == NULL) {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return NULL;
	}

	grxMesh* mesh = NULL;

	char typeToken[3];
	char expression[256];
	char longFloatHolder[64];
	char numericBuffer[10];
	int verticeIndex[16];
	
	int vertexSize = 0;
	int vertexPerFace = 0;
	int flags = 0;
	bool vertexSizeSet = false;
	bool vertexPerFaceSet = false;

	grxArrayList* vi = Push(grxArrayList, grxCreateArrayList(), grxDtorArrayList);
	grxArrayList* ti = Push(grxArrayList, grxCreateArrayList(), grxDtorArrayList);
	grxArrayList* ni = Push(grxArrayList, grxCreateArrayList(), grxDtorArrayList);
	grxArrayList* vf = Push(grxArrayList, grxCreateArrayList(), grxDtorArrayList);
	grxArrayList* tf = Push(grxArrayList, grxCreateArrayList(), grxDtorArrayList);
	grxArrayList* nf = Push(grxArrayList, grxCreateArrayList(), grxDtorArrayList);
	
	while(fscanf(file, "%s", typeToken) != EOF) {
		if((typeToken[0] == 'v') && (typeToken[1] == 't')) {
			double x, y;
			fscanf(file, "%lf %lf", &x, &y);
			grxAddArrayListBack(ti, grxDoubleAbstract(x));
			grxAddArrayListBack(ti, grxDoubleAbstract(y));
			flags |= GRX_MESH_TEXCOORDS;
		}
		else if((typeToken[0] == 'v') && (typeToken[1] == 'n')) {
			int componentCount = 0;
			char caught = 0;
			double componentValue;
			
			while(caught != '\n') {
				fscanf(file, "%lf", &componentValue);
				fscanf(file, "%c", &caught);
				grxAddArrayListBack(ni, grxDoubleAbstract(componentValue));
				componentCount++;
			}

			if(vertexSizeSet == false) {
				vertexSizeSet = true;
				vertexSize = componentCount;
			}

			flags |= GRX_MESH_NORMAL;
		}
		else if((typeToken[0] == 'v')) {
			int componentCount = 0;
			char caught = 0;
			double componentValue;

			while(caught != '\n') {
				fscanf(file, "%lf", &componentValue);
				fscanf(file, "%c", &caught);
				grxAddArrayListBack(vi, grxDoubleAbstract(componentValue));
				componentCount++;
			}

			if(vertexSizeSet == false) {
				vertexSizeSet = true;
				vertexSize = componentCount;
			}
		}
		else if((typeToken[0] == 'f')) {
			int vertexCount = 0;
			//int verticeIndex[16];
			int indexIndex = 0;
			//char numericBuffer[10];
			char caught = 0;
			static int __debug_counter = 0;

			while(caught != '\n') {
				int result = fscanf(file, " %[^/ \n]", numericBuffer);
				if(result == 0)
					memset(numericBuffer, 0, sizeof(numericBuffer));

				if(numericBuffer[0] == '\0') 
					verticeIndex[indexIndex] = -1;
				else 
					sscanf(numericBuffer, "%i", &verticeIndex[indexIndex]);
				fscanf(file, "%c", &caught);

				if(caught == ' ') 
					vertexCount++;

				indexIndex++;
				__debug_counter++;
			}

			if(vertexPerFaceSet == false) {
				vertexPerFace = vertexCount;
				vertexPerFaceSet = true;
			}

			for(int i = 0; i < indexIndex; i += 3) {
				// sets the vertices in order...
				for(int j = 0; j < vertexSize; j++) {
					double value = grxGetArrayListValue_Fast(vi, (verticeIndex[i] - 1) * vertexSize + j).d;
					grxAddArrayListBack(vf, grxDoubleAbstract(value));
				}

				//sets the textures coords...
				if(flags & GRX_MESH_TEXCOORDS) {
					for(int j = 0; j < GRX_TEXCOORDS_SIZE; j++) {
						double value = grxGetArrayListValue_Fast(ti, (verticeIndex[i + 1] - 1) * GRX_TEXCOORDS_SIZE + j).d;
						grxAddArrayListBack(vf, grxDoubleAbstract(value));
					}
				}

				if(flags & GRX_MESH_NORMAL) {
					for(int j = 0; j < vertexSize; j++) {
						double value = grxGetArrayListValue_Fast(ni, (verticeIndex[i + 2] - 1) * vertexSize + j).d;
						grxAddArrayListBack(nf, grxDoubleAbstract(value));
					}
				}
			}
			
		}
		else 
			fscanf(file, "%[^\n]", expression);
	}

	mesh = grxCreateMesh(_render, (vertexSize == 3) ? GL_TRIANGLES : GL_QUADS, flags, vertexSize, grxGetArrayListCount(vf) / vertexSize);
	memcpy(mesh->info->vertices, grxGetArrayListNative(vf), sizeof(double) * grxGetArrayListCount(vf));

	if(flags & GRX_MESH_TEXCOORDS) 
		memcpy(mesh->info->texcoords, grxGetArrayListNative(tf), sizeof(double) * grxGetArrayListCount(tf));

	if(flags & GRX_MESH_NORMAL) 
		memcpy(mesh->info->normals, grxGetArrayListNative(nf), sizeof(double) * grxGetArrayListCount(nf));	

	fclose(file);
	Return mesh;
} End;

grxMesh* grxLoadMeshFrom_Bezier(grxSpline* _spline, int64_t _precision) {
	grxList* points = grxGetSplinePoints(_spline, _precision);

	grxMesh* mesh = grxCreateMesh(NULL, GL_LINES, 0, 3, grxGetListCount(points));

	grxAbstract a;
	uint64_t index = 0;
	for(grxDLIterator it = grxGetListFrontIterator(points); grxHasDLIteratorNext(&it); grxDLIteratorNext(&it)) {
		a = grxGetDLIteratorValue(&it);

		memcpy(&mesh->info->vertices[index], (double*)a.ptr, sizeof(double) * 3);
		index += 3;

		grxDestroyVector((grxVector**)&a.ptr);
	}

	return mesh;
}

grxMesh* grxLoadMesh_AsPlaneBezier(grxEdge* _start, grxEdge* _middle, grxEdge* _end, int64_t _precision, uint8_t _flags) Do { 
	//grxVector startVec = grxGetEdge_MiddlePoint(_start);
	//grxVector middleVec = grxGetEdge_MiddlePoint(_middle);
	//grxVector endVec = grxGetEdge_MiddlePoint(_end);
	//grxBezierCurve* middleCurve = Push(grxBezierCurve, grxCreateBezierCurve(&startVec, &middleVec, &endVec), grxDestroyBezierCurve);

	// left bézier
	grxBezierCurve* leftCurve = Push(grxBezierCurve, grxCreateBezierCurve(&_start->from, &_middle->from, &_end->from), grxDestroyBezierCurve);
	grxSpline* leftSpline = grxCreateSpline();
	grxAddSplineNode(leftSpline, leftCurve, GRX_SPLINE_BACK);

	// right bézier
	grxBezierCurve* rightCurve = Push(grxBezierCurve, grxCreateBezierCurve(&_start->to, &_middle->to, &_end->to), grxDestroyBezierCurve);
	grxSpline* rightSpline = grxCreateSpline();
	grxAddSplineNode(rightSpline, rightCurve, GRX_SPLINE_BACK);

	// getting the points
	grxList* leftList = grxGetSplinePoints(leftSpline, _precision);
	grxList* rightList = grxGetSplinePoints(rightSpline, _precision);

	// getting an array of points (the grxVectors' addresses in the list have been copied to the array.)
	grxArray* leftArray = Push(grxArray, grxListToArray(leftList), grxDestroyArray);
	grxArray* rightArray = Push(grxArray, grxListToArray(rightList), grxDestroyArray);

	// freeing some memory
	grxDestroyList(&leftList);
	grxDestroyList(&rightList);
	
	// link points to make quads.
	grxArrayList* resultMeshPoints = Push(grxArrayList, grxCreateArrayList(), grxDestroyArrayList);
	grxArrayList* resultNormalVectors = Push(grxArrayList, grxCreateArrayList(), grxDestroyArrayList);
	grxArrayList* resultTexCoords = Push(grxArrayList, grxCreateArrayList(), grxDestroyArrayList);
	grxVector* leftVecs[2];
	grxVector* rightVecs[2];
	grxVector normalVector;

	for(int64_t i = 1; i < leftArray->count; i++) {
		leftVecs[0] = (grxVector*) grxGetArrayValue_Fast(leftArray, i - 1).ptr;
		leftVecs[1] = (grxVector*) grxGetArrayValue_Fast(leftArray,   i  ).ptr;
		rightVecs[0] = (grxVector*) grxGetArrayValue_Fast(rightArray,   i  ).ptr;
		rightVecs[1] = (grxVector*) grxGetArrayValue_Fast(rightArray, i - 1).ptr;

		for(int j = 0; j < 2; j++) {
			grxAddArrayListBack(resultMeshPoints, grxDoubleAbstract(leftVecs[j]->x));
			grxAddArrayListBack(resultMeshPoints, grxDoubleAbstract(leftVecs[j]->y));
			grxAddArrayListBack(resultMeshPoints, grxDoubleAbstract(leftVecs[j]->z));
		}

		for(int j = 0; j < 2; j++) {
			grxAddArrayListBack(resultMeshPoints, grxDoubleAbstract(rightVecs[j]->x));
			grxAddArrayListBack(resultMeshPoints, grxDoubleAbstract(rightVecs[j]->y));
			grxAddArrayListBack(resultMeshPoints, grxDoubleAbstract(rightVecs[j]->z));
		}

		if(_flags & GRX_MESH_NORMAL) {
			grxGetVectorCrossProduct(&normalVector, leftVecs[1], rightVecs[0]);
			for(int i = 0; i < 4; i++) {
				grxAddArrayListBack(resultNormalVectors, grxDoubleAbstract(normalVector.x));
				grxAddArrayListBack(resultNormalVectors, grxDoubleAbstract(normalVector.y));
				grxAddArrayListBack(resultNormalVectors, grxDoubleAbstract(normalVector.z));
			}
		}

		if(_flags & GRX_MESH_TEXCOORDS) {
			grxAddArrayListBack(resultTexCoords, grxDoubleAbstract(0.0));
			grxAddArrayListBack(resultTexCoords, grxDoubleAbstract(0.0));
			grxAddArrayListBack(resultTexCoords, grxDoubleAbstract(1.0));
			grxAddArrayListBack(resultTexCoords, grxDoubleAbstract(0.0));
			grxAddArrayListBack(resultTexCoords, grxDoubleAbstract(1.0));
			grxAddArrayListBack(resultTexCoords, grxDoubleAbstract(1.0));
			grxAddArrayListBack(resultTexCoords, grxDoubleAbstract(0.0));
			grxAddArrayListBack(resultTexCoords, grxDoubleAbstract(1.0));
		}

		grxDestroyVector(&leftVecs[0]); 
		grxDestroyVector(&rightVecs[1]);
	}
	
	grxDestroyVector(&leftVecs[1]); 
	grxDestroyVector(&rightVecs[0]);

	grxMesh* mesh = grxCreateMesh(NULL, GL_QUADS, _flags, 3, grxGetArrayListCount(resultMeshPoints) / 3);
	
	memcpy(mesh->info->vertices, grxGetArrayListNative(resultMeshPoints), sizeof(double) * grxGetArrayListCount(resultMeshPoints));

	if(mesh->info->normals)
		memcpy(mesh->info->normals, grxGetArrayListNative(resultNormalVectors), sizeof(double) * grxGetArrayListCount(resultNormalVectors));
	
	if(mesh->info->texcoords)
		memcpy(mesh->info->texcoords, grxGetArrayListNative(resultTexCoords), sizeof(double) * grxGetArrayListCount(resultTexCoords));
	
	Return mesh;
} End;

void grxModifyMesh(grxMesh* _mesh, grxMatrix16* _matrix) {
	static uint64_t i;
	static grxVector v; 

	for(i = 0; i < _mesh->info->vertexCount; i++) { 
		v = *((grxVector*)&_mesh->info->vertices[i * 3]);
		grxMultMatrix16_Vector(&v, _matrix, &v);

		if(_mesh->info->normals) {
			v = *((grxVector*)&_mesh->info->normals[i * 3]);
			grxMultMatrix16_Vector(&v, _matrix, &v);
		}
	}
}

void grxRenderMesh(grxMesh* _mesh) {
	if(_mesh->render)
		_mesh->render(_mesh->info);
}

//
// Mesh setters and getters
//

void grxSetMeshVertexComponents(grxMesh* _mesh, uint64_t _index, double _x, double _y, double _z) {
	static double component[3];
	static double* selected = NULL;
	static int i = 0;

	if(_index >= _mesh->info->vertexCount) {
		grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
		return;
	}

	component[0] = _x;
	component[1] = _y; 
	component[2] = _z;

	selected = &_mesh->info->vertices[_mesh->info->vertexSize * _index];

	for(i = 0; i < _mesh->info->vertexSize; i++) 
		selected[i] = component[i];
}

void grxSetMeshNormalComponents(grxMesh* _mesh, uint64_t _index, double _x, double _y, double _z) {
	static double component[3];
	static double* selected = NULL;
	static int i = 0;

	if(_index >= _mesh->info->vertexCount) {
		grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
		return;
	}

	component[0] = _x;
	component[1] = _y; 
	component[2] = _z;

	selected = &_mesh->info->normals[_mesh->info->vertexSize * _index];

	for(i = 0; i < _mesh->info->vertexSize; i++) 
		selected[i] = component[i];
}

void grxSetMeshTexCoordComponents(grxMesh* _mesh, uint64_t _index, double _x, double _y) {
#define GRX_MESH_TEXCOORDS_SIZE 2
	static double component[GRX_MESH_TEXCOORDS_SIZE];
	static double* selected = NULL;
	static int i = 0;

	if(_index >= GRX_MESH_TEXCOORDS_SIZE) {
		grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
		return;
	}

	component[0] = _x;
	component[1] = _y; 

	selected = &_mesh->info->texcoords[GRX_MESH_TEXCOORDS_SIZE * _index];

	for(i = 0; i < GRX_MESH_TEXCOORDS_SIZE; i++) 
		selected[i] = component[i];
#undef GRX_MESH_TEXCOORDS_SIZE
}

void grxSetMeshColorComponents(grxMesh* _mesh, uint64_t _index, uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) {
#define GRX_MESH_COLOR_SIZE 4
	static uint8_t component[GRX_MESH_COLOR_SIZE];
	static uint8_t* selected = NULL;
	static int i = 0;

	if(_index >= GRX_MESH_COLOR_SIZE) {
		grxSetError(GRX_ERR_INDEX_OUT_OF_BOUNDS);
		return;
	}

	component[0] = _r;
	component[1] = _g;
	component[2] = _b;
	component[3] = _a; 

	selected = &_mesh->info->colors[GRX_MESH_COLOR_SIZE * _index];

	for(i = 0; i < GRX_MESH_COLOR_SIZE; i++) 
		selected[i] = component[i];
#undef GRX_MESH_COLOR_SIZE
}

void grxSetMeshVertex(grxMesh* _mesh, uint64_t _index, grxVector* _vertex) {
	grxSetMeshVertexComponents(_mesh, _index, _vertex->x, _vertex->y, _vertex->z);
}

void grxSetMeshNormal(grxMesh* _mesh, uint64_t _index, grxVector* _vertex) {
	grxSetMeshNormalComponents(_mesh, _index, _vertex->x, _vertex->y, _vertex->z);
}

void grxSetMeshTexCoords(grxMesh* _mesh, uint64_t _index, grxPoint* _point) {
	grxSetMeshTexCoordComponents(_mesh, _index, _point->x, _point->y);
}

void grxSetMeshColors(grxMesh* _mesh, uint64_t _index, grxColor* _color) {
	grxSetMeshColorComponents(_mesh, _index, _color->r, _color->g, _color->b, _color->a);
}

void grxGetMeshVertex(grxMesh* _mesh, uint64_t _index, void* _result, uint64_t _resultSize) {
	static double* selected = NULL;
	static double* result = NULL;
	static int i = 0;

	if(_resultSize != (_mesh->info->vertexSize * sizeof(double))) {
		grxSetError(GRX_ERR_INVALID_VALUE);
		return;
	}

	if(_index >= _mesh->info->vertexCount) {
		grxSetError(GRX_ERR_INVALID_VALUE);
		return;
	}

	selected = &_mesh->info->vertices[_index * _mesh->info->vertexSize];
	result = (double*) _result;

	for(i = 0; i < _mesh->info->vertexCount; i++) 
		result[i] = selected[i];
}

void grxGetMeshNormal(grxMesh* _mesh, uint64_t _index, void* _result, uint64_t _resultSize) {
	static double* selected = NULL;
	static double* result = NULL;
	static int i = 0;

	if(_resultSize != (_mesh->info->vertexSize * sizeof(double))) {
		grxSetError(GRX_ERR_INVALID_VALUE);
		return;
	}

	if(_index >= _mesh->info->vertexCount) {
		grxSetError(GRX_ERR_INVALID_VALUE);
		return;
	}

	selected = &_mesh->info->normals[_index * _mesh->info->vertexSize];
	result = (double*) _result;

	for(i = 0; i < _mesh->info->vertexSize; i++) 
		result[i] = selected[i];
}

void grxGetMeshTexCoord(grxMesh* _mesh, uint64_t _index, void* _result, uint64_t _resultSize) {
	static double* selected = NULL;
	static double* result = NULL;
	static int i = 0;

	if(_resultSize != (_mesh->info->texcoordSize * sizeof(double))) {
		grxSetError(GRX_ERR_INVALID_VALUE);
		return;
	}

	if(_index >= _mesh->info->vertexCount) {
		grxSetError(GRX_ERR_INVALID_VALUE);
		return;
	}

	selected = &_mesh->info->texcoords[_index * _mesh->info->texcoordSize];
	result = (double*) _result;

	for(i = 0; i < _mesh->info->texcoordSize; i++) 
		result[i] = selected[i];
}

void grxGetMeshColor(grxMesh* _mesh, uint64_t _index, void* _result, uint64_t _resultSize) {
	static uint8_t* selected = NULL;
	static uint8_t* result = NULL;
	static int i = 0;

	if(_resultSize != (_mesh->info->colorSize * sizeof(uint8_t))) {
		grxSetError(GRX_ERR_INVALID_VALUE);
		return;
	}

	if(_index >= _mesh->info->vertexCount) {
		grxSetError(GRX_ERR_INVALID_VALUE);
		return;
	}

	selected = &_mesh->info->colors[_index * _mesh->info->colorSize];
	result = (uint8_t*) _result;

	for(i = 0; i < _mesh->info->colorSize; i++) 
		result[i] = selected[i];
}

grxVector grxGetMeshCenter(grxMesh* _mesh) {
	return _mesh->center;
}
