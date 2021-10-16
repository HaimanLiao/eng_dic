#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>

#define OK	0
#define ERROR	-1

#define GREEN	0
#define YELLOW	1
#define RED	2
extern uint32_t exit_code;

int str_input(char *buf, uint32_t num);

#endif
