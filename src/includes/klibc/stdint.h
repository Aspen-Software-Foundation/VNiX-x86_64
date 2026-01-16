/*
    Copyright (C) 2026 Aspen Software Foundation

    Module: stdint.h
    Description: STDINT module for the Ancore Operating System
    Author: Yazin Tantawi

    All components of the Ancore Operating System, except where otherwise noted, 
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

#ifndef STDINT_H
#define STDINT_H

#include <limits.h>



#if UCHAR_MAX == 0xFF
typedef unsigned char  uint8_t;
typedef signed char    int8_t;
#else
#error "No 8-bit integer type available"
#endif



#if USHRT_MAX == 0xFFFF
typedef unsigned short uint16_t;
typedef signed short   int16_t;
#elif UINT_MAX == 0xFFFF
typedef unsigned int   uint16_t;
typedef signed int     int16_t;
#else
#error "No 16-bit integer type available"
#endif


#if UINT_MAX == 0xFFFFFFFF
typedef unsigned int  uint32_t;
typedef signed int    int32_t;
#elif ULONG_MAX == 0xFFFFFFFF
typedef unsigned long uint32_t;
typedef signed long   int32_t;
#else
#error "No 32-bit integer type available"
#endif


#if ULONG_MAX == 0xFFFFFFFFFFFFFFFF
typedef unsigned long      uint64_t;
typedef signed long        int64_t;
#elif defined(ULLONG_MAX) && ULLONG_MAX == 0xFFFFFFFFFFFFFFFF
typedef unsigned long long uint64_t;
typedef signed long long   int64_t;
#else
#error "No 64-bit integer type available"
#endif


#if UINTPTR_MAX == UINT_MAX
typedef unsigned int  uintptr_t;
typedef signed int    intptr_t;
#elif UINTPTR_MAX == ULONG_MAX
typedef unsigned long uintptr_t;
typedef signed long   intptr_t;
#elif defined(ULLONG_MAX) && UINTPTR_MAX == ULLONG_MAX
typedef unsigned long long uintptr_t;
typedef signed long long   intptr_t;
#else
#error "No pointer-sized integer type available"
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(sizeof(uint8_t)  == 1, "uint8_t wrong size");
_Static_assert(sizeof(uint16_t) == 2, "uint16_t wrong size");
_Static_assert(sizeof(uint32_t) == 4, "uint32_t wrong size");
_Static_assert(sizeof(uint64_t) == 8, "uint64_t wrong size");
#endif

#endif /* STDINT_H */
