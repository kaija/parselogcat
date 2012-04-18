#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "parselog.h"
#define VERSION "0.1"
#define PGNAME "Parse log tool"
#define DEBUG 0
static struct parse_var gd;
void print_usage(char *name)
{
    printf("%s file\n",name);
}
void init_gd()
{
    gd.err_count = 0;
    gd.start_count = 0;
    gd.fp = NULL;
}
int parse_msg(char *line)
{
    if(line == NULL)
	return -1;
    if( strstr(line, "enum") != NULL )
    {
	printf("enum ya");
    }
}

int file_exist(char *path)
{
    struct stat buffer;
    if(stat(path, &buffer) == 0)
    {
	if(S_ISREG(buffer.st_mode))
	{
	    return 0;
	}
    }
    return -1;
}

int main(int argc, char **argv)
{
    printf("%s version %s\n",PGNAME, VERSION);
    if(argc < 2)
	print_usage(argv[0]);
       
    if(file_exist(argv[1]) != 0)
    {
	printf("file not exist\n");
	return -1;
    }
    init_gd();
    FILE *fp = NULL;
    fp = fopen(argv[1],"r");
    if(fp)
    {
	char buf[MAX_LINE_LEN];
	while(fgets(buf, MAX_LINE_LEN , fp) != NULL)
	{
	    if(DEBUG)
		printf("%s",buf);
	    parse_msg(buf);
	}
	fclose(fp);
    }
    return 0;
}
