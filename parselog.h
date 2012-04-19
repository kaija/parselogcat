#ifndef __PARSELOG_H__
#define __PARSELOG_H__
#include <stdio.h>
#define ERR_MSG_LEN 1024
#define MAX_LINE_LEN 2048
struct parse_var {
    int err_count;
    int start_count;
    FILE *fp;
};

enum{
	PLAT_QCOM,
	PLAT_NVIDIA
};

enum{
    ERR_ZYGOTE_CRASH,
    ERR_APP_ANR,
    ERR_APP_EXCEPTION,
    ERR_VM_SHUTDOWN,
    ERR_MAX
};

enum{
    INFO_SYS_INIT,
    INFO_ZYGOTE_INIT,
    INFO_SYS_SHUTDOWN
};

struct err_map{
    char msg[ERR_MSG_LEN];
};
#endif
