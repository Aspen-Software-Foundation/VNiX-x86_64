/*
    Copyright (C) 2026 Aspen Software Foundation

    Module: liballoc-impl.c
    Description: Liballoc implementations for the VNiX Operating System
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

#include "includes/memory/pmm.h"
#include "includes/memory/vmm.h"
#include "arch/limine.h"
#include "includes/util/serial.h"
#include <stddef.h>
#include <stdio.h>

// im also not reconfiguring the printing for any of this because i cba

int liballoc_lock() {
    // TODO: implement spinlock when we have SMP/multithreading
    __asm__ volatile("cli");
    return 0;
}

int liballoc_unlock() {
    // TODO: release spinlock
    __asm__ volatile("sti");
    return 0;
}

// maps them as virtual addresses then returns the physical address
void* liballoc_alloc(int pages) {
    if (pages <= 0) return NULL;
    
    // allocate first physical page
    uint64_t first_phys = palloc();
    if (first_phys == 0) {
        printf("ERROR: palloc failed on first page!\n");
        serial_write("ERROR: palloc failed on first page!\n", 38);
        return NULL;
    }
    
    // then allocate the rest
    for (int i = 1; i < pages; i++) {
        uint64_t phys = palloc();
        if (phys == 0) {
            printf("ERROR: palloc failed at page %d of %d\n", i, pages);
            serial_printf("ERROR: palloc failed at page %d of %d\n", i, pages);

            for (int j = 0; j < i; j++) {
                pfree(first_phys + (j * 0x1000));
            }
            return NULL;
        }
    }
    
    // map all pages into virtual address space
    uint64_t virt = first_phys + hhdm_offset;
    for (int i = 0; i < pages; i++) {
        map_page(virt + (i * 0x1000), first_phys + (i * 0x1000), 
                 PTE_WRITABLE);
    }
    
    printf("liballoc_alloc: allocated %d pages, phys=%p, virt=%p\n", pages, (void*)first_phys, (void*)virt);
    serial_printf("liballoc_alloc: allocated %d pages, phys=%p, virt=%p\n", pages, (void*)first_phys, (void*)virt);
    return (void*)virt;
}

// free 'pages' number of pages starting at ptr
int liballoc_free(void* ptr, int pages) {
    if (!ptr || pages <= 0) return -1;
    
    // convert virtual address back to physical
    uint64_t phys_addr = (uint64_t)ptr - hhdm_offset;
    
    printf("liballoc_free: freeing %d pages, virt=%p, phys=%p\n", pages, ptr, (void*)phys_addr);
    serial_printf("liballoc_free: freeing %d pages, virt=%p, phys=%p\n", pages, ptr, (void*)phys_addr);
    // unmap and free each page
    for (int i = 0; i < pages; i++) {
        unmap_page((uint64_t)ptr + (i * 0x1000));
        pfree(phys_addr + (i * 0x1000));
    }
    
    return 0;
}