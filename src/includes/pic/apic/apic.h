/*
    Copyright (C) 2026 Aspen Software Foundation

    Module: apic.h
    Description: APIC module for the VNiX Operating System
    Author: Mejd Almohammedi 

    All components of the VNiX Operating System, except where otherwise noted, 
    are copyright of the Aspen Software Foundation (and the corresponding author(s)) and licensed under GPLv2 or later.
    For more information on the GNU Public License Version 2, please refer to the LICENSE file
    or to the link provided here: https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html

 * THIS OPERATING SYSTEM IS PROVIDED "AS IS" AND "AS AVAILABLE" UNDER 
 * THE GNU GENERAL PUBLIC LICENSE VERSION 2, WITHOUT
 * WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, TITLE, AND NON-INFRINGEMENT.
 * 
 * TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, IN NO EVENT SHALL
 * THE AUTHORS, COPYRIGHT HOLDERS, OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE), ARISING IN ANY WAY OUT OF THE USE OF THIS OPERATING SYSTEM,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE OPERATING SYSTEM IS
 * WITH YOU. SHOULD THE OPERATING SYSTEM PROVE DEFECTIVE, YOU ASSUME THE COST OF
 * ALL NECESSARY SERVICING, REPAIR, OR CORRECTION.
 *
 * YOU SHOULD HAVE RECEIVED A COPY OF THE GNU GENERAL PUBLIC LICENSE
 * ALONG WITH THIS OPERATING SYSTEM; IF NOT, WRITE TO THE FREE SOFTWARE
 * FOUNDATION, INC., 51 FRANKLIN STREET, FIFTH FLOOR, BOSTON,
 * MA 02110-1301, USA.
*/

#ifndef APIC_H
#define APIC_H

#include <stdint.h>
#include "includes/arch/x86_64/isr.h"
#include "includes/arch/x86_64/io.h"

// APIC Registers
#define APIC_EOI                    0xB0  // End Of Interrupt Register
#define APIC_LID                    0xD0
#define APIC_ISR_BASE               0x100 // In-Service Register Base
#define APIC_IRR_BASE               0x200 // Interrupt Request Register Base
#define APIC_TPR                    0x080 // Task Priority Register
#define APIC_LVT_TIMER              0x320 // Local Vector Table Timer Register
#define APIC_LVT_LINT0              0x350 // Local Vector Table LINT0 Register
#define APIC_LVT_LINT1              0x360 // Local Vector Table LINT1 Register


#define APIC_BASE                   0xFEE00000 // APIC Base Address (commonly defined, can vary)
#define APIC_IO_BASE                0xFEC00000 // APIC Base Address (commonly defined, can vary)
#define APIC_REGISTER_OFFSET        0x10  // offset to the registers in APIC
#define APIC_ID                     0x20
#define CR4_APIC_BIT                0x200    // the bit in CR4 register for enabling APIC (bit 9)
#define IA32_APIC_BASE_MSR          0x1B
#define IA32_APIC_BASE_MSR_BSP      0x100 // processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE   0x800

#define APIC_DELIVERY_MODE_FIXED    0x00000000  // normal fixed priority delivery
#define APIC_DELIVERY_MODE_LOWEST   0x00000100  // lowest priority delivery
#define APIC_DELIVERY_MODE_SMI      0x00000200  // System Management Interrupt
#define APIC_DELIVERY_MODE_NMI      0x00000400  // Non-Maskable Interrupt
#define APIC_DELIVERY_MODE_INIT     0x00000500  // INIT signal
#define APIC_DELIVERY_MODE_STARTUP  0x00000600  // Startup IPI (SIPI)
#define APIC_DELIVERY_MODE_EXTINT   0x00000700  // External interrupt
#define APIC_LVT_INT_MASKED         0x00000100
#define APIC_LVT_TRIGGER_LEVEL      0x00000080


#define APIC_DELIVERY_MODE_MASK      0x00000700  // Mask for extracting delivery mode bits


#define APIC_SVR            0xF0  // Spurious Interrupt Vector Register
#define APIC_DEST_FORMAT    0xE0  // Destination Format Register


#define IOAPIC_REG_SELECT     0x00
#define IOAPIC_REG_WINDOW     0x10


extern volatile uint32_t* apic_base;
extern volatile uint32_t* apic_io_base;

// MSR functions - add these declarations
void cpuSetMSR(uint32_t msr, uint32_t low, uint32_t high);
void cpuGetMSR(uint32_t msr, uint32_t* low, uint32_t* high);

// fn to read CR4 register (x86_64 version)
static inline uint64_t ReadCR4() {
    uint64_t cr4;
    asm volatile("mov %%cr4, %0" : "=r"(cr4));
    return cr4;
}

// fn to write CR4 register (x86_64 version)
static inline void WriteCR4(uint64_t cr4) {
    asm volatile("mov %0, %%cr4" :: "r"(cr4));
}

static inline void APIC_Write(uint32_t reg, uint32_t value) {
    apic_base[reg / sizeof(uint32_t)] = value;
}

static inline uint32_t APIC_Read(uint32_t reg) {
    return apic_base[reg / sizeof(uint32_t)];
}

// read from IOAPIC register
static inline uint32_t APIC_ReadIO(uint32_t reg) {
    // Write the register index
    apic_io_base[IOAPIC_REG_SELECT / sizeof(uint32_t)] = reg;
    // Read the value
    return apic_io_base[IOAPIC_REG_WINDOW / sizeof(uint32_t)];
}

// write to IOAPIC register
static inline void APIC_WriteIO(uint32_t reg, uint32_t value) {
    // Write the register index
    apic_io_base[IOAPIC_REG_SELECT / sizeof(uint32_t)] = reg;
    // Write the value
    apic_io_base[IOAPIC_REG_WINDOW / sizeof(uint32_t)] = value;
}

// APIC Interrupt Control

// fns for setting and getting the APIC base address
void cpu_set_apic_base(uintptr_t apic);
uintptr_t cpu_get_apic_base();

// fns to read and write to the IOAPIC registers
uint32_t cpuReadIoApic(void *ioapicaddr, uint32_t reg);
void cpuWriteIoApic(void *ioapicaddr, uint32_t reg, uint32_t value);

// fn to configure the IMCR to switch from PIC to APIC mode
void ConfigureIMCR();
void APIC_Initialize();

static inline void LAPIC_SendEOI() {
    APIC_Write(APIC_EOI, 0);
}

#endif