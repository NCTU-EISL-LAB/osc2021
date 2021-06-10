#include "cpio.h"

extern uint64_t INITRAMFS_ADDR;
extern uint64_t INITRAMFS_ADDR_END;

void cpio_init(){
    const uint32_t *prop = dtb_get_node_prop_addr("chosen", NULL);
    if(!prop){
        miniuart_send_S("Error: dtb has no 'chosen' node!!!" NEW_LINE);
        return;
    }
    uint32_t value;
    int32_t n = dtb_get_prop_value("linux,initrd-start", prop, &value);
    if(n == -1){
        miniuart_send_S("Error: 'chosen' node has no 'linux,initrd-start' property!!!" NEW_LINE);
        return;
    }
    if(n != 4){
        //printf("Error: 'linux,initrd-start' property gives size of %d!!!" NEW_LINE, n);
        miniuart_send_S("Error: 'linux,initrd-start' property gives size of not 4!!!" NEW_LINE);
        return;
    }
    INITRAMFS_ADDR = (uint64_t)bswap32(value);

    n = dtb_get_prop_value("linux,initrd-end", prop, &value);
    if(n == -1){
        miniuart_send_S("Error: 'chosen' node has no 'linux,initrd-end' property!!!" NEW_LINE);
        return;
    }
    if(n != 4){
        //printf("Error: 'linux,initrd-end' property gives size of %d!!!" NEW_LINE, n);
        miniuart_send_S("Error: 'linux,initrd-end' property gives size of not 4!!!" NEW_LINE);
        return;
    }
    INITRAMFS_ADDR_END = (uint64_t)bswap32(value);
}

void cpio_ls(){
    if(INITRAMFS_ADDR == 0){
        miniuart_send_S("Error: initramfs not loaded!!!" NEW_LINE);
        return;
    }

    cpio_newc_header *blk = (cpio_newc_header *)INITRAMFS_ADDR;
    if(strncmp(blk->c_magic, CPIO_MAGIC_NUM, sizeof(blk->c_magic))){
        miniuart_send_S("Error: Invalid cpio's New ASCII Format!!!" NEW_LINE);
        return;
    }

    while(1){
        size_t namesize = xatou32(blk->c_namesize, sizeof(blk->c_namesize));
        char *name = ((char *)blk + sizeof(cpio_newc_header));
        if(!strcmp("TRAILER!!!", name)){
            break;
        }

        uint32_t mode = xatou32(blk->c_mode, sizeof(blk->c_mode));
        if(mode & CPIO_MODE_FILE){
            miniuart_send_nS(name, namesize);
            miniuart_send_S("  ");
        }

        size_t filesize = xatou32(blk->c_filesize, sizeof(blk->c_filesize));
        char *file = (char *)ROUNDUP_MUL4((uint64_t)name + namesize);
        blk = (cpio_newc_header *)ROUNDUP_MUL4((uint64_t)file + filesize);
    }
    miniuart_send_S(NEW_LINE);
}

void cpio_cat(const char *find_filename){
    if(!find_filename){
        miniuart_send_S("Usage: cat <filename>" NEW_LINE);
        return;
    }

    if(INITRAMFS_ADDR == 0){
        miniuart_send_S("Error: initramfs not loaded!!!" NEW_LINE);
        return;
    }

    cpio_newc_header *blk = (cpio_newc_header *)INITRAMFS_ADDR;
    if(strncmp(blk->c_magic, CPIO_MAGIC_NUM, sizeof(blk->c_magic))){
        miniuart_send_S("Error: Invalid cpio's New ASCII Format!!!" NEW_LINE);
        return;
    }

    while(1){
        size_t namesize = xatou32(blk->c_namesize, sizeof(blk->c_namesize));
        char *name = ((char *)blk + sizeof(cpio_newc_header));
        if(!strcmp("TRAILER!!!", name)){
            break;
        }

        size_t filesize = xatou32(blk->c_filesize, sizeof(blk->c_filesize));
        char *file = (char *)ROUNDUP_MUL4((uint64_t)name + namesize);

        uint32_t mode = xatou32(blk->c_mode, sizeof(blk->c_mode));
        if(mode & CPIO_MODE_FILE){
            if(!strcmp(find_filename, name)){
                if(filesize){
                    miniuart_send_nS(file, filesize);
                    miniuart_send_C('\r');
                }else{
                    miniuart_send_S(NEW_LINE);
                }
                return;
            }
        }

        blk = (cpio_newc_header *)ROUNDUP_MUL4((uint64_t)file + filesize);
    }
    miniuart_send_S("cat: ");
    miniuart_send_S(find_filename);
    miniuart_send_S(": No such file" NEW_LINE);
}

void cpiofs_populate(struct filesystem *fs){
    if(INITRAMFS_ADDR == 0){
        miniuart_send_S("Error: initramfs not loaded!!!" NEW_LINE);
        return;
    }

    cpio_newc_header *blk = (cpio_newc_header *)INITRAMFS_ADDR;
    if(strncmp(blk->c_magic, CPIO_MAGIC_NUM, sizeof(blk->c_magic))){
        miniuart_send_S("Error: Invalid cpio's New ASCII Format!!!" NEW_LINE);
        return;
    }

    struct mount *cpiofs = get_mount(fs->name);
    if(!cpiofs){
        miniuart_send_S("Error: Populate failed, no cpiofs!!!" NEW_LINE);
        return;
    }

    char abs_name[512];
    while(1){
        size_t namesize = xatou32(blk->c_namesize, sizeof(blk->c_namesize));
        char *name = ((char *)blk + sizeof(cpio_newc_header));
        if(!strcmp("TRAILER!!!", name)){
            break;
        }

        uint32_t mode = xatou32(blk->c_mode, sizeof(blk->c_mode));
        if(mode & CPIO_MODE_DIR){
            if((name[0] != '.') || (name[1] != '\0')){
                strncpy(abs_name, name, -1);
                vfs_mkdir(abs_name, cpiofs->root);
            }
        }
        if(mode & CPIO_MODE_FILE){
            strncpy(abs_name, name, -1);
            struct file *fp = vfs_fopen(abs_name, O_CREAT, cpiofs->root);
            if(fp){
                size_t filesize = xatou32(blk->c_filesize, sizeof(blk->c_filesize));
                char *file = (char *)ROUNDUP_MUL4((uint64_t)name + namesize);
                fwrite(fp, file, filesize);
                fclose(fp);
            }
        }

        size_t filesize = xatou32(blk->c_filesize, sizeof(blk->c_filesize));
        char *file = (char *)ROUNDUP_MUL4((uint64_t)name + namesize);
        blk = (cpio_newc_header *)ROUNDUP_MUL4((uint64_t)file + filesize);
    }
}
