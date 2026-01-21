# VNiX Operating System (VNiX) 

Created by the Aspen Software Foundation, the VNiX Operating System is a new OS that's main goal is to run multiple executable types, such as Microsoft Windows's EXE file type, and Linux's ELF file type, 
therefore fixing the issue of having to dualboot Windows and Linux.

This operating system uses a modular build system made in the GNU Makefile Language, a fast and complex build language made specifically for large projects. More information can be found here: https://www.gnu.org/software/make/

Currently, the VNiX Operating System uses Cuoreterm for its terminal. Cuoreterm is a super lightweight and freestanding terminal made by JerryJhird for CuoreOS. 
VNiX also features RISCV32 program support, made by Dcraftbg. For more information about either one of these projects, visit the links at the bottom of this README.md.
This OS also features the Limine Bootloader, because of its extensive features and ease of use. I personally support its goals and achievements, which is why i've included it in this project.

---

## Building the OS

To build this OS, simply run the command below in the project's root directory:
```
make
```
The required dependencies are as follows:

- Make
- GCC Toolchain
- QEMU-Full
- Clang
- Parted

Note: The method of installing these packages may differ by distribution and operating system. Please refer to the documentation for your distribution and/or OS.

---

## Directory Structure:

This is the average root directory for VNiX on the user side

  ```
root-dir:
├── A-Documentation
│   ├── 01
│   │   └── Introduction
│   ├── API-ABI
│   │   └── API-Info
│   ├── Build-Logs
│   │   └── Build-Logs
│   ├── Contributers-Maintainers
│   │   └── CONTRIBUTERS
│   ├── Development
│   │   └── EXE-Support
│   ├── Error-Codes
│   │   └── placeholder
│   └── Other
│       └── Gaming
├── Assets
│   └── wallpaper4k.png
├── build
│   ├── VNiX-uefi_dev-prototype.img
│   ├── gdt.o
│   ├── idt.o
│   ├── io.o
│   ├── isr.o
│   ├── isrs_gen.o
│   ├── isr_stubs.o
│   ├── kernel.elf
│   ├── kernel.o
│   ├── liballoc-impl.o
│   ├── liballoc.o
│   ├── log-info.o
│   ├── pmm.o
│   ├── serial.o
│   ├── stdio.o
│   ├── stdlib.o
│   ├── string.o
│   ├── term.o
│   ├── usb_root
│   │   ├── boot
│   │   │   ├── kernel.elf
│   │   │   └── wallpaper4k.png
│   │   ├── EFI
│   │   │   └── BOOT
│   │   │       └── BOOTX64.EFI
│   │   ├── limine.conf
│   │   └── startup.nsh
│   └── vmm.o
├── limine
│   ├── BOOTAA64.EFI
│   ├── BOOTIA32.EFI
│   ├── BOOTLOONGARCH64.EFI
│   ├── BOOTRISCV64.EFI
│   ├── BOOTX64.EFI
│   ├── LICENSE
│   ├── limine-bios-cd.bin
│   ├── limine-bios-hdd.h
│   ├── limine-bios-pxe.bin
│   ├── limine-bios.sys
│   ├── limine.c
│   ├── limine-uefi-cd.bin
│   └── Makefile
├── linker.ld
├── Makefile
├── ovmf
│   ├── OVMF_CODE_4M.fd
│   ├── OVMF.fd
│   └── OVMF_VARS_4M.fd
├── README.md
├── src
│   ├── arch
│   │   ├── limine.h
│   │   └── x86_64
│   │       ├── gdt.c
│   │       ├── idt.c
│   │       ├── io.c
│   │       ├── isr.c
│   │       ├── isrs_gen.c
│   │       └── isr_stubs.asm
│   ├── drivers
│   │   ├── emul
│   │   │   └── rv_vm.h
│   │   │       ├── LICENSE.md
│   │   │       └── rv_vm.h
│   │   ├── klibc
│   │   │   ├── stdio.c
│   │   │   ├── stdlib.c
│   │   │   └── string.c
│   │   ├── memory
│   │   │   ├── liballoc
│   │   │   │   ├── liballoc_1_1.c
│   │   │   │   ├── liballoc_1_1.h
│   │   │   │   ├── liballoc.c
│   │   │   │   └── liballoc.h
│   │   │   ├── liballoc-impl.c
│   │   │   ├── pmm.c
│   │   │   └── vmm.c
│   │   ├── terminal
│   │   │   ├── LICENSE
│   │   │   └── src
│   │   │       ├── cuoreterm.h
│   │   │       ├── kfont.h
│   │   │       └── term.c
│   │   └── util
│   │       ├── log-info.c
│   │       └── serial.c
│   ├── includes
│   │   ├── arch
│   │   │   └── x86_64
│   │   │       ├── gdt.h
│   │   │       ├── idt.h
│   │   │       ├── io.h
│   │   │       └── isr.h
│   │   ├── klibc
│   │   │   ├── stdarg.h
│   │   │   ├── stdbool.h
│   │   │   ├── stdint.h
│   │   │   ├── stdio.h
│   │   │   ├── stdlib.h
│   │   │   └── string.h
│   │   ├── memory
│   │   │   ├── alloc
│   │   │   ├── pmm.h
│   │   │   └── vmm.h
│   │   └── util
│   │       ├── endian.h
│   │       ├── log-info.h
│   │       ├── serial.h
│   │       └── util.h
│   ├── kernel
│   │   └── kernel.c
│   └── limine.conf

```
---

## License

Most of this project is licensed and shipped under GPLv2 as seen in the LICENSE file, but some code snippets/files may be subject to MPLv2 or different licenses. 
Please refer to the file contents and the disclaimer at the top of the file for more information.

---
## External project links
             
CuoreOS: https://codeberg.org/jerryjhird/CuoreOS

Cuoreterm: https://codeberg.org/jerryjhird/Cuoreterm

rv_vm.h: https://codeberg.org/Dcraftbg/rv_vm.h
