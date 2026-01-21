/*
    Copyright (C) 2026 Aspen Software Foundation

    Module: stdint.h
    Description: The standard integer types for the VNiX Operating System.
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

#ifndef _STDINT_H
#define _STDINT_H


typedef signed char        int8_t;
typedef unsigned char      uint8_t;
typedef signed short       int16_t;
typedef unsigned short     uint16_t;
typedef signed int         int32_t;
typedef unsigned int       uint32_t;
typedef signed long        int64_t;
typedef unsigned long      uint64_t;


typedef int8_t             int_least8_t;
typedef uint8_t            uint_least8_t;
typedef int16_t            int_least16_t;
typedef uint16_t           uint_least16_t;
typedef int32_t            int_least32_t;
typedef uint32_t           uint_least32_t;
typedef int64_t            int_least64_t;
typedef uint64_t           uint_least64_t;

typedef int8_t             int_fast8_t;
typedef uint8_t            uint_fast8_t;
typedef int64_t            int_fast16_t;
typedef uint64_t           uint_fast16_t;
typedef int64_t            int_fast32_t;
typedef uint64_t           uint_fast32_t;
typedef int64_t            int_fast64_t;
typedef uint64_t           uint_fast64_t;

typedef int64_t            intptr_t;
typedef uint64_t           uintptr_t;

typedef int64_t            intmax_t;
typedef uint64_t           uintmax_t;

#define INT8_MIN           (-128)
#define INT8_MAX           127
#define UINT8_MAX          255

#define INT16_MIN          (-32768)
#define INT16_MAX          32767
#define UINT16_MAX         65535

#define INT32_MIN          (-2147483648)
#define INT32_MAX          2147483647
#define UINT32_MAX         4294967295U

#define INT64_MIN          (-9223372036854775808L)
#define INT64_MAX          9223372036854775807L
#define UINT64_MAX         18446744073709551615UL

#define INT_LEAST8_MIN     INT8_MIN
#define INT_LEAST8_MAX     INT8_MAX
#define UINT_LEAST8_MAX    UINT8_MAX

#define INT_LEAST16_MIN    INT16_MIN
#define INT_LEAST16_MAX    INT16_MAX
#define UINT_LEAST16_MAX   UINT16_MAX

#define INT_LEAST32_MIN    INT32_MIN
#define INT_LEAST32_MAX    INT32_MAX
#define UINT_LEAST32_MAX   UINT32_MAX

#define INT_LEAST64_MIN    INT64_MIN
#define INT_LEAST64_MAX    INT64_MAX
#define UINT_LEAST64_MAX   UINT64_MAX

#define INT_FAST8_MIN      INT8_MIN
#define INT_FAST8_MAX      INT8_MAX
#define UINT_FAST8_MAX     UINT8_MAX

#define INT_FAST16_MIN     INT64_MIN
#define INT_FAST16_MAX     INT64_MAX
#define UINT_FAST16_MAX    UINT64_MAX

#define INT_FAST32_MIN     INT64_MIN
#define INT_FAST32_MAX     INT64_MAX
#define UINT_FAST32_MAX    UINT64_MAX

#define INT_FAST64_MIN     INT64_MIN
#define INT_FAST64_MAX     INT64_MAX
#define UINT_FAST64_MAX    UINT64_MAX

#define INTPTR_MIN         INT64_MIN
#define INTPTR_MAX         INT64_MAX
#define UINTPTR_MAX        UINT64_MAX

#define INTMAX_MIN         INT64_MIN
#define INTMAX_MAX         INT64_MAX
#define UINTMAX_MAX        UINT64_MAX

#define PTRDIFF_MIN        INT64_MIN
#define PTRDIFF_MAX        INT64_MAX

#define SIZE_MAX           UINT64_MAX

#define SIG_ATOMIC_MIN     INT32_MIN
#define SIG_ATOMIC_MAX     INT32_MAX

#define WCHAR_MIN          INT32_MIN
#define WCHAR_MAX          INT32_MAX

#define WINT_MIN           INT32_MIN
#define WINT_MAX           INT32_MAX

#define INT8_C(c)          c
#define INT16_C(c)         c
#define INT32_C(c)         c
#define INT64_C(c)         c ## L

#define UINT8_C(c)         c
#define UINT16_C(c)        c
#define UINT32_C(c)        c ## U
#define UINT64_C(c)        c ## UL

#define INTMAX_C(c)        c ## L
#define UINTMAX_C(c)       c ## UL

#endif /* _STDINT_H */