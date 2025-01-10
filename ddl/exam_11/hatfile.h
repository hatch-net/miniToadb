#ifndef HAT_FILE_H
#define HAT_FILE_H

#define FILE_PATH_MAX_LEN       1024

int CreateFile(char *filename, int mode);
int CloseFile(int fd);

#endif 