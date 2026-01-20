/*
    Copyright (C) 2026 Aspen Software Foundation

    Module: shell.c
    Description: Shell module for the Ancore Operating System.
    Author: Yazin Tantawi

    All components of the Ancore Operating System, except where otherwise noted, 
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

#include "drivers/terminal/src/cuoreterm.h"
#include <string.h>  
#include "includes/util/serial.h"
#include "includes/shell/keyboard.h"
#include <stdio.h>
#include "includes/memory/pmm.h"


extern struct terminal fb_term;

static int shell_strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

void print_string(const char* str) {
    printf("%s", str);
}

void print_char(char c) {
    printf("%c", c);
}

void cmd_help(void) {
    print_string("Available commands:\n");
    print_string("  help  - Show this help\n");
    print_string("  echo  - Echo arguments\n");
    print_string("  clear - Clear screen\n");
    print_string("  pmmstats - Gets the PMM stats\n");
}

void cmd_echo(const char* args) {
    print_string(args);
    print_char('\n');
}

void cmd_clear(void) {
    cuoreterm_clear(&fb_term);
}
void pmmstats(void){
    uint64_t total = pmm_get_total_pages();
    uint64_t free  = pmm_get_free_pages();
    uint64_t used  = pmm_get_used_pages();
        printf("PMM stats:\n");
printf("  Total pages: %llu (%llu MB)\n", total, (total * PAGE_SIZE) / (1024 * 1024));
printf("  Free pages : %llu (%llu MB)\n", free,  (free  * PAGE_SIZE) / (1024 * 1024));
printf("  Used pages : %llu (%llu MB)\n", used,  (used  * PAGE_SIZE) / (1024 * 1024));
}


void execute(char* buffer) {
    // skip leading spaces
    while (*buffer == ' ') buffer++;
    
    //if empty, return
if (*buffer == '\0') return;
    
    // find command end
    char* args = buffer;
    while (*args && *args != ' ') args++;
    
    // separate da command from arguments
    int has_args = 0;
    if (*args) {
        *args = '\0';
        args++;
        while (*args == ' ') args++;
        has_args = (*args != '\0');
    }
    
    // execute commands that are built in
if (shell_strcmp(buffer, "help") == 0) {
        cmd_help();
    } else if (shell_strcmp(buffer, "echo") == 0) {
        if (has_args) {
            cmd_echo(args);
        } else {
            print_char('\n');
        }
    } else if (shell_strcmp(buffer, "clear") == 0) {
        cmd_clear();
    }
    else if (shell_strcmp(buffer, "pmmstats") == 0)
    {
        pmmstats();
    }
     
    else {
        print_string("Unknown command: ");
        print_string(buffer);
        print_string("\nType 'help' for available commands.\n");
    }
}

//loop
void shell_main(void) {
    char buffer[256];
    int pos;
    char c;
    
    printf("ancoreOS Interactive Shell v1.0\n");
    printf("Type 'help' for commands\n\n");
    
    while (1) {
        printf("ancoreOS@root:$> ");
        
        pos = 0;
        while (1) {
            c = getc();
            
            if (c == '\n' || c == '\r') {
                printf("\n");
                break;
            } else if (c == '\b' || c == 127) {  // backspace or DEL
            if (pos > 0) {
                    pos--;
                    printf("\b \b");  //delete the char
                }
            } else if (c >= 32 && c < 127 && pos < 255) {  // printable char
                buffer[pos++] = c;
                printf("%c", c);  // echo char
            }
        }
        
        buffer[pos] = '\0';
        
        execute(buffer);
    }
}
//note: chars that are deleted dont look deleted