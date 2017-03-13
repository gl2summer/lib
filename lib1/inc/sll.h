#ifndef _SLL_H__
#define _SLL_H__

#include "stdint.h"
#include "stdbool.h"

extern void *sys_malloc(uint16_t size);
extern void sys_free(void *buffer);

typedef struct _SLL
{
  void* pData;
  struct _SLL *pNext;
}SLL;

typedef struct
{
  uint32_t data;
  uint32_t length;
  struct _SLL *pFirst;
}SLL_LIST;


void SLL_list_init(SLL_LIST* list, uint32_t dat);
SLL_LIST* SLL_list_create(void);
void SLL_list_release(SLL_LIST* list);
void SLL_list_set_dat(SLL_LIST* list, uint32_t dat);
uint32_t SLL_list_get_dat(SLL_LIST* list);
SLL* SLL_create_node(void *pDat, uint32_t dat_len);
bool SLL_set_node_dat(SLL* node, void *pDat, uint32_t dat_len);
void* SLL_get_node_dat(SLL* node);
void SLL_release_node(SLL* node, bool b_free_dat);
void SLL_remove_all(SLL_LIST* list, bool b_free_dat);
bool SLL_put(SLL_LIST* list, SLL* node);
SLL* SLL_get(SLL_LIST* list);
SLL* SLL_take(SLL_LIST* list);
bool SLL_follow(SLL* node_front, SLL* node_behind);

#endif //_SLL_H__
