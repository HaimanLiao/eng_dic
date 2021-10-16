#ifndef LIST_H
#define LIST_H

#include <stdint.h>

typedef struct List{
	char	*ptr;		//Need to be initialized as NULL

	uint32_t head_size;	//Need initialization
	uint32_t element_size;	//Need initialization
	uint32_t size;		//Need initialization (used as malloc parameter)
	uint32_t flag;		//Need initialization (0)

	uint32_t element_num;
	uint32_t element_real_num;
}List;

typedef int(*element_cmp)(const char *buf1, const char *buf2);
typedef int(*element_delete)(char *);
typedef int(*element_qualify)(const char *);

int List_init(List *list_hd);
int List_get_item_num(List *list_hd, uint32_t flag);
int List_append_items(List *list_hd, char *buf, uint32_t num);
int List_get_item_index(List *list_hd, const char *name, element_cmp cmp);
int List_delete_item(List *list_hd, uint32_t index, element_delete dequalify);
int List_clean(List *list_hd, element_qualify is_qualify);
void List_uinit(List *list_hd);

#endif
