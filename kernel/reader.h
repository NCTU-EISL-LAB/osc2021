#pragma once

#include "vfs.h"

struct cpio_newc_header {
    char    c_magic[6];
    char    c_ino[8];
    char    c_mode[8];
    char    c_uid[8];
    char    c_gid[8];
    char    c_nlink[8];
    char    c_mtime[8];
    char    c_filesize[8];
    char    c_devmajor[8];
    char    c_devminor[8];
    char    c_rdevmajor[8];
    char    c_rdevminor[8];
    char    c_namesize[8];
    char    c_check[8];
};

struct cpio_file {
    struct cpio_newc_header cpio_header;
    int size;
    char filename[1024];        // 1 KB limited
    char contents[1024 * 1024]; // 1 MB limited
};

void cpio_exec(char * filename, char * argv[]);

void Cpiols();
void Cpiocat(char arg[]);
void Cpioexe(char arg[]);

int Hex2Int(char byte[]);

void ReadCpio();
void ReadBytesData(char data[], int offset, int bytes);
void ReadCpioHeader(struct cpio_file * cpio_, int offset);
int ReadCpioContent(struct cpio_file * cpio_, int offset);

void LoadUserProgram(char filename[]);

void FromEl1toEl0();

void EnableTimer();
void EnableInterrupt();

void PrintCpio();
void PrintFileContent(char arg[]);
