#include "common.h"
#include "dic_api.h"

#define CLEAN_TIME	100
uint32_t Clean_interval = 0;

#define COMMAND_SIZE	20

void buf_print(char *buf, int num)
{
        for(int i = 0; i < num; i++)
        {
                if((i % 8) == 0)printf("\n");

                if(buf[i] == '\0')printf(" \\0   ");
                else if((((int)buf[i]) > 32) && (((int)buf[i]) < 127))printf("  %c   ", buf[i]);
                else printf("0x%2x  ", buf[i]);
        }
        printf("\n");
}

void Print_word_content(const char *print_ptr)
{
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

void add_command(const char *word)
{
	int word_index = get_word_index(word);
	if(word_index != -1)
	{
		printf("%s already exists\n", word);
		print_word_content(word_index);
		return;
	}

        char *voc_element = (char *)malloc(sizeof(word_content));
        memset(voc_element, 0, sizeof(word_content));
        char *voc_element_ptr = voc_element;

	strcpy(voc_element, word);
        voc_element_ptr += WORD_LEN;
        for(int i = 0; i < EXP_NUM; i++)
        {
                printf("Please input #%s# definition %d:\n", word, i + 1);
                str_input(voc_element_ptr, SENTENCE_START - 1);//Leave the room for '\0'
		
		if(voc_element_ptr[0] == CTRL_PLUS_F)
		{
			voc_element_ptr[0] = '\0';
			break;
		}
		if(voc_element_ptr[0] == CTRL_PLUS_X)
		{
			free(voc_element);
			return;
		}

                printf("Please input #%s# example sentence %d:\n", word, i + 1);
                str_input(voc_element_ptr + SENTENCE_START, EXP_LEN - SENTENCE_START - 1);//Also leave the room for '\0'

                if((voc_element_ptr + SENTENCE_START)[0] == CTRL_PLUS_F)
                {
			(voc_element_ptr + SENTENCE_START)[0] = '\0';
                        break;
                }
                if((voc_element_ptr + SENTENCE_START)[0] == CTRL_PLUS_X)
                {
                        free(voc_element);
                        return;
                }

                voc_element_ptr += EXP_LEN;
        }
	Print_word_content(voc_element);

	char tmp_char = '\0';
	printf("Confirm the content you want to save and input y/n to decide to save it or not\n");
	scanf(" %c", &tmp_char);
	if(tmp_char == 'y')
	{
		//buf_print(voc_element, sizeof(word_content));
		if(add_word(voc_element) == OK)
		{
			printf("Add OK\n");
		}
	}
        
        free(voc_element);
}

void search_command(const char *word)
{
	int index = get_word_index(word);

	if(index == -1)
	{
		printf("#%s# doesn't exist in dictionary file\n", word);
		return;
	}

	print_word_content(index);
}

void delete_command(const char *word)
{
	if(delete_word(word) == 0)
	{
		printf("#%s# is deleted\n", word);
		Clean_interval += 1;
	}
	else printf("%s doesn't exist in dictionary file\n", word);
}

void list_command(void)
{
	list_words();
}

void quit_command(void)
{
	//Nothing for now
}

int main(void)
{
	if(dic_init() == -1)
	{
		printf("Red Error\n");
		//printf("%d\n", exit_code);
		//return -1;
		goto end;
	}

	//index_ram_print();
	//if(dic_clean() == -1)goto end;//Clean in dic_init()

start:
	printf("\nPlease input one of commands as follow:\n");
	printf("\tsearch [enter] [word]\n");
	printf("\tadd [enter] [word]\n");
	printf("\tdelete [enter] [word]\n");
	printf("\tlist\n");
	printf("\tquit\n");

	char command[COMMAND_SIZE] = {0};
	char word[WORD_LEN] = {0};
	
	str_input(command, COMMAND_SIZE - 1);
	if(strcmp(command, "add") == OK)
	{
		str_input(word, WORD_LEN - 1);
		add_command(word);
	}
	else if(strcmp(command, "search") == OK)
	{
		str_input(word, WORD_LEN - 1);
		search_command(word);
	}
	else if(strcmp(command, "delete") == OK)
	{
		str_input(word, WORD_LEN - 1);
		delete_command(word);
	}
	else if(strcmp(command, "list") == OK)list_command();
	else if(strcmp(command, "quit") == OK)
	{
		quit_command();
		goto end;
	}
	else printf("Command \"%s\" is not found\n", command);

	if(Clean_interval > CLEAN_TIME)
	{
		dic_clean();
		Clean_interval = 0;
	}

	goto start;

end:
//	dic_clean();	//Clean at the start of program
	dic_exit();
	return 0;
}
