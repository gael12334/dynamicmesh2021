#pragma once

#include "../DataStructure/ArrayList.h"
#include "../DataStructure/List.h"
#include "Vector.h"

/*********************************
 * Contains the information for a 
 * 3 points bézier curve.
 *********************************/
typedef struct grxBezierCurve {
	grxVector startPtn;
	grxVector middlePtn;
	grxVector endPtn;
} grxBezierCurve;

void grxCtorBezier(grxBezierCurve* _bezier, grxVector* _start, grxVector* _middle, grxVector* _end) {
	_bezier->startPtn = *_start;
	_bezier->middlePtn = *_middle;
	_bezier->endPtn = *_end;
}

void grxDtorBezier(grxBezierCurve* _bezier) {
	// Empty dtor.
}

grxBezierCurve* grxCreateBezierCurve(grxVector* _start, grxVector* _middle, grxVector* _end) {
	grxBezierCurve* curve = Malloc(grxBezierCurve, 1);
	grxCtorBezier(curve, _start, _middle, _end);
	return curve;
}

void grxDestroyBezierCurve(grxBezierCurve** _bezier) {
	grxDtorBezier(*_bezier);
	free(*_bezier);
	*_bezier = NULL;
}

grxBezierCurve* grxCopyBezierCurve(grxBezierCurve* _bezier) {
	return grxCreateBezierCurve(&_bezier->startPtn, &_bezier->middlePtn, &_bezier->endPtn);
}

/*********************************
 * Contains information for the 
 * creation of a spline made with
 * a bézier curve.
 *********************************/
#define GRX_SPLINE_FRONT 0
#define GRX_SPLINE_BACK 1

typedef struct grxSpline {
	grxArrayList* list;
} grxSpline;

void grxCtorSpline(grxSpline* _spline) {
	_spline->list = grxCreateArrayList();
}

void grxDtorSpline(grxSpline* _spline) {
	static grxAbstract temp; 

	for(int64_t i = 0, m = grxGetArrayListCount(_spline->list); i < m; i++) {
		temp = grxGetArrayListValue_Fast(_spline->list, i);
		grxDestroyVector((grxVector**)&temp.ptr);
	}

	grxDestroyArrayList(&_spline->list);
}

grxSpline* grxCreateSpline() {
	grxSpline* spline = Malloc(grxSpline, 1);
	grxCtorSpline(spline);
	return spline;
}

void grxDestroySpline(grxSpline** _spline) {
	grxDtorSpline(*_spline);
	free(*_spline);
	*_spline = NULL;
}

void grxAddSplineNode(grxSpline* _spline, grxBezierCurve* _bezier, int _where) {
	grxBezierCurve* bezierCopy = grxCreateBezierCurve(&_bezier->startPtn, &_bezier->middlePtn, &_bezier->endPtn);
	(_where == 0) ? 
		grxAddArrayListFront(_spline->list, grxPtrAbstract(bezierCopy)) :
		grxAddArrayListBack(_spline->list, grxPtrAbstract(bezierCopy));
	
	if(grxGetError() != GRX_ERR_NONE)
		grxDestroyBezierCurve(&bezierCopy);
}

void grxRemoveSplineNode(grxSpline* _spline, int _where) {
	(_where == 0) ? 
		grxRemoveArrayListFront(_spline->list) :
		grxRemoveArrayListBack(_spline->list);
}

