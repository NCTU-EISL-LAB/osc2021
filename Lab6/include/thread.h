void threadSchedule();
int tidGet();
void exec(char* path,char** argv);
void exit();
int fork();
int sys_open(const char *pathname,int flags);
int sys_close(int fd);
int sys_write(int fd,const void *buf,int count);
int sys_read(int fd,void *buf,int count);
void threadTest();
void threadtest1();
void threadtest2();