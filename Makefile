#--------------------------------------Makefile-------------------------------------
OBJECT_DIR = ./object
SRC_DIR = ./src

CFILES = $(wildcard $(SRC_DIR)/*.c)
OFILES = $(CFILES:$(SRC_DIR)/%.c=$(OBJECT_DIR)/%.o)

GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib
<<<<<<< Updated upstream
<<<<<<< Updated upstream
LDFLAGS = -nostdlib
=======
LDFLAGS = -nostdlib 
>>>>>>> Stashed changes
=======
LDFLAGS = -nostdlib 
>>>>>>> Stashed changes

all: kernel8.img #run

$(OBJECT_DIR)/boot.o: $(SRC_DIR)/boot.S
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.c
	aarch64-none-elf-gcc $(CFLAGS) -c $< -o $@

kernel8.img: $(OBJECT_DIR)/boot.o $(OFILES)
	aarch64-none-elf-ld $(LDFLAGS) $(OBJECT_DIR)/boot.o $(OFILES) -T $(SRC_DIR)/link.ld -o $(OBJECT_DIR)/kernel8.elf
	aarch64-none-elf-objcopy -O binary $(OBJECT_DIR)/kernel8.elf kernel8.img

clean:
	rm *.img .\build\kernel8.elf .\build\*.o

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio

test: all run