void grxSplitSpline(grxSpline* _spline, grxSpline** _resultA, grxSpline** _resultB, int64_t _index) {
	if(_index == 0) {
		*_resultA = NULL;
		*_resultB = NULL;
		return;
	}

	if(_index == (grxGetArrayListCount(_spline->list) - 1)) {
		*_resultA = NULL;
		*_resultB = NULL;
		return;
	}

	grxSpline* splineA = grxCreateSpline();
	grxBezierCurve* toCopy = NULL;

	for(int64_t i = 0; i < _index; i++) {
		toCopy = (grxBezierCurve*) grxGetArrayListValue_Fast(_spline->list, _index).ptr;
		grxAddSplineNode(splineA, grxCopyBezierCurve(toCopy), GRX_SPLINE_BACK);
	}

	grxSpline* splineB = grxCreateSpline();
	toCopy = NULL;

	for(int64_t i = _index + 1, m = grxGetArrayListCount(_spline->list); i < m; i++) {
		toCopy = (grxBezierCurve*) grxGetArrayListValue_Fast(_spline->list, _index).ptr;
		grxAddSplineNode(splineB, grxCopyBezierCurve(toCopy), GRX_SPLINE_BACK);
	}

	*_resultA = splineA;
	*_resultB = splineB;
}

grxBezierCurve* grxGetSplineBezier(grxSpline* _spline, int64_t _index) {
	return (grxBezierCurve*) grxGetArrayListValue(_spline->list, _index).ptr;
}

grxList* grxGetSplinePoints(grxSpline* _spline, int64_t _precision) {
	grxList* list = grxCreateList();
	grxBezierCurve* bezier = NULL;
	
	for(int64_t i = 0, m = grxGetArrayListCount(_spline->list); i < m; i++) {
		bezier = grxGetSplineBezier(_spline, i);
		grxVector start, end, result, deltaStart, deltaEnd, deltaMid;
		grxVector* seekedPtn = NULL;

		deltaStart.x = (bezier->middlePtn.x - bezier->startPtn.x) / _precision;
		deltaStart.y = (bezier->middlePtn.y - bezier->startPtn.y) / _precision;
		deltaStart.z = (bezier->middlePtn.z - bezier->startPtn.z) / _precision;

		deltaEnd.x = (bezier->endPtn.x - bezier->middlePtn.x) / _precision;
		deltaEnd.y = (bezier->endPtn.y - bezier->middlePtn.y) / _precision;
		deltaEnd.z = (bezier->endPtn.z - bezier->middlePtn.z) / _precision;
		
		for(int64_t i = 0, m = _precision + 1; i < m; i++) {

			start.x = deltaStart.x * i + bezier->startPtn.x;
			start.y = deltaStart.y * i + bezier->startPtn.y;
			start.z = deltaStart.z * i + bezier->startPtn.z;

			end.x = deltaEnd.x * i + bezier->middlePtn.x;
			end.y = deltaEnd.y * i + bezier->middlePtn.y;
			end.z = deltaEnd.z * i + bezier->middlePtn.z;

			
			deltaMid.x = (end.x - start.x) / _precision;
			deltaMid.y = (end.y - start.y) / _precision;
			deltaMid.z = (end.z - start.z) / _precision;

			result.x = deltaMid.x * i + start.x;
			result.y = deltaMid.y * i + start.y;
			result.z = deltaMid.z * i + start.z;

			/*
			grxCtorVector(&start,
				(((bezier->middlePtn.x - bezier->startPtn.x) / _precision) * i) + bezier->startPtn.x,
				(((bezier->middlePtn.y - bezier->startPtn.y) / _precision) * i) + bezier->startPtn.y,
				(((bezier->middlePtn.z - bezier->startPtn.z) / _precision) * i) + bezier->startPtn.z
			);
			grxCtorVector(&end, 
				(((bezier->endPtn.x - bezier->middlePtn.x) / _precision) * i) + bezier->middlePtn.x,
				(((bezier->endPtn.y - bezier->middlePtn.y) / _precision) * i) + bezier->middlePtn.y,
				(((bezier->endPtn.z - bezier->middlePtn.x) / _precision) * i) + bezier->middlePtn.z
			);
			 
			seekedPtn = grxCreateVector(
				(((end.x - start.x) / _precision) * i) + start.x,
				(((end.y - start.y) / _precision) * i) + start.y,
				(((end.z - start.y) / _precision) * i) + start.z
			);
			*/

			seekedPtn = grxCreateVector(result.x, result.y, result.z);

			grxPushListBack(list, grxPtrAbstract(seekedPtn));
		}
	}

	return list;
}


