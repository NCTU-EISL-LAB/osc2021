ARMGNU = aarch64-linux-gnu
FLAGS = -Wall -nostdlib -Iinclude/bootloader -Iinclude/kernel -Iinclude/lib -ffreestanding -Werror

BUILD_DIR= build
SRC_DIR = src
OBJS_DIR = objs

C_FILES = $(wildcard $(SRC_DIR)/*/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/*/*.S)

OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(OBJS_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(OBJS_DIR)/%_s.o)

all: kernel8.img bootloader.img assets

$(OBJS_DIR)/%_c.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(ARMGNU)-gcc $(FLAGS) -c $< -o $@

$(OBJS_DIR)/%_s.o: $(SRC_DIR)/%.S
	@mkdir -p $(@D)
	$(ARMGNU)-gcc $(FLAGS) -c $< -o $@

kernel8.img: $(filter $(OBJS_DIR)/kernel/%.o $(OBJS_DIR)/lib/%.o, $(OBJ_FILES)) $(SRC_DIR)/kernel/linker.ld
	@mkdir -p $(BUILD_DIR)
	@$(ARMGNU)-ld -T $(SRC_DIR)/kernel/linker.ld -o $(BUILD_DIR)/kernel8.elf $(filter $(OBJS_DIR)/kernel/%.o $(OBJS_DIR)/lib/%.o, $(OBJ_FILES))
	@$(ARMGNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary $(BUILD_DIR)/$@

bootloader.img: $(filter $(OBJS_DIR)/bootloader/%.o $(OBJS_DIR)/lib/%.o, $(OBJ_FILES)) $(SRC_DIR)/bootloader/linker.ld
	@mkdir -p $(BUILD_DIR)	
	@$(ARMGNU)-ld -T $(SRC_DIR)/bootloader/linker.ld -o $(BUILD_DIR)/bootloader.elf $(filter $(OBJS_DIR)/bootloader/%.o $(OBJS_DIR)/lib/%.o, $(OBJ_FILES))
	@$(ARMGNU)-objcopy $(BUILD_DIR)/bootloader.elf -O binary $(BUILD_DIR)/$@

assets: $(SRC_DIR)/config.txt
	@mkdir -p $(BUILD_DIR)
	cp $< $(BUILD_DIR)
	cd rootfs; find . | cpio -o -H newc > ../${BUILD_DIR}/initramfs.cpio



clean:
	rm -rf $(BUILD_DIR) $(OBJS_DIR)

run:
	qemu-system-aarch64 -M raspi3 -kernel $(BUILD_DIR)/bootloader.img -serial null -serial stdio -display none

run.tty:
	qemu-system-aarch64 -M raspi3 -kernel $(BUILD_DIR)/bootloader.img -serial null -serial pty -display none

debug:
	qemu-system-aarch64 -M raspi3 -kernel $(BUILD_DIR)/bootloader.img -serial null -serial pty -display none -s -S
	