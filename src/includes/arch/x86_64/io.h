/*
    Copyright (C) 2026 Aspen Software Foundation

    Module: io.h
    Description: I/O module for the VNiX Operating System
    Author: Yazin Tantawi

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

#ifndef IO_H
#define IO_H
#define UNUSED_PORT 0x80

#include <stdint.h>
#include <stddef.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

#define PAGE_SIZE 4096
#define ALIGN_UP(address, alignment) (((address) + (alignment - 1)) & ~((alignment) - 1))
#define ALIGN_DOWN(address, alignment) ((address) & ~((alignment) - 1))

//#define enable_interrupts() __asm__ ("cli")
//#define disable_interrupts() __asm__ ("sti")

typedef uint64_t virt_addr_t;
typedef uint64_t physc_addr_t;

static inline void hcf(void)
{
  for (;;)
  {
    __asm__("hlt");
  }
}

static inline void iowait() {
    outb(UNUSED_PORT, 0);
}


static inline void cpuGetMSR(uint32_t msr, uint32_t *eax, uint32_t *edx) {
    // Inline assembly to read from the MSR
    asm (
        "rdmsr" // MSR read instruction
        : "=a"(*eax), "=d"(*edx) // Output: EAX and EDX will store the 64-bit MSR value
        : "c"(msr) // Input: MSR to read from
    );
}


static inline void cpuSetMSR(uint32_t msr, uint32_t eax, uint32_t edx) {
    // Inline assembly to write to the MSR
    asm (
        "wrmsr" // MSR write instruction
        : // No outputs
        : "c"(msr), "a"(eax), "d"(edx) // MSR, EAX (lower 32 bits), EDX (upper 32 bits)
    );
}


typedef struct list_t 
{
    struct list_t *next, *prev;
} list_t;

static inline void list_init(list_t *list) {
    list->next = list;
    list->prev = list;
}

static inline list_t *list_last(list_t *list) {
    return list->prev != list ? list->prev : NULL;
}

static inline list_t *list_next(list_t *list) {
    return list->next != list ? list->next : NULL;
}

static inline void list_insert(list_t *new, list_t *link) {
    new->prev = link->prev;
    new->next = link;
    new->prev->next = new;
    new->next->prev = new;
}

static inline void list_append(list_t *new, list_t *into) {
    list_insert(new, into);
}

static inline void list_remove(list_t *list) {
    list->prev->next = list->next;
    list->next->prev = list->prev;
    list->next = list;
    list->prev = list;
}


static inline uint64_t read_cr3(void)
{
    uint64_t val;
    __asm__ volatile(
        "mov %%cr3, %0" : "=r"(val));
    return val;
}

static inline void set_cr3(uint64_t val)
{
    __asm__ volatile("mov %0, %%cr3" :: "r"(val));
}

void enable_interrupts();
void disable_interrupts();
void halt_interrupts_enabled(void);
void halt(void);
#endif
