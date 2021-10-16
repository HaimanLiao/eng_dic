#include "common.h"
#include "List.h"
#include <stdint.h>

int List_init(List *list_hd)
{
	if(list_hd->ptr != NULL)return -1;

	list_hd->ptr = (char *)malloc(list_hd->size);
	memset(list_hd->ptr, 0, list_hd->size);//Implicitly assign the head part

	return OK;
}

int List_get_item_num(List *list_hd, uint32_t flag)
{
	if(list_hd->ptr == NULL)return -1;
	
	if(flag == 0)return list_hd->element_num;
	else return list_hd->element_real_num;
}

int List_append_items(List *list_hd, char *buf, uint32_t num)
{
	if(list_hd->ptr == NULL)return -1;

	char *tmp_ptr = (list_hd->ptr) + (list_hd->head_size) + (list_hd->element_real_num) * (list_hd->element_size);
	memcpy(tmp_ptr, buf, num * (list_hd->element_size));//Could cause memory corrupt if the size allocated for ram isn't enough

	(list_hd->element_real_num) += num;
	(list_hd->element_num) += num;

	return 0;
}

int List_get_item_index(List *list_hd, const char *name, element_cmp cmp)
{
	if(list_hd->ptr == NULL)return -1;

	char *element_ptr = list_hd->ptr + list_hd->head_size;

	for(int i = 0; i < (list_hd->element_real_num); i++)
	{
		if(cmp(element_ptr, name) == OK)return i;
		element_ptr += (list_hd->element_size);
	}

	return -1;
}

int List_delete_item(List *list_hd, uint32_t index, element_delete de_qualify)
{
	if(list_hd->ptr == NULL)return -1;
	if(index >= (list_hd->element_real_num))return -1;

	char *element_ptr = (list_hd->ptr) + (list_hd->head_size) + index * (list_hd->element_size);
	if(de_qualify(element_ptr) == OK)
	{
		(list_hd->element_num)--;
		return OK;
	}

	return -1;
}

int List_clean(List *list_hd, element_qualify is_qualify)
{
	char *new_ptr = malloc(list_hd->size);
	memset(new_ptr, 0, list_hd->size);

	//copy the head part

	char *ptr1 = new_ptr + list_hd->head_size;
	char *ptr2 = list_hd->ptr + list_hd->head_size;

	for(int i = 0; i < (list_hd->element_real_num); i++)
	{
		if(is_qualify(ptr2) == OK)
		{
			memcpy(ptr1, ptr2, list_hd->element_size);
			ptr1 += (list_hd->element_size);
		}
		ptr2 += (list_hd->element_size);
	}

	free(list_hd->ptr);
	list_hd->ptr = new_ptr;
	list_hd->element_real_num = list_hd->element_num;

	return OK;
}

void List_uinit(List *list_hd)
{	
	if(list_hd->ptr == NULL)return;

	free(list_hd->ptr);
	list_hd->ptr = NULL;
}

