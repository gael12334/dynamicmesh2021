#pragma once

#define GRX_ERR_NONE 0
#define GRX_ERR_INDEX_OUT_OF_BOUNDS 1
#define GRX_ERR_NULL_REFERENCE 2
#define GRX_ERR_INVALID_VALUE 3
#define GRX_ERR_INVALID_RESULT 4
#define GRX_ERR_SPECIFIC_ERROR 5
#define GRX_ERR_OVERFLOW 6
#define GRX_ERR_UNDERFLOW 7

uint32_t __grx_last_error = 0;
bool __grx_Modified_Since_Last_Read = false;

uint32_t grxGetError() {
	uint32_t ret = (__grx_Modified_Since_Last_Read) ? __grx_last_error : GRX_ERR_NONE;
	__grx_Modified_Since_Last_Read = false;
	return ret;
}

uint32_t grxGetLastError() {
	return __grx_last_error;
}

void grxSetError(uint32_t _err) {
	__grx_last_error = _err;
	__grx_Modified_Since_Last_Read = true;	
}