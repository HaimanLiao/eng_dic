#include "common.h"
#include "linear_file_operate.h"

#define TEST_FILE_HEAD_LEN	20
#define WORD_NAME_LEN		20		//The actual word_name_len is 19
#define TEST_FILE_NAME		"LFO_test_file"

typedef struct Index{
	char name[WORD_NAME_LEN];
	int number;		//Not Use Currently
}word_index;

linear_file test_file = {-1, TEST_FILE_HEAD_LEN, sizeof(word_index), TEST_FILE_NAME};

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


int main(void)
{
	linear_file *test_file_hd = &test_file;
	int test_code;

start:
	printf("Please input a number to decide which function you want to test\n");

	printf("1 -- linear_file_init\n");

	printf("2 -- linear_file_add\n");
	char str1[20] = {0};		//WORD_NAME_LEN = 20
	word_index buf1 = {{0}, 0};

	printf("3 -- linear_file_read\n");
	int num1;
	int index1;
	int ret1;
	char *buf2 = NULL;

	printf("4 -- linear_file_delete\n");
	int index2;
	printf("5 -- linear_file_clean\n\n");
	printf("6 -- exit\n");

	scanf("%d", &test_code);
	switch(test_code)
	{
		case 1:
			if(linear_file_init(test_file_hd) == -1)
			{
				printf("Fail\n");
				goto start;
			}
			printf("file_name:\t%s\n", test_file_hd->file_name);
			printf("file_real_size: %d\n", test_file_hd->file_real_size);
			goto start;
		case 2:
			printf("Please input word name\n");
			scanf("%19s", str1);
			strcpy(buf1.name, str1);
			printf("%s\n", buf1.name);
			if(linear_file_add(test_file_hd, buf1.name) == -1)printf("Add Fail\n");
			else
			{
				printf("Add OK\n");
				printf("file_real_size: %d\n", test_file_hd->file_real_size);
			}
			goto start;
		case 3:
			printf("Please input parameter: index\n");
			scanf("%d", &index1);
			printf("Please input parameter: num\n");
			scanf("%d", &num1);
			buf2 = (char *)malloc(num1 * test_file_hd->element_size);
			memset(buf2, 0, num1 * test_file_hd->element_size);

			if((ret1 = linear_file_read(test_file_hd, index1, buf2, num1)) == -1)printf("Read Fail\n");
			else 
			{
				printf("Read %d bytes, with each element being %d bytes", ret1, test_file_hd->element_size);
				buf_print(buf2, ret1);
			}
			goto start;
		case 4:
			printf("Please input parameter: index\n");
			scanf("%d", &index2);
			if(linear_file_delete(test_file_hd, index2) == 0)printf("Deleted\n");
			else printf("Delete Fails\n");
			goto start;
		case 5:
			if(linear_file_clean(test_file_hd) == 0)
			{
				printf("Cleaned\n");
				printf("file_real_size: %d\n", test_file_hd->file_real_size);
				printf("file_size:\t%d\n", test_file_hd->file_size);
			}
			else printf("Clean Fails\n");
			goto start;
		case 6:
			goto end;
		default:
			goto start;
	}

end:
	free(buf2);
	return 0;
}
