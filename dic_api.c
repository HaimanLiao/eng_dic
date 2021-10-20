#include "common.h"
#include "List.h"
#include "linear_file_operate.h"
#include "dic_api.h"


#define VOC_FILE	"voc.dic"
linear_file voc_dic_hd = {-1, 16, sizeof(word_content), VOC_FILE, 0, 0, 0};
linear_file *voc_dic = &voc_dic_hd;

//char index_element[WORD_LEN];
#define INDEX_FILE	"index.dic"
linear_file index_dic_hd = {-1, 16, WORD_LEN, INDEX_FILE, 0, 0, 0};
linear_file *index_dic = &index_dic_hd;

#define RAM_SIZE	1000000
//char list_element[WORD_LEN] = {0};
List index_ram_hd = {NULL, 8, WORD_LEN, RAM_SIZE, 0};
List *index_ram = &index_ram_hd;


#define ELE_LOAD_NUM	100		//ELE_LOAD_NUM elements (not byte) are loaded each time
int dic_init(void)
{
	if(linear_file_init(voc_dic) == -1)
	{
		exit_code = RED;
		return -1;
	}
	if(linear_file_init(index_dic) == -1)
	{
		exit_code = RED;
		return -1;
	}
	if(linear_file_clean(index_dic) == -1)
        {
                exit_code = RED;
                return -1;
        }
        if(linear_file_clean(voc_dic) == -1)
        {
                exit_code = RED;
                return -1;
        }

	if(List_init(index_ram) == -1)//If return OK, ptr is pointed to a ram initialized by 0
	{
		exit_code = YELLOW;
		return -1;
	}
	//Load the file index.dic to index_ram
	char *ram_ptr = index_ram->ptr + index_ram->head_size;
	int index = 0;
	uint32_t ret = 0;
	uint32_t tmp_int = ELE_LOAD_NUM * (index_ram->element_size);
	while(index < (index_dic->file_real_size))
	{
		ret = linear_file_read(index_dic, index, ram_ptr, ELE_LOAD_NUM);
		if(ret == -1)
		{
			exit_code = YELLOW;
			return -1;
		}
/*		
		if((ret % WORD_LEN) != 0)//Belive "linear_file_read" can detect this error
		{
			exit_code = YELLOW;
			return -1;
		}
*/
		else if(ret < tmp_int)break;
		else
		{
			index += ELE_LOAD_NUM;
			ram_ptr += tmp_int;
		}
	}
	index_ram->element_real_num = index_dic->file_real_size;
	index_ram->element_num = index_ram->element_real_num;

	return OK;
}

//Print the all elements in index_ram (without printing the head part and the part that not places elements)
//If no elements, then there is nothing to be printed
//This is a test fuction (you can consider removing it when test is over or keep it for the future version test)
void index_ram_print(void)
{
	if(index_ram->ptr == NULL)return;

	char *buf = index_ram->ptr + index_ram->head_size;
	uint32_t tmp_int = (index_ram->element_real_num) * (index_ram->element_size);
        for(int i = 0; i < tmp_int; i++)
        {
                if(((i % 8) == 0) && (i != 0))printf("\n");

                if(buf[i] == '\0')printf(" \\0   ");
                else if((((int)buf[i]) > 32) && (((int)buf[i]) < 127))printf("  %c   ", buf[i]);
                else printf("0x%2x  ", buf[i]);
        }
        printf("\n");
}

void list_words(void)
{
	char *ram_ptr = index_ram->ptr + (index_ram->head_size);

	for(int i = 0; i < (index_ram->element_real_num); i++)
	{
		if(ram_ptr[0] == '\0');
		else printf("# %s\n", ram_ptr);

		ram_ptr += (index_ram->element_size);
	}
}

int get_word_index(const char *name)
{
	return List_get_item_index(index_ram, name, strcmp);
}

int add_word(const char *buf)//buf is a string which the caller needs to make sure its qualification (the function don't check this)
{
	char name[WORD_LEN];
	memcpy(name, buf, WORD_LEN);

	if(linear_file_append(voc_dic, buf) == -1)
	{
		exit_code = RED;
		return -1;
	}
	if(linear_file_append(index_dic, name) == -1)
	{
		exit_code = RED;
		return -1;
	}
	if(List_append_items(index_ram, name, 1) == -1)
	{
		exit_code = YELLOW;
		return -1;
	}

	return OK;
}

void print_word_content(uint32_t index)
{
	char buf[sizeof(word_content)];
	memset(buf, 0, sizeof(word_content));

	if(linear_file_read(voc_dic, index, buf, 1) == -1)
	{
		exit_code = YELLOW;
		return;
	}

	char *print_ptr = buf;
	
	printf("# %s #\n", print_ptr);
        print_ptr += WORD_LEN;
        for(int i = 0; i < EXP_NUM; i++)
        {
                if(print_ptr[0] == '\0')
                {
                        if((print_ptr + SENTENCE_START)[0] == '\0')return;
                        printf("\t$ %s\n", (print_ptr + SENTENCE_START));//some words may not have definition but have example sentence
                }
                else
                {
                        printf("%s\n", print_ptr);
                        printf("\t$ %s\n", (print_ptr + SENTENCE_START));
                }

                print_ptr += EXP_LEN;
        }
}

int de_qualify(char *element)//Private fuction
{
	element[0] = '\0';
	return OK;
}
int delete_word(const char *name)
{
	int index;
	if((index = get_word_index(name)) == -1)return 1;//word does't exist

	if(linear_file_delete(voc_dic, index) == -1)
	{
		exit_code = RED;
		return -1;
	}
	if(linear_file_delete(index_dic, index) == -1)
	{
		exit_code = RED;
		return -1;
	}
	if(List_delete_item(index_ram, index, de_qualify) == -1)
	{
		exit_code = YELLOW;
		return -1;
	}

	return OK;
}

int is_qualify(const char *buf)//Private function
{
	if(buf[0] == '\0')return -1;
	else return OK;
}
int dic_clean(void)
{
	if(linear_file_clean(index_dic) == -1)
	{
		exit_code = RED;
		return -1;
	}
	if(linear_file_clean(voc_dic) == -1)
	{
		exit_code = RED;
		return -1;
	}
	if(List_clean(index_ram, is_qualify) == -1)
	{
		exit_code = YELLOW;
		return -1;
	}

	return 0;
}

void dic_exit(void)
{
	List_uinit(index_ram);
	linear_file_close(index_dic);
	linear_file_close(voc_dic);
}
