/**
* @file
* @brief QF-micro port, PIC16, QV kernel, MPLABX-XC8 compiler
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
#ifndef QFN_PORT_H
#define QFN_PORT_H

#include <xc.h>
#include "qpn_conf.h"


/* The maximum number of active objects in the application, see NOTE01 */
#define QF_MAX_ACTIVE           1U

#define Q_ROM_VAR
#define Q_ROM

/* QF interrupt disable/enable, see NOTE02 */
#define QF_INT_DISABLE()    	{INTCONbits.GIE = 0;}
#define QF_INT_ENABLE()    		{INTCONbits.GIE = 1;}

#include <stdint.h>      /* Exact-width types. WG14/N843 C99 Standard */
#include <stdbool.h>     /* Boolean type.      WG14/N843 C99 Standard */

// Display the build configuration and switch in the Hsm, if needed.
#if   (defined XPRJ_Hsm)
    #include "Hsm.h"        /* QP-micro hierarchical state machine */
#elif (defined XPRJ_QPN_Vtables) || (defined XPRJ_QPN_NoVtables)
#else
    #error Project configuration not correctly set.
#endif  //  #if (defined PRJ_Hsm)
#include "qepn.h"
#include "qfn.h"        /* QF-nano platform-independent public interface */
#include "qvn.h"        /* QV-nano platform-independent public interface */

/*****************************************************************************
* NOTE01:
* The maximum number of active objects QF_MAX_ACTIVE can be increased up to
* 64, inclusive. The lower limit of 8 is used here to save RAM and also
* because the internal scheduler algorithms perform slightly better for this
* lower limit.
*/

#endif /* QFN_PORT_H */

