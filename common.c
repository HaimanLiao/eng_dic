#include "common.h"

//Public variable
uint32_t exit_code = GREEN;

//Private variable
#define BUFFER_SIZE	4096
char input_buf[BUFFER_SIZE] = {0};

//Public function
//Read a string (with '\0') from stard input with removing the newline character
//The max input charater is size and the buf should leave room for character '\0' that would automaticlly be added
int str_input(char *buf, uint32_t size)
{
	int n;
	int num = size + 1;

	if((n = read(STDIN_FILENO, input_buf, BUFFER_SIZE)) == -1)return -1;
	memcpy(buf, input_buf, num);

	if(n < num)buf[n - 1] = '\0';//Remove the newline character
	else buf[num - 1] = '\0';

	memset(input_buf, 0, BUFFER_SIZE);//str_input is probably  used many time in a c file, so input_buf has to be cleared
	return OK;
}
