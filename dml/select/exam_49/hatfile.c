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

int OpenFile(char *filename, int mode)
{    
    int fd = -1;
    char filepath[FILE_PATH_MAX_LEN];

    snprintf(filepath, FILE_PATH_MAX_LEN, "%s/%s", DataDir, filename);

    if (access(filepath, F_OK) != 0) 
    {
        return -1;
    }

    fd = open(filepath, O_RDWR, mode);
    if (fd == -1) 
    {
        return -1;
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

int ReadFile(int fd, unsigned long offset, int size, char *page)
{
    int readSize = 0;
	long realLen = 0;

    if(fd < 0 || page == NULL)
        return -1;

    realLen = lseek(fd, offset, SEEK_SET);
	if(realLen < 0)
	{
		return -1;
	}

    readSize = read(fd, (char *)page, size);
    return readSize;
}

int WriteFile(int fd, unsigned long offset, int size, char *pageBuf)
{
    int writeSize = 0;

    if(fd < 0 || pageBuf == NULL)
        return -1;

    lseek(fd, offset, SEEK_SET);

    writeSize = write(fd, pageBuf, size);
    return writeSize;
}

int SyncFile(int fd)
{
    if(fd > 0)
        fsync(fd);
    return 0;
}

int GetFileBlockNum(int fd, int blockSize)
{
    long length = lseek(fd, 0, SEEK_END);

    if(length < 0)
        return 0;
    return (int)(length / blockSize);
}