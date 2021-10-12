#ifndef DIC_API_H
#define DIC_API_H

#include <stdint.h>

int dic_init(void);
void index_ram_print(char *buf, uint32_t num);//for testing
int add_word(const char *buf);
int get_word_index(const char *name);
void print_word_content(uint32_t index);
int delete_word(const char *name);
int dic_clean(void);
void dic_exit(void);

#endif
