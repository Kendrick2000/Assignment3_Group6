#--------------------------------------Makefile-----------------------------------
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=build/%.o) #

BUILD_DIR = ./build
SRC_DIR = ./src

CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)
CPP_OFILES = $(CPP_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib 
LDFLAGS = -nostdlib

all: kernel8.img #run 
 
build/boot.o: src/boot.S
	aarch64-none-elf-gcc $(CFLAGS) -c src/boot.S -o build/boot.o 
 
build/%.o: src/%.c 
	aarch64-none-elf-gcc $(CFLAGS) -c $< -o $@ 

build/%.o: src/%.cpp
	aarch64-none-elf-g++ $(CFLAGS) -c $< -o $@	
 
kernel8.img: build/boot.o $(OBJS) $(CPP_OFILES)
	aarch64-none-elf-ld $(LDFLAGS) build/boot.o $(OBJS) $(CPP_OFILES) -T src/link.ld -o build/kernel8.elf 
	aarch64-none-elf-objcopy -O binary build/kernel8.elf ./kernel8.img 
 
clean: 
	del kernel8.img
	del build\*.o
	del build\*.elf
	
flash:
	powershell Get-WmiObject Win32_Volume -Filter "DriveType='2'"
	
	
 
run: 
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio