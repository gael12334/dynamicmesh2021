#pragma once

#define GRX_PI 3.14159

inline double grxRadToDeg(double _rad) {
	return _rad * 360.0 / (2 * GRX_PI);
}

inline double grxDegToRad(double _deg) {
	return _deg * (2 * GRX_PI) / 360.0;
}

