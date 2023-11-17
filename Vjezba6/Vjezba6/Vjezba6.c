#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATE_LENGTH (11)
#define MAX_LENGTH (50)
#define MAX_LINE (1024)
#define FILE_NOT_OPEN (-1)
#define FAILED_MEMORY_ALLOCATION (NULL)
#define DATE2_LATER (-1)
#define DATE2_EARLIER (1)
#define DATES_EQUAL (0)

struct _article;
typedef struct _article* ArticlePosition;
typedef struct _article
{
	char name[MAX_LENGTH];
	int quantity;
	double price;
	ArticlePosition next;
} Article;