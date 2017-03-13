#include "sll.h"
#include <string.h>

void SLL_list_init(SLL_LIST* list, uint32_t dat)
{
  if(list)
  {
    list->pFirst = NULL;
  }
}

SLL_LIST* SLL_list_create(void)
{
  SLL_LIST* list;
  list = (SLL_LIST*)sys_malloc(sizeof(SLL_LIST));
  if(list==NULL)
    return NULL;
  
  list->length = 0;
  list->pFirst = NULL;
  list->data = 0;
  return list;
}

void SLL_list_release(SLL_LIST* list)
{
  if(list)
  {
    sys_free(list);
  }
}


void SLL_list_set_dat(SLL_LIST* list, uint32_t dat)
{
  if(list)
  {
    list->data = dat;
  }
}
uint32_t SLL_list_get_dat(SLL_LIST* list)
{
  if(list)
  {
    return list->data;
  }
  return 0;
}

SLL* SLL_create_node(void *pDat, uint32_t dat_len)
{
  SLL* node;
  
  node = (SLL*)sys_malloc(sizeof(SLL));
  if(node==NULL)
  {
    return NULL;
  }
  if((pDat==NULL)&&(dat_len>0))
  {
    pDat = sys_malloc(dat_len);
    if(pDat==NULL)
    {
      sys_free(node);
      return NULL;
    }
  }
  
  node->pData = pDat;
  node->pNext = NULL;
  return node;
}

bool SLL_set_node_dat(SLL* node, void *pDat, uint32_t dat_len)
{
  if((node==NULL)||(pDat==NULL)||(dat_len==0))
  {
    return false;
  }
  
  memcpy(node->pData, pDat, dat_len);
  return true;
}

void* SLL_get_node_dat(SLL* node)
{
  if(node==NULL)
  {
    return NULL;
  }
  return node->pData;
}


void SLL_release_node(SLL* node, bool b_free_dat)
{
  if(node)
  {
    if((b_free_dat)&&(node->pData))
    {
      sys_free(node->pData);
    }
    sys_free(node);
  }
}


void SLL_remove_all(SLL_LIST* list, bool b_free_dat)
{
  SLL *node, *node_next;
  
  if(list!=NULL)
  {
    node = list->pFirst;
    node_next = node->pNext;
    while(node!=NULL)
    {
      if((b_free_dat)&&(node->pData))
      {
        sys_free(node->pData);
      }
      sys_free(node);
      node = node_next;
      node_next = node_next->pNext;
    }
    list->pFirst = NULL;
  }
}


bool SLL_put(SLL_LIST* list, SLL* node)//put to tail
{
  if((list==NULL)||(node==NULL))
  {
    return false;
  }
  if(list->pFirst==NULL)
  {
    list->pFirst = node;
    return true;
  }
  
  SLL* node_cur;
  node_cur = list->pFirst;
  while(node_cur->pNext!=NULL)
  {
    node_cur = node_cur->pNext;
  }
  node_cur->pNext = node;
  return true;
}

SLL* SLL_get(SLL_LIST* list)//get head
{
  if(list==NULL)
  {
    return NULL;
  }
  
  return list->pFirst;
}

SLL* SLL_take(SLL_LIST* list)//take head
{
  if(list==NULL)
  {
    return NULL;
  }
  
  SLL* node;
  node = list->pFirst;
  
  if(node)
  {
    list->pFirst = node->pNext;
  }
  
  return node;
}

bool SLL_follow(SLL* node_front, SLL* node_behind)
{
  if((node_front==NULL)||(node_behind==NULL))
  {
    return false;
  }
  
  node_behind->pNext = node_front->pNext;
  node_front->pNext = node_behind;
  return true;
}



