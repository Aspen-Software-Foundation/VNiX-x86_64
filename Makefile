
CFLAGS := -ffreestanding -Wall -Wextra -static -nostartfiles -nostdlib -fno-pie -no-pie -mno-red-zone -mcmodel=large -T linker.ld
QEMU_CPU ?=
QEMU_MEM ?= -m 2G
all: clean build/uefi.img kernel run

kernel:
	mkdir -p build
	gcc -ffreestanding -c src/kernel/kernel.c -o build/kernel.o $(CFLAGS)
	gcc -ffreestanding -c src/drivers/framebuffer/src/term.c -o build/term.o $(CFLAGS)
	gcc -ffreestanding -c src/drivers/libc/memory.c -o build/memory.o $(CFLAGS) 
	gcc -ffreestanding -c src/drivers/libc/stdio.c -o build/stdio.o $(CFLAGS) 
	gcc -ffreestanding -c src/drivers/util/math.c -o build/math.o $(CFLAGS) 
	ld -T linker.ld -nostdlib -static -o build/kernel.elf build/kernel.o build/term.o build/stdio.o build/memory.o build/math.o 

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

clean:
	rm -rf build

run:
	qemu-system-x86_64 $(if $(QEMU_CPU),-cpu $(QEMU_CPU)) \
		$(QEMU_MEM) \
		-bios ovmf/OVMF.fd \
		-drive if=virtio,format=raw,file=build/uefi.img \
		-serial stdio \


