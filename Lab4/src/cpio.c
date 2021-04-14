#include "cpio.h"
#include "uart.h"
#include "my_string.h"
#include "stddef.h"
#include "stdint.h"
unsigned int c2i(char c){
  if (c >= '0' && c <= '9') return      c - '0';
  if (c >= 'A' && c <= 'F') return 10 + c - 'A';
    if (c >= 'a' && c <= 'f') return 10 + c - 'a';
    return -1;
}

unsigned h2i(char *buf, int len){
  unsigned int num = 0;
  for(int i = 0; i < len; i++){
    num <<= 4;
    num += (c2i(buf[i]));
  }
  return num;
}
size_t align_up(size_t size, int alignment) {
  return (size + alignment - 1) & -alignment;
}
char ls(void *cpiobuf){
  uintptr_t ptr = (uintptr_t)cpiobuf;
  const struct cpio_newc_header *header;
  uart_printf("list all \n");
  while(1){
    header = (struct cpio_newc_header *)ptr;
    if (strncmp(header->c_magic, "070701", 6)) {
            return;
        }

        unsigned namesize = h2i(header->c_namesize, 8);
        unsigned filesize = h2i(header->c_filesize, 8);
        if (!strcmp(header->content_buf, "TRAILER!!!")) {
            break;
        }

        ptr += sizeof(struct cpio_newc_header);
        uart_printf("%s\n", (char *)ptr);
        ptr = align_up(ptr + namesize, 4);
        ptr = align_up(ptr + filesize, 4);
  }
}

char *cpio_content(const char *content, void *cpiobuf){
  uintptr_t ptr = (uintptr_t)cpiobuf;
  const struct cpio_newc_header *header;
  while(1){
    header = (struct cpio_newc_header *)ptr;
    if (strncmp(header->c_magic, "070701", 6)) {
          return NULL;
      }
      if (!strcmp(header->content_buf, "TRAILER!!!")) {
          return NULL;
      }

      unsigned namesize = h2i(header->c_namesize, 8);
      unsigned filesize = h2i(header->c_filesize, 8);
      unsigned mod = h2i(header->c_mode, 8);

      if (!strcmp(header->content_buf, content)) {
          if (S_ISDIR(mod))return "directory";

          if (filesize) {
            ptr += sizeof(struct cpio_newc_header);
            ptr = align_up(ptr + namesize, 4);
            return (char *)ptr;
          }
       return "";

      }

      else {
          ptr += sizeof(struct cpio_newc_header);
          ptr = align_up(ptr + namesize, 4);
          ptr = align_up(ptr + filesize, 4);
      }
  }

}


void cpio_load_program(const char *target, unsigned char* target_addr, void *cpiobuf){
  uintptr_t ptr = (uintptr_t)cpiobuf;
  const struct cpio_newc_header *header;
  uart_printf("%s\n",target);
  while(1){
    header = (struct cpio_newc_header *)ptr;
    if (strncmp(header->c_magic, "070701", 6)) {
          return ;
      }
      if (!strcmp(header->content_buf, "TRAILER!!!")) {
          return ;
      }

      unsigned namesize = h2i(header->c_namesize, 8);
      unsigned filesize = h2i(header->c_filesize, 8);
      uart_printf("%s\n", target_addr);
      if (!strcmp(header->content_buf, target)) {
          
          uart_printf("%s\n", target_addr);
          if (filesize) {
            unsigned char *content = (unsigned char *)ptr;
            for(unsigned long long i = 0; i < filesize; i++){
              target_addr[i] = content[i];
            }
            return;
          }
       return ;

      }
      else {
          ptr += sizeof(struct cpio_newc_header);
          ptr = align_up(ptr + namesize, 4);
          ptr = align_up(ptr + filesize, 4);
      }
  }

}

void load(void* cpio_buf){
    uart_printf("load user prgram\n");

    unsigned long size;

    unsigned char *load_addr = (unsigned char*)0x30000000;

    cpio_load_program("app.img",load_addr, cpio_buf);


    uart_printf("start user app\n");
    // change exception level
    // asm volatile("mov x0, 0x3c0  \n"); // disable timer interrupt, enable svn
    asm volatile("mov x0, 0x340  \n"); // enable core timer interrupt
    asm volatile("msr spsr_el1, x0  \n");
    asm volatile("msr elr_el1, %0   \n" ::"r"(load_addr));
    asm volatile("msr sp_el0, %0    \n" ::"r"(load_addr));

    // enable the core timer’s interrupt
    asm volatile("mov x0, 1             \n");
    asm volatile("msr cntp_ctl_el0, x0  \n");
    asm volatile("mrs x0, cntfrq_el0    \n");
    asm volatile("add x0, x0, x0        \n");
    asm volatile("msr cntp_tval_el0, x0 \n");
    asm volatile("mov x0, 2             \n");
    asm volatile("ldr x1, =0x40000040   \n");
    asm volatile("str w0, [x1]          \n");

    asm volatile("eret              \n");
}