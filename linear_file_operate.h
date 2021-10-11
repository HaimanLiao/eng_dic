#ifndef LINEAR_FILE_OPERATE_H
#define LINEAR_FILE_OPERATE_H

#include <stdint.h>

#define FILE_NAME_LEN		20
//This prototype is opened to other C files
typedef struct linear_file{
	int fd;					//Need Initialization (-1)
	uint32_t head_size;			//Need Initialization (>=8 && <=24)
	uint32_t element_size;			//Need Initialization
	char file_name[FILE_NAME_LEN - 1];	//Need Initialization

	uint32_t file_real_size;		//The first 4 bytes of linear file is used to initialized file_real_size
	uint32_t file_size;
	uint32_t flag;				//Use to identify if the file is linear file (Not using yet and leave it for future extention)
}linear_file;
//The first 4 bytes of all kinds of linear files store their element numbers
//So the meaning of the 4 bytes is the same in all kinds of linear file


int linear_file_init(linear_file *file_hd);
int linear_file_get_size(linear_file *file_hd, uint32_t flag);
int linear_file_read(const linear_file *file_hd, uint32_t index, char *buf, uint32_t num);
int linear_file_delete(linear_file *file_hd, uint32_t index);
int linear_file_append(linear_file *file_hd, const char *buf);
int linear_file_clean(linear_file *file_hd);

#endif
