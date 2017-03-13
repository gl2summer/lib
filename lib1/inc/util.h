#ifndef _UTIL_H__
#define _UTIL_H__


#include "stdint.h"
#include "stdio.h"





#define ARRAY_COUNT(array) (sizeof(array)/sizeof(array[0]))





uint8_t cal_chksum(uint8_t* dat, uint32_t len, uint8_t chksum0);


#endif //_UTIL_H__


