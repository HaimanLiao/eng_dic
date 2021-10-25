#ifndef DIC_API_H
#define DIC_API_H

#include <stdint.h>

#define WORD_LEN        20      //The actual length is WORD_LEN - 1 = 19
#define EXP_LEN         400     //5 * 400
#define RESERVE_LEN     1000
typedef struct Word{
        char name[WORD_LEN];

        char exp1[EXP_LEN];
        char exp2[EXP_LEN];
        char exp3[EXP_LEN];
        char exp4[EXP_LEN];
        char exp5[EXP_LEN];

        char reserve[RESERVE_LEN];
}word_content;          //Be aware of pack problem if you want to operate struct member by address
#define EXP_NUM         5
#define SENTENCE_START	200

int dic_init(void);
void index_ram_print(void);//for testing
int add_word(const char *buf);
int get_word_index(const char *name);
void print_word_content(uint32_t index);
int delete_word(const char *name);
void list_words(void);
int dic_clean(void);
void dic_exit(void);
int get_word_num(uint32_t flag);

#endif
