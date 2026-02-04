/*
    Copyright (C) 2026 Aspen Software Foundation

    Module: pit.C
    Description: PIT module for the VNiX Operating System.
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
#include "includes/util.h"
#include "includes/arch/x86_64/io.h"
#include "includes/math.h"
#include "includes/pit.h"
#include "includes/time/tsc.h"

// Read 16-bit counter from channel 0 (latch + read lo/hi)
uint16_t pit_read_counter() {
    uint8_t lo, hi;
    outb(PIT_CMD_PORT, 0x00);      // latch counter 0
    lo = inb(PIT_CHANNEL0_PORT);   // read low byte
    hi = inb(PIT_CHANNEL0_PORT);   // read high byte
    return ((uint16_t)hi << 8) | lo;
}

// Initialize PIT channel 0 in one-shot mode with a reload value
void pit_start_one_shot(uint16_t reload) {
    outb(PIT_CMD_PORT, 0x34);       // channel 0, lo/hi, mode 2 (one-shot)
    outb(PIT_CHANNEL0_PORT, reload & 0xFF);
    outb(PIT_CHANNEL0_PORT, reload >> 8);
}

void pit_delay_us(uint32_t micros) {
    if (micros > 25000) {   // Limit to 55.556 ms (max PIT reload value, 18 hz)
        // Sharding
        for (uint32_t i = 0; i < micros / 25000; i++) {
            pit_delay_us_limited(25000);
        }
        pit_delay_us_limited(micros % 25000); // Handle remaining time
        return;
    }
    pit_delay_us_limited(micros);
}

void pit_delay_us_limited(uint32_t micros) {
    if (micros == 0) return; // No delay needed

    uint16_t pit_reload = (uint16_t)((PIT_FREQ / 1000000) * micros);
    if (pit_reload == 0) pit_reload = 1; // Ensure at least 1 tick

    pit_start_one_shot(pit_reload * 2); // Start PIT countdown

    // Wait until the PIT counter reaches zero
    while (pit_read_counter() > pit_reload);

    return; // Success
}

uint8_t pit_get_is_legit() {
    pit_start_one_shot(8); // Start PIT countdown
    uint8_t old_pit_counter = pit_read_counter();
    if (!cpu_has_tsc()) {
        return 1; // Legacy hw has pit
    }

    // Wait until the PIT counter reaches zero
    uint64_t base_tsc = read_tsc_serialized();
    while (pit_read_counter() == old_pit_counter && read_tsc_serialized()-base_tsc > 24000);

    return (uint8_t)(read_tsc_serialized()-base_tsc > 24000);
}