CC = aarch64-linux-gnu-gcc
CFLAG = -g -fPIC -Iinclude -fno-stack-protector -nostdlib -nostartfiles -ffunction-sections -fno-builtin

SRC_DIR = src/
BUILD_DIR = build/
SRCS = $(wildcard $(SRC_DIR)*.c)
ASM_FILE = $(wildcard $(SRC_DIR)*.S)

all:clean kernel8.img


# start.o: $(SRC_DIR)start.S
# 	$(CC) $(CFLAG) -c $< -o $@

# entry.o: $(SRC_DIR)entry.S ./include/entry.h
# 	$(CC) $(CFLAG) -c $< -o $@
$(BUILD_DIR)%_s.o: $(SRC_DIR)%.S 
	$(CC) $(CFLAG) -c $< -o $@
$(BUILD_DIR)%_c.o: $(SRC_DIR)%.c
	aarch64-linux-gnu-gcc $(CFLAG) -c $< -o $@

OBJS = $(SRCS:$(SRC_DIR)%.c=$(BUILD_DIR)%_c.o)
OBJS += $(ASM_FILE:$(SRC_DIR)%.S=$(BUILD_DIR)%_s.o)

kernel8.img: $(OBJS) 
	aarch64-linux-gnu-ld $(OBJS) -T $(SRC_DIR)linker.ld -nostdlib -o kernel8.elf
	aarch64-linux-gnu-objcopy -O binary kernel8.elf kernel8.img

clean:
	-rm $(BUILD_DIR)*.o
	-rm $(SRC_DIR)*.o
	-rm kernel8.elf kernel8.img

test:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio -display none -initrd ../initramfs.cpio
gdb:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -display none -S -s -initrd ../initramfs.cpio
test_lab2:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio -display none -dtb ../OSDI/bcm2710-rpi-3-b-plus.dtb

test_qemu_lab2:
	qemu-system-aarch64 -M raspi3 -kernel bootloader.img -serial null -serial pty -display none -initrd ../initramfs.cpio -dtb ../OSDI/bcm2710-rpi-3-b-plus.dtb

check:
	aarch64-linux-gnu-readelf -s kernel8.elf

user_program:
	$(CC) $(CFLAG) -c user_program.S -o user_program.elf
	aarch64-linux-gnu-objcopy -O binary user_program.elf user_program.img
	-rm ../initramfs.cpio
	cp ./user_program.img ../rootfs
	cd ../rootfs && find . | cpio -o -H newc > ../initramfs.cpio
	
	