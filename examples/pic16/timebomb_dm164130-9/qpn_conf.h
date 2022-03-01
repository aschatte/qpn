/*****************************************************************************
* Product: QP-nano configuration for the Timebomb example
* Last Updated for Version: 5.6.2
* Date of the Last Update:  2016-04-05
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* http://www.state-machine.com
* mailto:info@state-machine.com
*****************************************************************************/
#ifndef QPN_CONF_H
#define QPN_CONF_H

#if (defined DM164130_9)
    #include <xc.h>
    #include <pic16f1829.h>
#endif  //  #if (defined DM164130_9)

/* If the project is built outside the MPLAB-X project at lest one of the
 * following three build configurations must be specified: */
//#define XPRJ_Hsm
//#define XPRJ_QPN_NoVtables
//#define XPRJ_QPN_Vtables
#if   (defined XPRJ_Hsm)
    /* This configuration uses the least device resources (Flash & RAM). */
#elif (defined XPRJ_QPN_NoVtables)
    /* If the state machines use only the built-in init, dispatch, post, & postISR
     * functions, define NO_QActiveVtable to save ~% flash and ~% RAM usage. */
    #define NO_QActiveVtable
#elif (defined XPRJ_QPN_Vtables)
    /* This configuration mimics the QPN release most closely. */
    #undef NO_QActiveVtable
#else
    #error One of the project build configurations must be specified.
    //  This is done automatically by the Build Configuration switch in MPLAB-X.
    //  Use independent of MPLAB-X will need to specify one of these immediately
    //  before this preprocessor directive.
#endif  //  #if   !(defined XPRJ_Hsm) && !(defined XPRJ_QPN_NoVtables) && !(defined XPRJ_QPN_Vtables)

/* QP-nono configuration parameters. */
#define Q_PARAM_SIZE        0
#define QF_MAX_TICK_RATE    1
#define QF_TIMEEVT_CTR_SIZE 2
#undef QF_TIMEEVT_PERIODIC
#undef QF_TIMEEVT_USAGE

/* Time bomb application parameters. */
#define TIMEBOMB_QUEUE_LEN  10  // Event queue size in events
#define QF_MARGIN           0   // 0xFF- QF_NO_MARGIN, 0- no margin function calls
#undef USE_FULL_TEST            // undef- no extra test code

#endif  /* QPN_CONF_H */
