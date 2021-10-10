#ifndef LINEAR_FILE_OPERATE_H
#define LINEAR_FILE_OPERATE_H

#define FILE_NAME_LEN		20
//This prototype is opened to other C files
typedef struct linear_file{
	int fd;					//Need Initialization (-1)
	unsigned int head_size;			//Need Initialization (>=8 && <=24)
	int element_size;			//Need Initialization
	char file_name[FILE_NAME_LEN - 1];	//Need Initialization

	int file_real_size;
	int file_size;
	int flag;				//Use to identify if the file is linear file (Not using yet and leave it for future extention)
}linear_file;
//The head part in linear file is used to store file_real_size(front) and flag, and each of them occupy 4 bytes (in case of sizeof(int) = 4)


int linear_file_init(linear_file *file_hd);
int linear_file_read(const linear_file *file_hd, unsigned int index, char *buf, unsigned int num);
int linear_file_delete(linear_file *file_hd, unsigned int index);
int linear_file_add(linear_file *file_hd, const char *buf);
int linear_file_clean(linear_file *file_hd);

#endif
