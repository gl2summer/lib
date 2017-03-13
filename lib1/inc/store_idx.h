#ifndef __STORE_IDX_H__
#define __STORE_IDX_H__



#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"


#define ERASE_SECTOR_SIZE 0x1000UL



void SpiFlash_Write(uint32_t addr, uint8_t*pBuf, uint16_t len);
void SpiFlash_Read(uint32_t addr, uint8_t*pBuf, uint16_t len);
void SpiFlash_Erase(uint32_t EraseAddr);







typedef uint32_t SI_POSITION;

#define MAX_ITEM_DATA_LEN (64)

typedef enum{SI_OK=0, SI_PARAM_ERR, SI_POS_ERR, SI_CHKSUM_ERR, SI_DAT_LEN_ERR, SI_FULL_ERR}SI_ERROR;


typedef __packed struct _STORE_ITEM_HEAD
{
  uint8_t len;
  uint8_t chksum;
}STORE_ITEM_HEAD;

typedef __packed struct
{
  struct _STORE_ITEM_HEAD head;
  uint8_t dat1;
}STORE_ITEM;



#define ITEM_SIZE(si) (si->data_size +sizeof(STORE_ITEM_HEAD))
#define POS_LAST_ITEM(si) (si->flash_size - ITEM_SIZE(si))

typedef struct
{
  uint32_t flash_begin_addr;
  uint32_t flash_size;
  SI_POSITION offset_wr;
  SI_POSITION offset_rd;
  uint8_t data_size;
  uint32_t item_num;
}STORE_IDX;






SI_ERROR si_get_last_pos(STORE_IDX* si, SI_POSITION* pos);
SI_ERROR si_copy_dat_from_pos(STORE_IDX* si, SI_POSITION pos, uint8_t* dat);//get dat from special position
void si_reset_auto_get(STORE_IDX* si);//reset pointer to zero, for auto get
SI_ERROR si_get_dat_auto(STORE_IDX* si, uint8_t* dat);//auto get dat from flash
SI_ERROR si_put_dat(STORE_IDX* si, uint8_t* dat);//auto save dat to flash
SI_ERROR si_clear(STORE_IDX* si);//erase flash
bool si_is_full(STORE_IDX* si);//
uint32_t si_total_capacity(STORE_IDX* si);//number of total erea for store ITEMS
uint32_t si_rest_capacity(STORE_IDX* si);//number of rest erea for store new ITEMS
uint32_t si_item_number(STORE_IDX* si);//number of exist ITEMS
uint32_t si_item_dat_size(STORE_IDX* si);//data_size of item
uint32_t si_init(STORE_IDX* si, uint32_t flash_begin_addr, uint32_t flash_size, uint8_t item_data_size);


#endif //__STORE_IDX_H__



