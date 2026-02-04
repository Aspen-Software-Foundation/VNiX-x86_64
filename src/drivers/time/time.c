/*
    Copyright (C) 2026 Aspen Software Foundation

    Module: time.c
    Description: The time module for the VNiX Operating System.
    Author: Yazin Tantawi

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

/*
            The AMPOS Operating System is
            copyright under the Aspen Software Foundation (And the file's corresponding developers)
            
            This project is licensed under the GNU Public License v2;
            For more information, visit "https://www.gnu.org/licenses/gpl-2.0.en.html"
            OR see to the "LICENSE" file.

*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "includes/arch/x86_64/io.h"
#include "util/includes/util.h"
#include "util/includes/math.h"
#include "util/includes/pit.h"
#include "includes/time/tsc.h"
#include "includes/time/time.h"

t_delay_mode microdelay_mode = TSC_DELAY;


uint32_t get_time_ms(void) {
    int8_t error = set_CPU_clock_speed();

    if (error)
        return 0; // Error: CPU clock speed not set
    
    uint64_t tsc = read_tsc_serialized() * 1000;
    return (uint32_t)(tsc / CPU_clock_speed); // Convert TSC ticks to milliseconds. should be fine for the first 68 years on 8GHz CPU or more for weaker CPUs
    // (2^64 upper limit / 2^33 CPU clock cycles per second on some super CPUs) = 2^31 seconds = 68 years without reboot before astornomical bugs.
}   // looks like we implemented it. Ig I did know after all.

double get_time_ms_fp(void) {
    int8_t error = set_CPU_clock_speed();
    if (error)
        return 0.0; // cpu clock speed not set

    uint64_t tsc = read_tsc_serialized();           // raw CPU cycles
    double seconds = (double)tsc / CPU_clock_speed; // seconds with fraction
    return seconds * 1000.0;                        // milliseconds with fraction
}

uint32_t get_time_us(void) {
    int8_t error = set_CPU_clock_speed();

    if (error)
        return 0; // Error: CPU clock speed not set
    
    uint64_t tsc = read_tsc_serialized();
    uint64_t cycles_per_us = CPU_clock_speed / 1000000; // How many cycles per microsecond
    return (uint32_t)(tsc / cycles_per_us);
}

int8_t udelay(uint32_t micros) {
    if (microdelay_mode == PIT_DELAY && !pit_get_is_legit()) {
        microdelay_mode = TSC_DELAY;
    }
    if (microdelay_mode == TSC_DELAY && !cpu_has_tsc()) {
        microdelay_mode = HPET_DELAY;
    }
    switch (microdelay_mode) {
        case PIT_DELAY:
            pit_delay_us(micros); return 0;
        case PIC_DELAY:
            return -1; // Error: unknown delay mode
        case APIC_DELAY:
            return -1; // Error: unknown delay mode
        case IOAPIC_DELAY:
            return -1; // Error: unknown delay mode
        case TSC_DELAY:
            return tsc_delay_us(micros);
        case HPET_DELAY:
            return -1; // Error: unknown delay mode
        default:
            return -1; // Error: unknown delay mode
    }
}