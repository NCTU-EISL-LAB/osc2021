#include "syscall.h"
#include "printf.h"

void dumpState(){
    asm volatile("svc 0 \n"::);
    return;

}
int getpid(){
	long ret;
	asm volatile("\
		svc 5\n\
		mov %0, x0\n\
	":"=r"(ret):);
	return ret;
}

int uart_read(char* buf,int size){
	long ret;
	asm volatile("\
		svc 4\n\
		mov %0, x0\n\
	":"=r"(ret):);
	return ret;
}

int uart_write(char* buf,int size){
	long ret;
	asm volatile("\
		svc 3\n\
		mov %0, x0\n\
	":"=r"(ret):);
	return ret;
}

unsigned int uart_printf(char* fmt,...){
	char dst[100];
    //__builtin_va_start(args, fmt): "..." is pointed by args
    //__builtin_va_arg(args,int): ret=(int)*args;args++;return ret;
    __builtin_va_list args;
    __builtin_va_start(args,fmt);
    unsigned int ret=vsprintf(dst,fmt,args);
    int x;
    x = uart_write(dst,ret);
    return x;
}

int exec(char* name,char** argv){
    asm volatile("svc 2   \n"::);
    while(1){}
}

void cur_exit(){
	asm volatile("svc 1\n"::);
	while(1){}
}

int fork(){
	long ret;
	asm volatile("\
		svc 6\n\
		mov %0, x0\n\
	":"=r"(ret):);
	return ret;
}
