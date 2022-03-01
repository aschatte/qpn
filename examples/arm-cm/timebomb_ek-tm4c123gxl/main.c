/*.$file${.::main.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*
* Model: TimeBomb.qm
* File:  ${.::main.c}
*
* This code has been generated by QM 5.1.1 <www.state-machine.com/qm/>.
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
/*.$endhead${.::main.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/* TimeBomb/Button with QP/N framework */
#include <stdbool.h>    /* Boolean type.      WG14/N843 C99 Standard */
#include "qpn_conf.h"
#include "Hsm.h"
#include "qf_port.h"
#include "bsp.h"
#include "qassert.h"

#warning ----------------   Build Configuration   ----------------
#if   (Q_PARAM_SIZE == 0)
    #warning PARAM_SIZE=0:
#elif (Q_PARAM_SIZE == 2)
    #warning PARAM_SIZE=2:
#elif (Q_PARAM_SIZE == 4)
    #warning PARAM_SIZE=4:
#else
    #warning PARAM_SIZE=ERROR:
#endif  //  #if   (Q_PARAM_SIZE == 0)
#warning ----------------   Build Configuration   ----------------


/* Active objects... */
extern QActive *AO_TimeBomb;

Q_DEFINE_THIS_MODULE("main_Hsm") /* this module name for Q_ASSERT() */

#define USE_EVENT_QUEUE
//#define USE_TIMER1

/* The TimeBomb AO =======================================================*/
/*.$declare${AOs::TimeBomb} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::TimeBomb} ........................................................*/
typedef struct TimeBomb {
/* protected: */
    QActive super;

/* public: */
    QTimer te;
    uint32_t blink_ctr;
} TimeBomb;

/* public: */
static void TimeBomb_ctor(TimeBomb * const me);

/* protected: */
static QState TimeBomb_initial(TimeBomb * const me);
static QState TimeBomb_armed(TimeBomb * const me);
static QState TimeBomb_wait4button(TimeBomb * const me);
static QState TimeBomb_blink(TimeBomb * const me);
static QState TimeBomb_pause(TimeBomb * const me);
static QState TimeBomb_boom(TimeBomb * const me);
static QState TimeBomb_defuse(TimeBomb * const me);
/*.$enddecl${AOs::TimeBomb} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

/*.$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*. Check for the minimum required QP version */
#if (QP_VERSION < 690U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpn version 6.9.0 or higher required
#endif
/*.$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*.$define${AOs::TimeBomb} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::TimeBomb} ........................................................*/
/*.${AOs::TimeBomb::ctor} ..................................................*/
static void TimeBomb_ctor(TimeBomb * const me) {
    QActive_ctor(&me->super, (QStateHandler)&TimeBomb_initial);
    //QTimeEvt_ctorX(&me->te, &me->super, TIMEOUT_SIG, 0U);
}

