/*
    Copyright (C) 2026 Aspen Software Foundation

    Module: tsc.c
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

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "includes/arch/x86_64/io.h"
#include "util/includes/util.h"
#include "util/includes/math.h"
#include "util/includes/pit.h"
#include "includes/time/tsc.h"
#include "includes/time/time.h"

uint64_t CPU_clock_speed = 0; // in hz



// Measure elapsed TSC ticks over a PIT countdown. NOTE: I might have slightly over-engineered this function.
uint64_t measure_tsc_over_pit(uint16_t pit_reload) {
    if (pit_reload == 0 || pit_get_is_legit()) 
        return 0; /* Technically 0 ticks elapse every 0 PIT cycles. T
        The only way this can be wrong is in 99698 AD when they have infinite ticks per 0 PIT cycles, 
        or If the PIT is broken and just returns garbage. must be checked for as we might get a division by zero error */

    bool unary = pit_reload <= 0x7FFF;
    uint16_t tickrate = unary ? pit_reload*2 : pit_reload;
    uint16_t skiprate = unary ? pit_reload   : 0;
    pit_start_one_shot(tickrate);   // start PIT countdown
    uint64_t tsc_start = read_tsc_serialized();

    // poll PIT counter until it reaches zero
    while (pit_read_counter() > skiprate);

    uint64_t tsc_end = read_tsc_serialized();
    uint16_t remaining_ticks = pit_read_counter();

    uint16_t elapsed_ticks = tickrate - remaining_ticks;
    uint64_t tsc_estimate = tsc_end - tsc_start;
    if (elapsed_ticks > 0 && elapsed_ticks != pit_reload) {
        tsc_estimate = (tsc_estimate * pit_reload) / elapsed_ticks;
    }

    return tsc_estimate;
}


uint64_t get_cpu_clock_speed_khz(void) {
    if (!cpu_has_tsc()) return 0; // TSC not supported
    uint64_t tsc_in_1_ltu = measure_tsc_over_pit(1193*4); // 1193 *2 PIT ticks equals percisely 0.999848528 *2 milliseconds (extra time for averaging).
    if (tsc_in_1_ltu == 0) return 0; // Measurement failed or no ticks elapsed

    uint64_t CPU_KHz = tsc_in_1_ltu * 1000000000 / 999848528;  // This is percisely CPU clock speed in kilohertz. Overflow shouldn't occur on any CPU slower than 3.65 THz
    return CPU_KHz / 4;
}


int8_t tsc_delay_us(uint32_t micros) {
    uint32_t last_recorded_time, start_time;
    start_time = get_time_us();
    last_recorded_time = start_time;

    if (start_time == 0)
        return -1; // Error: CPU clock speed not set
    
    while (last_recorded_time != 0 && last_recorded_time-start_time < micros) {
        last_recorded_time = get_time_us();
    }
    return last_recorded_time == 0 ? -1 : 0; // Error detection
}