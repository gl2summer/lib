#include "store_idx.h"
#include "util.h"
#include "string.h"


SI_ERROR si_get_last_pos(STORE_IDX* si, SI_POSITION* pos)
{
  if((si==NULL)||(pos==NULL))
  {
    return SI_PARAM_ERR;
  }
  if(si->offset_wr < (si->data_size+sizeof(STORE_ITEM_HEAD)))
  {
    return SI_POS_ERR;
  }
  
  (*pos) = si->offset_wr - (si->data_size+sizeof(STORE_ITEM_HEAD));
  return SI_OK;
}

SI_ERROR si_copy_dat_from_pos(STORE_IDX* si, SI_POSITION pos, uint8_t* dat)
{
  if((si==NULL)||(dat==NULL))
  {
    return SI_PARAM_ERR;
  }
  if(pos > (si->offset_wr - (si->data_size+sizeof(STORE_ITEM_HEAD))))
  {
    return SI_POS_ERR;
  }

  uint32_t addr = si->flash_begin_addr + pos;
  uint32_t item_size = si->data_size +sizeof(STORE_ITEM_HEAD);
  STORE_ITEM* item;
  uint8_t buff[MAX_ITEM_DATA_LEN + sizeof(STORE_ITEM_HEAD)];
  
  SpiFlash_Read(addr, buff, item_size);
  item = (STORE_ITEM*)buff;
  
  if(item->head.len != si->data_size)
  {
    return SI_DAT_LEN_ERR;
  }
  if(item->head.chksum != cal_chksum(&(item->dat1), item->head.len, 0))
  {
    return SI_CHKSUM_ERR;
  }
  
  memcpy(dat, &(item->dat1), item->head.len);
  return SI_OK;
}




void si_reset_auto_get(STORE_IDX* si)
{
  if(si==NULL)
  {
    return;
  }
  si->offset_rd = 0;
}

SI_ERROR si_get_dat_auto(STORE_IDX* si, uint8_t* dat)
{
  if((si==NULL)||(dat==NULL))
  {
    return SI_PARAM_ERR;
  }
  if(si->offset_rd >= si->offset_wr)
  {
    return SI_POS_ERR;
  }

  uint32_t addr = si->flash_begin_addr + si->offset_rd;
  uint32_t item_size = si->data_size +sizeof(STORE_ITEM_HEAD);
  STORE_ITEM* item;
  uint8_t buff[MAX_ITEM_DATA_LEN + sizeof(STORE_ITEM_HEAD)];
  
  SpiFlash_Read(addr, buff, item_size);
  si->offset_rd += item_size;
  
  item = (STORE_ITEM*)buff;
  if(item->head.len != si->data_size)
  {
    return SI_DAT_LEN_ERR;
  }
  if(item->head.chksum != cal_chksum(&(item->dat1), item->head.len, 0))
  {
    return SI_CHKSUM_ERR;
  }
  
  memcpy(dat, &(item->dat1), item->head.len);
  return SI_OK;
}


SI_ERROR si_put_dat(STORE_IDX* si, uint8_t* dat)
{
  if((si==NULL)||(dat==NULL))
  {
    return SI_PARAM_ERR;
  }
  if(si->offset_wr > si->flash_size - si->data_size)
  {
    return SI_FULL_ERR;
  }

  uint32_t addr = si->flash_begin_addr + si->offset_wr;
  uint32_t item_size = si->data_size +sizeof(STORE_ITEM_HEAD);
  STORE_ITEM item;
  uint8_t buff[MAX_ITEM_DATA_LEN + sizeof(STORE_ITEM_HEAD)];
  
  item.head.len = si->data_size;
  item.head.chksum = cal_chksum(dat, si->data_size, 0);
  memcpy(buff, &(item.head), sizeof(item.head));
  memcpy(buff+sizeof(item.head), dat, si->data_size);
  SpiFlash_Write(addr, buff, item_size);
  si->offset_wr += item_size;
  si->item_num++;
  
  return SI_OK;
}


SI_ERROR si_clear(STORE_IDX* si)
{
  if(si==NULL)
  {
    return SI_PARAM_ERR;
  }
  
  uint32_t offset;
  offset = 0;
  while(offset < si->flash_size)
  {
    SpiFlash_Erase(si->flash_begin_addr +offset);
    offset += ERASE_SECTOR_SIZE;
  }
  
  si->item_num = 0;
  si->offset_rd = 0;
  si->offset_wr = 0;
  return SI_OK;
}


bool si_is_full(STORE_IDX* si)
{
  if(si==NULL)
  {
    return false;
  }
  return (si->offset_wr > (si->flash_size - ITEM_SIZE(si)));
}


uint32_t si_total_capacity(STORE_IDX* si)//
{
  if(si==NULL)
  {
    return 0;
  }
  
  return (si->flash_size/ITEM_SIZE(si));
}


uint32_t si_rest_capacity(STORE_IDX* si)//
{
  if(si==NULL)
  {
    return 0;
  }
  
  return ((si->flash_size -si->offset_wr)/ITEM_SIZE(si));
}


uint32_t si_item_number(STORE_IDX* si)
{
  if(si==NULL)
  {
    return 0;
  }
  
  return si->item_num;
}

uint32_t si_item_dat_size(STORE_IDX* si)
{
  if(si==NULL)
  {
    return 0;
  }
  return si->data_size;
}


uint32_t si_init(STORE_IDX* si, uint32_t flash_begin_addr, uint32_t flash_size, uint8_t item_data_size)
{
  if(si==NULL)
  {
    return false;
  }
  if(item_data_size > MAX_ITEM_DATA_LEN)
  {
    return false;
  }
  
  si->flash_begin_addr = flash_begin_addr;
  si->flash_size = flash_size;
  si->offset_wr = 0;
  si->offset_rd = 0;
  si->data_size = item_data_size;
  si->item_num = 0;
  
  uint32_t addr;
  STORE_ITEM* item;
  uint8_t item_size;
  uint8_t buff[MAX_ITEM_DATA_LEN + sizeof(STORE_ITEM_HEAD)];
  
  item_size = si->data_size +sizeof(STORE_ITEM_HEAD);
  
  while(si->offset_wr <= (si->flash_size - item_size))
  {
    addr = si->flash_begin_addr +si->offset_wr;
    SpiFlash_Read(addr, buff, item_size);
    item = (STORE_ITEM*)buff;

    if((item->head.len != si->data_size)
      ||(item->head.chksum != cal_chksum(&(item->dat1), item->head.len, 0)))
    {
      break;
    }
    
    si->offset_wr += item_size;
    si->item_num++;
  }
  
  return si->item_num;
}





