# Copyright (C) 2026 Aspen Software Foundation

# 	Module: kernel.c
# 	Description: The UEFI kernel for the Ancore Operating System.
# 	Author: Yazin Tantawi (and Jerry Jhird)

# 	All components of the Ancore Operating System, except where otherwise noted,
# 	are copyright of the Aspen Software Foundation (and the corresponding author(s)) and licensed under GPLv2 or later.
# 	For more information on the GNU Public License Version 2, please refer to the LICENSE file 
# 	or to the link provided here: https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html

# THIS OPERATING SYSTEM IS PROVIDED "AS IS" AND "AS AVAILABLE" UNDER
# THE GNU GENERAL PUBLIC LICENSE VERSION 2, WITHOUT
# WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
# TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE, TITLE, AND NON-INFRINGEMENT.

# TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, IN NO EVENT SHALL
# THE AUTHORS, COPYRIGHT HOLDERS, OR CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE), ARISING IN ANY WAY OUT OF THE USE OF THIS OPERATING SYSTEM,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE OPERATING SYSTEM IS
# WITH YOU. SHOULD THE OPERATING SYSTEM PROVE DEFECTIVE, YOU ASSUME THE COST OF
# ALL NECESSARY SERVICING, REPAIR, OR CORRECTION.

# YOU SHOULD HAVE RECEIVED A COPY OF THE GNU GENERAL PUBLIC LICENSE
# ALONG WITH THIS OPERATING SYSTEM; IF NOT, WRITE TO THE FREE SOFTWARE
# FOUNDATION, INC., 51 FRANKLIN STREET, FIFTH FLOOR, BOSTON,
# MA 02110-1301, USA.


CFLAGS := -I src/ -ffreestanding -Wall -Wextra -Wunused-parameter -static -nostartfiles -nostdlib -fno-pie -no-pie -mno-red-zone -mcmodel=large -T linker.ld -I src/includes/ -I src/includes/klibc/ -I src/drivers/memory/liballoc/ -D_ALLOC_SKIP_DEFINE
QEMU_CPU ?=
QEMU_MEM ?= -m 2G

all: clean kernel build/uefi.iso run

kernel:
#I dont care if this is incorrect, if it works, it works.
	mkdir -p build
	gcc -ffreestanding -c src/kernel/kernel.c -o build/kernel.o $(CFLAGS)
	gcc -ffreestanding -c src/drivers/terminal/src/term.c -o build/term.o $(CFLAGS)
	gcc -ffreestanding -c src/drivers/klibc/stdlib.c -o build/stdlib.o $(CFLAGS) 
	gcc -ffreestanding -c src/drivers/klibc/stdio.c -o build/stdio.o $(CFLAGS) 
	gcc -ffreestanding -c src/drivers/klibc/string.c -o build/string.o $(CFLAGS)  
	gcc -ffreestanding -c src/drivers/util/serial.c -o build/serial.o $(CFLAGS)
	gcc -ffreestanding -c src/arch/x86_64/gdt.c -o build/gdt.o $(CFLAGS)
	gcc -ffreestanding -c src/arch/x86_64/idt.c -o build/idt.o $(CFLAGS)
	gcc -ffreestanding -c src/arch/x86_64/io.c -o build/io.o $(CFLAGS)
	gcc -ffreestanding -c src/drivers/memory/pmm.c -o build/pmm.o $(CFLAGS)
	gcc -ffreestanding -c src/arch/x86_64/isr.c -o build/isr.o $(CFLAGS)
	gcc -ffreestanding -c src/arch/x86_64/isrs_gen.c -o build/isrs_gen.o $(CFLAGS)
	gcc -ffreestanding -c src/drivers/memory/liballoc/liballoc.c -o build/liballoc.o $(CFLAGS) 
	gcc -ffreestanding -c src/drivers/memory/liballoc-impl.c -o build/liballoc-impl.o $(CFLAGS)
	gcc -ffreestanding -c src/drivers/memory/vmm.c -o build/vmm.o $(CFLAGS)
	nasm -f elf64 src/arch/x86_64/isr_stubs.asm -o build/isr_stubs.o
	ld -T linker.ld -nostdlib -static -o build/kernel.elf build/kernel.o build/term.o build/stdio.o build/stdlib.o  build/serial.o  build/io.o build/idt.o build/gdt.o build/pmm.o build/isr.o build/isrs_gen.o build/isr_stubs.o build/string.o build/liballoc.o build/liballoc-impl.o build/vmm.o

build/uefi.iso: kernel 
	@printf ">>> Creating UEFI bootable ISO image: $@\n"
	rm -rf build/iso_root build/efi.img
	mkdir -p build/iso_root/boot
	cp limine/limine-bios-cd.bin build/iso_root/boot/
	cp build/kernel.elf build/iso_root/boot/
	cp src/limine.conf build/iso_root/
	cp Assets/wallpaper4k.png build/iso_root/boot/
	dd if=/dev/zero of=build/efi.img bs=1M count=10
	mkfs.vfat -F 12 build/efi.img
	mmd -i build/efi.img ::/EFI
	mmd -i build/efi.img ::/EFI/BOOT
	mmd -i build/efi.img ::/boot
	mcopy -i build/efi.img limine/BOOTX64.EFI ::/EFI/BOOT/
	mcopy -i build/efi.img build/kernel.elf ::/boot/
	mcopy -i build/efi.img src/limine.conf ::/
	mcopy -i build/efi.img Assets/wallpaper4k.png ::/boot/
	xorriso -as mkisofs \
		-b boot/limine-bios-cd.bin \
		-no-emul-boot \
		-boot-load-size 4 \
		-boot-info-table \
		-eltorito-alt-boot \
		-e efi.img \
		-no-emul-boot \
		-isohybrid-gpt-basdat \
		-o build/uefi.iso \
		build/iso_root build/efi.img
	if [ -f limine/limine ]; then ./limine/limine bios-install build/uefi.iso; fi
	rm -rf build/iso_root build/efi.img

run:
	qemu-system-x86_64 $(if $(QEMU_CPU),-cpu $(QEMU_CPU)) \
		$(QEMU_MEM) \
		-bios ovmf/OVMF.fd \
		-cdrom build/uefi.iso \
		-serial stdio

clean:
	rm -rf build

# generate compile_commands.json for clangd
cc_commands:
	bear -- make