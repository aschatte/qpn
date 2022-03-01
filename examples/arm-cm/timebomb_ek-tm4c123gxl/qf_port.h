/**
* @file
* @brief QF/C port, PIC24/dsPIC, QV kernel, MPLABX-XC16 compiler
* @ingroup ports
* @cond
******************************************************************************
* Last updated for version 6.8.0
* Last updated on  2020-01-19
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2002-2020 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#ifndef QF_PORT_H
#define QF_PORT_H

/* The maximum number of active objects in the application, see NOTE01 */
#define QF_MAX_ACTIVE           1U

#define QF_EVENT_SIZ_SIZE       1U
#define QF_EQUEUE_CTR_SIZE      1U
#define QF_MPOOL_SIZ_SIZE       1U
#define QF_MPOOL_CTR_SIZE       1U
#ifndef QF_TIMEEVT_CTR_SIZE
    #define QF_TIMEEVT_CTR_SIZE     2U
#endif  //  #ifndef QF_TIMEEVT_CTR_SIZE

#define Q_ROM_VAR
/****************************************************************************/
/* macros for accessing data in ROM */
#ifndef Q_ROM /* if NOT defined, provide the default definition */

    /*! Macro to specify compiler-specific directive for placing a
    * constant object in ROM. */
    /**
    * @description
    * Many compilers for 8-bit Harvard-architecture MCUs provide non-standard
    * extensions to support placement of objects in different memories.
    * In order to conserve the precious RAM, QP-nano uses the Q_ROM macro for
    * all constant objects that can be allocated in ROM.
    *
    * @note
    * To override the following empty definition, you need to define the
    * Q_ROM macro in the qpn_port.h header file. Some examples of valid
    * Q_ROM macro definitions are: __code (IAR 8051 compiler), code (Keil
    * 8051 compiler), PROGMEM (gcc for AVR), __flash (IAR for AVR).
    */
    #define Q_ROM
#endif

#ifndef Q_ROM_BYTE
    /*! Macro to access a byte allocated in ROM */
    /**
    * Some compilers for Harvard-architecture MCUs, such as gcc for AVR, do
    * not generate correct code for accessing data allocated in the program
    * space (ROM). The workaround for such compilers is to explicitly add
    * assembly code to access each data element allocated in the program
    * space. The macro Q_ROM_BYTE() retrieves a byte from the given ROM
    * address.
    *
    * @note
    * The Q_ROM_BYTE() macro should be defined in the qpn_port.h header file
    * for each compiler that cannot handle correctly data allocated in ROM
    * (such as the gcc). If the macro is left undefined, the default
    * definition simply returns the parameter and lets the compiler
    * synthesize the correct code.
    */
    #define Q_ROM_BYTE(rom_var_)   (rom_var_)
#endif

#ifndef Q_ROM_PTR
    /*! Macro to access a pointer allocated in ROM */
    /**
    * Some compilers for Harvard-architecture MCUs, such as gcc for AVR, do
    * not generate correct code for accessing data allocated in the program
    * space (ROM). The workaround for such compilers is to explicitly add
    * assembly code to access each data element allocated in the program
    * space. The macro Q_ROM_PTR() retrieves an object-pointer from the given
    * ROM address. Please note that the pointer can be pointing to the object
    * in RAM or ROM.
    *
    * @note
    * The Q_ROM_PTR() macro should be defined in the qpn_port.h header file
    * for each compiler that cannot handle correctly data allocated in ROM
    * (such as the gcc). If the macro is left undefined, the default
    * definition simply returns the parameter and lets the compiler
    * synthesize the correct code.
    */
    #define Q_ROM_PTR(rom_var_)    (rom_var_)
#endif



/* fast log-base-2 with FBCL instruction, NOTE03 */
// #define QF_LOG2(n_) ((uint8_t)(15U + __builtin_fbcl(n_)))

#include <stdbool.h>    /* Boolean type.      WG14/N843 C99 Standard */
#include "Hsm.h"
#include "qfn_port.h"
#include "bsp.h"

///* Active objects... */
extern QActive *AO_TimeBomb;



#ifndef QPN_H
#define QPN_H

#if (Q_PARAM_SIZE != 0)
    /*! Deprecated API defined for backwards-compatibility */
    #define QACTIVE_POST(me_, sig_, par_) QActive_postX_((me_), QF_NO_MARGIN, (sig_), (par_));
#else
    #define QACTIVE_POST(me_, sig_) QActive_postX_((me_), QF_NO_MARGIN, (sig_))
#endif

#if (Q_PARAM_SIZE != 0)
    #define QACTIVE_POST_ISR(me_, sig_, par_)                       \
            QActive_postXISR_((me_), QF_NO_MARGIN, (sig_), (par_))
