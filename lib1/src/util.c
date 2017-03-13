#include "util.h"











uint8_t cal_chksum(uint8_t* dat, uint32_t len, uint8_t chksum0)
{
  if(dat==NULL)
    return 0;
  
  uint8_t chksum = chksum0;
  while(len)
  {
    chksum += dat[len-1];
    len--;
  }
  return chksum;
}







