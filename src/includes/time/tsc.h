/*
    Copyright (C) 2026 Aspen Software Foundation

    Module: tsc.h
    Description: TSC module for the VNiX Operating System.
    Author: Mejd Almohammedi

    All components of the VNiX Operating System, except where otherwise noted, 
    are copyright of the Aspen Software Foundation (and the corresponding author(s)) and licensed under GPLv2 or later.
    For more information on the Gnu Public License Version 2, please refer to the LICENSE file
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

#ifndef TSC_H
#define TSC_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "includes/arch/x86_64/io.h"
#include "util/includes/math.h"
extern uint64_t CPU_clock_speed;

uint64_t measure_tsc_over_pit(uint16_t pit_reload);
uint64_t get_cpu_clock_speed_khz(void);
int8_t tsc_delay_us(uint32_t micros);


static inline bool cpu_has_tsc(void) {
    uint32_t eax, ebx, ecx, edx;
    eax = 1; // function 1: feature flags
    asm("cpuid"
        : "=d"(edx), "=a"(eax), "=b"(ebx), "=c"(ecx)
        : "a"(eax)
        : );
    return (edx & (1 << 4)) != 0; // bit 4 = TSC
}

static inline uint64_t read_tsc_serialized(void) {
    uint32_t lo, hi;

    // Serialize instructions before reading TSC
    asm(
        "cpuid\n"           // serialize
        "rdtsc\n"           // read TSC
        : "=a"(lo), "=d"(hi)
        : "a"(0)             // CPUID with eax=0
        : "ebx", "ecx"
    );

    return ((uint64_t)hi << 32) | lo;
}

static inline int8_t set_CPU_clock_speed_partial(void) {
    if (CPU_clock_speed == 0)
        CPU_clock_speed = get_cpu_clock_speed_khz() * 1000;  // set in hz

    return CPU_clock_speed == 0 ? -1 : 0; // Error detection
}

static inline int8_t set_CPU_clock_speed(void) {
    if (CPU_clock_speed != 0)
        return 0; // Already set

    uint64_t accumulate = 0;
    for (int i=0; i<10; i++) {
        set_CPU_clock_speed_partial();
        if (CPU_clock_speed == 0) 
            return -1;

        accumulate += CPU_clock_speed/1000; // in khz
    }

    CPU_clock_speed = accumulate * 100; // average in hz
    return 0; // Success
}

#endif // TSC_H