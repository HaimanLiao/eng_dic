#include "linear_file_operate.h"
#include "common.h"

#define LINEAR_TMP_FILE		"linear.tmp"

int linear_file_init(linear_file *file_hd)
{
	if(file_hd->fd != -1)return -1;	

	if((file_hd->head_size < 8) || (file_hd->head_size > 24))
	{
		printf("Illegal Head Size\n");
		return -1;
	}
	file_hd->file_name[FILE_NAME_LEN - 1] = '\0';

	if((file_hd->fd = open(file_hd->file_name, O_RDWR)) == -1)
	{
		printf("%s don't exist\n", file_hd->file_name);
		if((file_hd->fd = open(file_hd->file_name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1)
		{
			printf("%s creates error\n", file_hd->file_name);
			return -1;
		}

		char *file_head = (char *)malloc(file_hd->head_size);			//Put the Head Part in the new created file
		memset(file_head, 0, (size_t)(file_hd->head_size));
		if((write(file_hd->fd, file_head, file_hd->head_size)) == -1)
		{
			close(file_hd->fd);
			file_hd->fd = -1;
			free(file_head);

			printf("%s creates error\n", file_hd->file_name);
			return -1;
		}
		free(file_head);
		file_hd->file_real_size = 0;
		file_hd->file_size = file_hd->file_real_size;

		printf("%s creates successfully\n", file_hd->file_name);
		return OK;
	}
	else
	{
		//If file exists then regard it as linear file, which has a legal head part, and don't qualify its flag number
		if((lseek(file_hd->fd, 0, SEEK_SET)) == -1)return -1;
		if((read(file_hd->fd, &(file_hd->file_real_size), sizeof(file_hd->file_real_size))) != sizeof(file_hd->file_real_size))
		{
			close(file_hd->fd);
			file_hd->fd = -1;

			printf("%s read error\n", file_hd->file_name);
			return -1;
		}
		file_hd->file_size = file_hd->file_real_size;

		printf("%s open successfully\n", file_hd->file_name);
		return OK;
	}
}

int linear_file_get_size(linear_file *file_hd, uint32_t flag)
{
	if(file_hd->fd == -1)return -1;

	if(flag == 0)return (file_hd->file_size);
	else return (file_hd->file_real_size);
}

int linear_file_read(const linear_file *file_hd, uint32_t index, char *buf, uint32_t num)//char * should be void *
{
	int ret = 0;
	
	if(file_hd->fd == -1)return -1;
	//if(num > (file_hd->file_real_size))return -1;
	if(index >= (file_hd->file_real_size))return -1;

	off_t new_offset = file_hd->head_size + index * (file_hd->element_size);
	if((lseek(file_hd->fd, new_offset, SEEK_SET)) == -1)return -1;
	ret = read(file_hd->fd, buf, num * (file_hd->element_size));		//Don't care if element is deleted or not

	if(ret == -1)return -1;					//read error
	else if((ret % (file_hd->element_size)) != 0)return -1;	//file format corrupts
	return ret;						//return the number of bytes that actually read not the number of elements
}

int linear_file_delete(linear_file *file_hd, uint32_t index)
{
	if(file_hd->fd == -1)return -1;
	if(file_hd->file_size == 0)return -1;
	if(index >= (file_hd->file_real_size))return -1;

	off_t new_offset = file_hd->head_size + index * (file_hd->element_size);

	if((lseek(file_hd->fd, new_offset, SEEK_SET)) == -1)return -1;//Don't care if element has been deleted or not
	if((write(file_hd->fd, "\0", 1)) != 1)return -1;

	(file_hd->file_size)--;//Don't "file_real_size--", because the real deletion didn't happen
	return OK;
}

int linear_file_append(linear_file *file_hd, const char *buf)
{
	if(file_hd->fd == -1)return -1;//if the file can be opened then it is linear file and has the head part

//	buf[WORD_NAME_LEN - 1] = '\0';//Don't care about the element feacture of specified linear file
	if((lseek(file_hd->fd, 0, SEEK_END)) == -1)return -1;
	if((write(file_hd->fd, buf, file_hd->element_size)) == -1)return -1;

	(file_hd->file_real_size)++;
	(file_hd->file_size)++;

	if((lseek(file_hd->fd, 0, SEEK_SET)) == -1)return 1;
	if((write(file_hd->fd, &(file_hd->file_real_size), sizeof(file_hd->file_real_size))) == -1)return 1;
	return OK;
}

int linear_file_clean(linear_file *file_hd)
{
	if(file_hd->fd == -1)return 1;
	//if(file_hd->file_size == file_hd->file_real_size)return 0;
	/*
	When you forget clean and exit process, this is error happenning
	The next time you open the file, file_size is not correct because file_real_size is "wrong" as you forget clean last time
	So assume that when using clean, file_size != file_real_size
	the caller knows some operation may cause that file_size != file_real_size, and then the caller can clean in realized condition
	*/

	int fd;
	if((fd = open(LINEAR_TMP_FILE, O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR)) == -1)return 2;
	char *file_head = (char *)malloc(file_hd->head_size);	//Put the Head Part in the new created file
	memset(file_head, 0, (size_t)(file_hd->head_size));
	if((write(fd, file_head, file_hd->head_size)) == -1)
	{
		close(fd);
		free(file_head);

		return 3;
	}

	char *buf = (char *)malloc(file_hd->element_size);
	memset(buf, 0, file_hd->element_size);

	off_t file_offset = file_hd->head_size;
	int k = 0;
	file_hd->file_size = 0;				//Recounting the file_size
	while(true)
	{
		lseek(file_hd->fd, file_offset, SEEK_SET);
		if((k = read(file_hd->fd, buf, file_hd->element_size)) != (file_hd->element_size))
		{
			if(k == -1)return 4;	//read error
			else if(k == 0)break;	//End of file
			else return 5;		//the other value means that the file's format is wrong
		}
		else
		{
			if(buf[0] == '\0');
			else
			{
				if((write(fd, buf, (file_hd->element_size))) == -1) return 6;
				(file_hd->file_size)++;
				//fd is O_APPEND and don't need lseek over the head part
			}
		}

		file_offset += (file_hd->element_size);
	}

	close(file_hd->fd);
	file_hd->fd = -1;

	char command1[50] = {0}; 
	char command2[50] = {0};
	sprintf(command1, "rm %s", file_hd->file_name);
	sprintf(command2, "mv %s %s", LINEAR_TMP_FILE, file_hd->file_name);
	system(command1);
	system(command2);

	if((file_hd->fd = open(file_hd->file_name, O_RDWR)) == -1)return 7;//file_hd->fd changes but the other members of file_hd don't
	file_hd->file_real_size = file_hd->file_size;
	if((lseek(file_hd->fd, 0, SEEK_SET)) == -1)return 8;//file_hd->fd isn't O_APPEND
	if((write(file_hd->fd, &(file_hd->file_real_size), sizeof(file_hd->file_real_size))) == -1)return 9;
	
	return OK;
}

void linear_file_close(linear_file *file_hd)
{
	if((file_hd->fd) != -1)
	{
		close(file_hd->fd);
		file_hd->fd = -1;
	}
}
