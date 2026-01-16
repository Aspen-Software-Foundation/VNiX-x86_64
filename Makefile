# 	Copyright (C) 2026 Aspen Software Foundation
#
# 	Module: Makefile
# 	Description: The main build system for the VNiX Operating System
# 	Author: Yazin Tantawi (and Jerry Jhird)

# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
# If a copy of the MPL was not distributed with this file, You can obtain one at 
# https://mozilla.org/MPL/2.0/.

CFLAGS := -I src/ -ffreestanding -Wall -Wextra -Wunused-parameter -static -nostartfiles -nostdlib -fno-pie -no-pie -mno-red-zone -mcmodel=large -T linker.ld
QEMU_CPU ?=
QEMU_MEM ?= -m 2G

all: clean build/uefi.img kernel run

kernel:
	mkdir -p build
	gcc -ffreestanding -c src/kernel/kernel.c -o build/kernel.o $(CFLAGS)
	gcc -ffreestanding -c src/drivers/terminal/src/term.c -o build/term.o $(CFLAGS)
	gcc -ffreestanding -c src/drivers/klibc/memory.c -o build/memory.o $(CFLAGS) 
	gcc -ffreestanding -c src/drivers/klibc/stdio.c -o build/stdio.o $(CFLAGS)  
	gcc -ffreestanding -c src/drivers/util/serial.c -o build/serial.o $(CFLAGS)
	gcc -ffreestanding -c src/arch/x86_64/gdt.c -o build/gdt.o $(CFLAGS)
	gcc -ffreestanding -c src/arch/x86_64/idt.c -o build/idt.o $(CFLAGS)
	gcc -ffreestanding -c src/arch/x86_64/io.c -o build/io.o $(CFLAGS)
	
	ld -T linker.ld -nostdlib -static -o build/kernel.elf build/kernel.o build/term.o build/stdio.o build/memory.o  build/serial.o  build/io.o build/idt.o build/gdt.o

build/uefi.img: kernel 
	@printf ">>> Creating UEFI bootable image: $@\n"

	dd if=/dev/zero of=build/uefi.img bs=1M count=64

	parted -s build/uefi.img mklabel gpt
	parted -s build/uefi.img mkpart ESP fat32 1MiB 100%
	parted -s build/uefi.img set 1 esp on

	mformat -i build/uefi.img ::/

	mmd -i build/uefi.img ::EFI
	mmd -i build/uefi.img ::EFI/BOOT
	mcopy -i build/uefi.img limine/BOOTX64.EFI ::EFI/BOOT/
	mmd -i build/uefi.img ::boot
	mcopy -i build/uefi.img build/kernel.elf ::boot/
	mcopy -i build/uefi.img src/limine.conf ::

run:
	qemu-system-x86_64 $(if $(QEMU_CPU),-cpu $(QEMU_CPU)) \
		$(QEMU_MEM) \
		-bios ovmf/OVMF.fd \
		-drive if=virtio,format=raw,file=build/uefi.img \
		-serial stdio \

clean:
	rm -rf build

# generate compile_commands.json for clangd
cc_commands:
	bear -- make