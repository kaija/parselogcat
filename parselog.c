#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "parselog.h"
#define VERSION "0.1"
#define PGNAME "Parse log tool"
#define DEBUG 0
static int platform = PLAT_QCOM;
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

void dump_time(char *in)
{
	if(in == NULL)
		return;
	char buf[MAX_LINE_LEN];
	char buf2[MAX_LINE_LEN];
	strncpy(buf2, in , MAX_LINE_LEN);
	memset(buf, 0 , MAX_LINE_LEN);
	char * pch;
	pch=strchr(buf2,' ');
	if(pch != NULL)
	{
		memcpy(buf, buf2 , pch - buf2);
		printf("time:%s\n",buf);
	}
}
void dump_log(char *in)
{
	char buf2[MAX_LINE_LEN];
	memset(buf2, 0, MAX_LINE_LEN);
	memcpy(buf2, in , MAX_LINE_LEN);
	if(in == NULL)
		return;
	int len = 0;
	len = strlen(buf2) + 6;
	buf2[strlen(buf2)-1] = 0;
	int i = 0;
	for(i=0;i<len;i++)
	{
		printf("=");
		fflush(stdout);
	}
	printf("\n=  %s   =\n",buf2);
	for(i=0;i<len;i++)
	{
		printf("=");
	}
	printf("\n\n\n");
}
int parse_msg(char *line)
{
    if(line == NULL)
	return -1;
    if( strstr(line, "Zygote") != NULL )
    {
		if(strstr(line, ": Preloading classes...") != NULL)
		{
			printf("Zygote init\n");
			dump_time(line);
			dump_log(line);
		}
    }
	if( strstr(line, "AndroidRuntime") != NULL)
	{
		if( strstr(line, "FATAL EXCEPTION") != NULL)
		{
			printf("Application fatal exception\n");
			dump_time(line);
			dump_log(line);
		}
	}
	if( strstr(line, "ActivityManager") != NULL)
	{
		if( strstr(line, "ANR") != NULL)
		{
			printf("Activity ANR\n");
			dump_time(line);
			dump_log(line);
		}
#ifdef APP_DIED
		if( strstr(line, "has died.") != NULL)
        {
            printf("Application has died\n");
			dump_time(line);
			dump_log(line);
        }
#endif
	}
	if( strstr(line, "GPT partitions found") != NULL  && platform == PLAT_QCOM)
	{
			printf("System boot completed\n");
			dump_time(line);
			dump_log(line);
	}
	if( strstr(line, "ShutdownThread") != NULL)
	{
		if( strstr(line , "Performing low-level shutdown") != NULL)
		{
			printf("Normal shutdown\n");
			dump_time(line);
			dump_log(line);
		}
	}
	if( strstr(line, "tid")!=NULL && strstr(line, "pid")!=NULL && strstr(line, ">>>")!=NULL && strstr(line, "<<<")!=NULL)
	{
		printf("Daemon crash\n");
		dump_time(line);
		dump_log(line);
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
