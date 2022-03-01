/*****************************************************************************
* Product: QP-nano configuration for the Blinky example
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


/* If the state machines use only the built-in init, dispatch, post, & postISR
 * functions, define NO_QActiveVtable to save ~% flash and ~% RAM usage. */
#undef NO_QActiveVtable
#define Q_PARAM_SIZE        0
#define QF_MAX_TICK_RATE    1
#define QF_TIMEEVT_CTR_SIZE 2
#undef QF_TIMEEVT_PERIODIC
#undef QF_TIMEEVT_USAGE
#define TIMEBOMB_QUEUE_LEN  10
#define QF_MARGIN           0   // 0xFF- QF_NO_MARGIN, 0- no margin function calls
#undef USE_FULL_TEST            // undef- no extra test code

#endif  /* QPN_CONF_H */
