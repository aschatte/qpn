/*$file${.::philo.c} #######################################################*/
/*
* Model: dpp_comp.qm
* File:  C:/qp_lab/qpn/examples/win32-qv/dpp_comp/philo.c
*
* This code has been generated by QM tool (https://state-machine.com/qm).
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*/
/*$endhead${.::philo.c} ####################################################*/
#include "qpn.h"  /* QP-nano port */
#include "bsp.h"  /* Board Support Package */
#include "dpp.h"  /* Application interface */

Q_DEFINE_THIS_MODULE("philo")

/* Active object class -----------------------------------------------------*/

/* Local objects -----------------------------------------------------------*/
#define THINK_TIME  \
    (QTimeEvtCtr)((BSP_random() % BSP_TICKS_PER_SEC) + (BSP_TICKS_PER_SEC/2U))
#define EAT_TIME    \
    (QTimeEvtCtr)((BSP_random() % BSP_TICKS_PER_SEC) + BSP_TICKS_PER_SEC)

/* Philo definition --------------------------------------------------------*/
/*$define${AOs::Philo} #####################################################*/
/* Check for the minimum required QP version */
#if ((QP_VERSION < 601) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8)))
#error qpn version 6.0.1 or higher required
#endif
/*${AOs::Philo} ............................................................*/
/*${AOs::Philo::ctor} ......................................................*/
void Philo_ctor(Philo * const me, uint8_t num) {
    QHsm_ctor(&me->super, Q_STATE_CAST(&Philo_initial));
    me->num = num;
}

/*${AOs::Philo::SM} ........................................................*/
QState Philo_initial(Philo * const me) {
    /*${AOs::Philo::SM::initial} */
    me->tickCtr = 0U;
    return Q_TRAN(&Philo_thinking);
}
/*${AOs::Philo::SM::thinking} ..............................................*/
QState Philo_thinking(Philo * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /*${AOs::Philo::SM::thinking} */
        case Q_ENTRY_SIG: {
            me->tickCtr = THINK_TIME;
            status_ = Q_HANDLED();
            break;
        }
        /*${AOs::Philo::SM::thinking} */
        case Q_EXIT_SIG: {
            me->tickCtr = 0U;
            status_ = Q_HANDLED();
            break;
        }
        /*${AOs::Philo::SM::thinking::Q_TIMEOUT} */
        case Q_TIMEOUT_SIG: {
            status_ = Q_TRAN(&Philo_hungry);
            break;
        }
        /*${AOs::Philo::SM::thinking::EAT, DONE} */
        case EAT_SIG: /* intentionally fall through */
        case DONE_SIG: {
            Q_ERROR(); /* these events should never arrive in this state */
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*${AOs::Philo::SM::hungry} ................................................*/
QState Philo_hungry(Philo * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /*${AOs::Philo::SM::hungry} */
        case Q_ENTRY_SIG: {
            QACTIVE_POST(&AO_Table, HUNGRY_SIG, me->num);
            status_ = Q_HANDLED();
            break;
        }
        /*${AOs::Philo::SM::hungry::EAT} */
        case EAT_SIG: {
            status_ = Q_TRAN(&Philo_eating);
            break;
        }
        /*${AOs::Philo::SM::hungry::DONE} */
        case DONE_SIG: {
            Q_ERROR(); /* this event should never arrive in this state */
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*${AOs::Philo::SM::eating} ................................................*/
QState Philo_eating(Philo * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /*${AOs::Philo::SM::eating} */
        case Q_ENTRY_SIG: {
            me->tickCtr = EAT_TIME;
            status_ = Q_HANDLED();
            break;
        }
        /*${AOs::Philo::SM::eating} */
        case Q_EXIT_SIG: {
            me->tickCtr = 0U;
            QACTIVE_POST(QF_ACTIVE_CAST(&AO_Table), DONE_SIG, me->num);
            status_ = Q_HANDLED();
            break;
        }
        /*${AOs::Philo::SM::eating::Q_TIMEOUT} */
        case Q_TIMEOUT_SIG: {
            status_ = Q_TRAN(&Philo_thinking);
            break;
        }
        /*${AOs::Philo::SM::eating::EAT, DONE} */
        case EAT_SIG: /* intentionally fall through */
        case DONE_SIG: {
            Q_ERROR(); /* these events should never arrive in this state */
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*$enddef${AOs::Philo} #####################################################*/