#else
    #define QACTIVE_POST_ISR(me_, sig_) QActive_postXISR_((me_), QF_NO_MARGIN, (sig_))
#endif

#endif /* QPN_H */


#ifndef QFN_H
#define QFN_H

    /*! special value of margin that causes asserting failure in case
    * event posting fails.
    */
    #define QF_NO_MARGIN ((uint_fast8_t)0xFF)

    /*! Processes all armed time events at every clock tick. */
    void QF_tickXISR(uint_fast8_t const tickRate);
    
#if (Q_PARAM_SIZE != 0)
bool QActive_postX_(QActive * const me, uint_fast8_t margin,
                    enum_t const sig, QParam const par);
#else
bool QActive_postX_(QActive * const me, uint_fast8_t margin,
                enum_t const sig);
#endif  //  #if (Q_PARAM_SIZE != 0)


#endif /* QFN_H */

/*****************************************************************************
* NOTE1:
* On Cortex-M0/M0+/M1 (architecture v6-M, v6S-M), the interrupt disabling
* policy uses the PRIMASK register to disable interrupts globally. The
* QF_AWARE_ISR_CMSIS_PRI level is zero, meaning that all interrupts are
* "kernel-aware".
*
* NOTE2:
* On Cortex-M3/M4, the interrupt disable/enable policy uses the BASEPRI
* register (which is not implemented in Cortex-M0/M0+/M1) to disable
* interrupts only with priority lower than the level specified by the
* QF_BASEPRI macro. The interrupts with priorities above QF_BASEPRI (i.e.,
* with numerical priority values lower than QF_BASEPRI) are not disabled in
* this method. These free-running interrupts are not allowed to call any QF
* services, because QF is not aware of these interrupts. Coversely, only
* "QF-aware" interrupts, with numerical values of priorities eqal to or
* higher than QF_BASEPRI, can call QF services.
*
* NOTE3:
* The QF_AWARE_ISR_CMSIS_PRI macro is useful as an offset for enumerating
* the QF-aware interrupt priority levels in the applications, whereas the
* numerical values of the QF-aware interrupts must be greater or equal to
* QF_AWARE_ISR_CMSIS_PRI. The enumerated values based on
* QF_AWARE_ISR_CMSIS_PRI can be passed directly to the CMSIS function
* NVIC_SetPriority(), which shifts them by (8 - __NVIC_PRIO_BITS) into the
* correct bit position, while __NVIC_PRIO_BITS is the CMSIS macro defining
* the number of implemented priority bits in the NVIC. Please note that
* the macro QF_AWARE_ISR_CMSIS_PRI is intended only for applications and
* is not used inside the QF port, which remains generic and not dependent
* on the number of implemented priority bits in the NVIC.
*/

// #endif /* QFN_PORT_H */



/*****************************************************************************
* NOTE01:
* The maximum number of active objects QF_MAX_ACTIVE can be increased up to
* 64, inclusive. The lower limit of 8 is used here to save RAM and also
* because the internal scheduler algorithms perform slighlty better for this
* lower limit.
*
* NOTE02:
* The DISI (disable interrupts) instruction is used for very fast,
* unconditional locking and unlocking of interrupts. The DISI #0x3FFF
* instruction disables interrupts for 16383 instruction cycles, which is
* much longer than any critical section in QP. The DISI #0 instruction
* is then used to unconditionally unlock the interrupts at the end of the
* critical section. The DISI instruction only disables interrupts with
* priority levels 1-6. Priority level 7 interrupts and all trap events still
* have the ability to interrupt the CPU when the DISI instruction is active.
* This means that from the perspective of QP, the level 7 interrupts are
* treated as non-maskable interrupts (NMIs). Such non-maskable interrupts
* _cannot_ call any QP services. In particular, they cannot post events.
*
* CAUTION: This QP port assumes that interrupt nesting is _enabled_,
* which is the default in the PIC24/dsPIC processors. Interrupt nesting should
* never be disabled by setting the NSTDIS control bit (INTCON1<15>). If you
* don't want interrupts to nest, you can always prioritize them at the same
* level. For example, the default priority level for all interrupts is 4 out
* of reset. If you don't change this level for any interrupt the nesting of
* interrupt will not occur.
*
* NOTE03:
* The FBCL instruction (Find First Bit Change Left) determines the exponent
* of a value by detecting the first bit change starting from the value’s sign
* bit and working towards the LSB. Since the PIC24/dsPIC’s barrel shifter
* uses negative values to specify a left shift, the FBCL instruction returns
* the negated exponent of a value. This value added to 15 gives the log-2.
*/

#endif /* QF_PORT_H */

