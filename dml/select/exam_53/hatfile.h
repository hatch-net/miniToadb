#ifndef HAT_FILE_H
#define HAT_FILE_H

#define FILE_PATH_MAX_LEN       1024

int CreateFile(char *filename, int mode);
int DeleteFile(char *filename);

int OpenFile(char *filename, int mode);
int CloseFile(int fd);

int ReadFile(int fd, unsigned long offset, int size, char *page);
int WriteFile(int fd, unsigned long offset, int size, char *pageBuf);

int SyncFile(int fd);

int GetFileBlockNum(int fd, int blockSize);

#endif 