/*.${AOs::TimeBomb::SM} ....................................................*/
static QState TimeBomb_initial(TimeBomb * const me) {
    /*.${AOs::TimeBomb::SM::initial} */
    return Q_TRAN(&TimeBomb_wait4button);
}
/*.${AOs::TimeBomb::SM::armed} .............................................*/
static QState TimeBomb_armed(TimeBomb * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /*.${AOs::TimeBomb::SM::armed} */
        case Q_EXIT_SIG: {
            BSP_ledRedOff();
            BSP_ledGreenOff();
            BSP_ledBlueOff();
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::TimeBomb::SM::armed::initial} */
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&TimeBomb_wait4button);
            break;
        }
        /*.${AOs::TimeBomb::SM::armed::BUTTON2_PRESSED} */
        case BUTTON2_PRESSED_SIG: {
            status_ = Q_TRAN(&TimeBomb_defuse);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*.${AOs::TimeBomb::SM::armed::wait4button} ................................*/
static QState TimeBomb_wait4button(TimeBomb * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /*.${AOs::TimeBomb::SM::armed::wait4button} */
        case Q_ENTRY_SIG: {
            BSP_ledGreenOn();
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::TimeBomb::SM::armed::wait4button} */
        case Q_EXIT_SIG: {
            BSP_ledGreenOff();
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::TimeBomb::SM::armed::wait4button::BUTTON_PRESSED} */
        case BUTTON_PRESSED_SIG: {
            me->blink_ctr = 5U;
            status_ = Q_TRAN(&TimeBomb_blink);
            break;
        }
        default: {
            status_ = Q_SUPER(&TimeBomb_armed);
            break;
        }
    }
    return status_;
}
/*.${AOs::TimeBomb::SM::armed::blink} ......................................*/
static QState TimeBomb_blink(TimeBomb * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /*.${AOs::TimeBomb::SM::armed::blink} */
        case Q_ENTRY_SIG: {
            BSP_ledRedOn();
            // QTimeEvt_armX(&me->te, BSP_TICKS_PER_SEC/2, 0U);
            // QTimeEvt_armX(&((me->super).tickCtr[0]), BSP_TICKS_PER_SEC/2, 0U);
            QActive_armX(&(me->super), 0U, BSP_TICKS_PER_SEC/2);
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::TimeBomb::SM::armed::blink} */
        case Q_EXIT_SIG: {
            BSP_ledRedOff();
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::TimeBomb::SM::armed::blink::Q_TIMEOUT} */
        case Q_TIMEOUT_SIG: {
            status_ = Q_TRAN(&TimeBomb_pause);
            break;
        }
        default: {
            status_ = Q_SUPER(&TimeBomb_armed);
            break;
        }
    }
    return status_;
}
/*.${AOs::TimeBomb::SM::armed::pause} ......................................*/
static QState TimeBomb_pause(TimeBomb * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /*.${AOs::TimeBomb::SM::armed::pause} */
        case Q_ENTRY_SIG: {
            // QTimeEvt_armX(&me->te, BSP_TICKS_PER_SEC/2, 0U);
            // QTimeEvt_armX(&((me->super).tickCtr[0]), BSP_TICKS_PER_SEC/2, 0U);
            QActive_armX(&(me->super), 0U, BSP_TICKS_PER_SEC/2);
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::TimeBomb::SM::armed::pause::Q_TIMEOUT} */
        case Q_TIMEOUT_SIG: {
            --me->blink_ctr;
            /*.${AOs::TimeBomb::SM::armed::pause::Q_TIMEOUT::[blink_ctr>0]} */
            if (me->blink_ctr > 0U) {
                status_ = Q_TRAN(&TimeBomb_blink);
            }
            /*.${AOs::TimeBomb::SM::armed::pause::Q_TIMEOUT::[else]} */
            else {
                status_ = Q_TRAN(&TimeBomb_boom);
            }
            break;
        }
        default: {
            status_ = Q_SUPER(&TimeBomb_armed);
            break;
        }
    }
    return status_;
}
/*.${AOs::TimeBomb::SM::armed::boom} .......................................*/
static QState TimeBomb_boom(TimeBomb * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /*.${AOs::TimeBomb::SM::armed::boom} */
        case Q_ENTRY_SIG: {
            BSP_ledRedOn();
            BSP_ledGreenOn();
            BSP_ledBlueOn();
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&TimeBomb_armed);
            break;
        }
    }
    return status_;
}
/*.${AOs::TimeBomb::SM::defuse} ............................................*/
static QState TimeBomb_defuse(TimeBomb * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /*.${AOs::TimeBomb::SM::defuse} */
        case Q_ENTRY_SIG: {
            BSP_ledBlueOn();
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::TimeBomb::SM::defuse} */
        case Q_EXIT_SIG: {
            BSP_ledBlueOff();
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::TimeBomb::SM::defuse::BUTTON2_PRESSED} */
        case BUTTON2_PRESSED_SIG: {
            status_ = Q_TRAN(&TimeBomb_armed);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*.$enddef${AOs::TimeBomb} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

/**
    Development bifurcates here with one path developing the Hsm toward the
    Microchip midrange products and another developing the Hsm toward the QPN
    framework denoted by the BOard Support Package (BSP) being used.  EK-TM4C123GXL
    is used for Hsm->QPN development, and DM164130-9 is used for Microchip development.
*/
#if (defined EK_TM4C123GXL)

static QEvt timeBomb_queue[10];
static TimeBomb timeBomb;
QActive *AO_TimeBomb = &timeBomb.super;

/* QF_active[] array defines all active object control blocks ----*/
QActiveCB const Q_ROM Q_ROM_VAR QF_active[] = {
    { (QActive *)0              , (QEvt *)0             , 0                     },
    { (QActive *)&timeBomb.super, (QEvt *)timeBomb_queue, Q_DIM(timeBomb_queue) },
};

/* make sure that the QF_active[] array matches QF_MAX_ACTIVE in qpn_port.h */
Q_ASSERT_COMPILE(1U == Q_DIM(QF_active) - 1U);

/* the main function =========================================================*/
int main() {

    BSP_init(); /* initialize the BSP */
    QF_init(Q_DIM(QF_active));   /* initialize QP/C */

    /* create AO and start it */
    TimeBomb_ctor(&timeBomb);

    QF_run(); /* run QP/C... */
    return 0; /* NOTE: the scheduler does NOT return */
}

#elif (defined DM164130_9)

//       Usage of Event queues implemented as circular buffer for event pointers.
//    (void)QEQueue_init(&DispatchQueue, QEvt const *qSto[], QEQueueCtr const qLen);
//    (void)QEQueue_postFIFO(&DispatchQueue, DOEvent)
//    (void)QEQueue_postLIFO(&DispatchQueue, &TimeoutEvent);
//    QEvt const * Event = QEQueue_get(&DispatchQueue);

#include "qequeue.h"
QEQueue DispatchQueue;
QEQueueCtr const DispatchQueueLen = 10;
Q_ASSERT_COMPILE(DispatchQueueLen);
QEvt *DispatchQueueSto[DispatchQueueLen];

QEvt const *e;

HsmEvent        TimeoutEvent;

int main(void)
{
    //  Initialize the board level stuff.  This is done primarily at the ctor level.
    BSP_init();

    //  Set up the UART for 9600 baud, 8 data bits.
//    UART_SetUp();
//    UART_Start();

    (void)QEQueue_init(&DispatchQueue, &DispatchQueueSto[0], DispatchQueueLen);

    //  Initialize the Hsms.
    (void)TimeBomb_ctor(AO_TimeBomb);
    Hsm_init(AO_TimeBomb, (HsmEvent *)0);


//#ifdef USE_TIMER1
//    BSP_Timer1Init(MS_PER_TICK);    //  Set up Timer1 for 20ms tick rate
//    BSP_Timer1Start();
//#endif //   #ifdef USE_TIMER1

//    BSP_EnableInterrupts();

    //  Start the test.
#ifdef USE_PRINTF
    (void)printf("Start of program.\n");
#endif  //  #ifdef USE_PRINTF
    //  Initialize the Hsms.
    /** This is the background loop:  */
    while (1)
    {
        //  1.  Get the next event for the AO.
        e = QEQueue_get(&DispatchQueue);
//        Q_ASSERT(e != (QEvt const *)0);     //  Assert that the event is 'real'.
        if (e != (QEvt const *)0) {
            //  2.  Dispatch the event to the AO.
            Hsm_dispatch((Hsm *)AO_TimeBomb, e);
            //  3.  Garbage collect the event.
            ((QEvt *)e)->sig = Q_EMPTY_SIG;
            e = (QEvt const *)0;
        }
    }

 //   UART_Stop();

    return 0;
}
#endif  //  #if (BSP == EK-TM4C123GXL)
