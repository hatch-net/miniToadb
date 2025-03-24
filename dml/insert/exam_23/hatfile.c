#include "hatfile.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char DataDir[] = "."; 

int CreateFile(char *filename, int mode)
{
    int fd = -1;
    char filepath[FILE_PATH_MAX_LEN] = {0};

    snprintf(filepath, FILE_PATH_MAX_LEN, "%s/%s", DataDir, filename);

    // 检查文件是否存在
    if (access(filepath, F_OK) == 0) 
    {
        return -1;
    }

    // 创建文件
    fd = open(filepath, O_RDWR | O_CREAT, mode);
    if (fd == -1) 
    {
        return -2;
    }

    return fd;
}


int CloseFile(int fd)
{
    if(fd > 0)
    {
        close(fd);
    }
    return 0;
}

int DeleteFile(char *filename)
{
    int ret = 0;
    char filepath[FILE_PATH_MAX_LEN];

    snprintf(filepath, FILE_PATH_MAX_LEN, "%s/%s", DataDir, filename);

    if (access(filepath, F_OK) != 0) 
    {
        return -1;
    }

    ret = unlink(filepath);
    if (ret != 0) 
    {
        return -1;
    }

    return ret;
}