/*****************************************************************************/
/** \file Hsm.c
 *  This file implements the Hierarchical State Machine (Hsm) Package.
 *
 * Contact information: Alvin Schatte, Consultant
 * [EmailTo]AlvinSchatte@skytex.net
 * Web:
 *
 * Copyright 2022 Schatte Family Enterprises  ALL RIGHTS RESERVED
 * Schatte Family Enterprises, 12399 S Hwy 121, Trenton, TX 75490
 *
 *****************************************************************************/

//****************************************
//  include files
//****************************************
#include "Hsm.h"
#include "qassert.h"

//****************************************
//  local defines
//****************************************

Q_DEFINE_THIS_MODULE("Hsm")

//****************************************
// global variable definitions
//****************************************
HsmEvent HSM_reservedEvt_[HSM_USER_SIG] = {HSM_EMPTY_SIG, HSM_ENTRY_SIG, HSM_EXIT_SIG, HSM_INIT_SIG};

HsmState QHsm_top(Hsm *me) {
    (void)me; /* avoid the compiler warning about unused parameter */
    return HSM_IGNORED(); /* the top state ignores all events */
}

//****************************************
//  local variables
//****************************************


//****************************************
//  local functions
//****************************************
/****************************************************************************/
/**
* @description
* Performs the first step of HSM initialization by assigning the initial
* pseudostate to the currently active state of the state machine.
*
* @param[in,out] me      pointer (see @ref oop)
* @param[in]     initial pointer to the top-most initial state-handler
*                        function in the derived state machine
* @note
* Must be called only by the constructors of the derived state machines.
*
* @note
* Must be called only ONCE before QHSM_INIT().
*
* @usage
* The following example illustrates how to invoke QHsm_ctor() in the
* "constructor" of a derived state machine:
* @include qepn_qhsm_ctor.c
*/
void QHsm_ctor(QHsm * const me, QStateHandler initial) {
    #ifdef __Hsm_H
    #else
        static QHsmVtable const vtable = { /* QHsm virtual table */
            &QHsm_init_,
            &QHsm_dispatch_
        };
        me->vptr  = &vtable;
        me->temp  = initial;
    #endif  //  #ifdef __Hsm_H
    me->state = initial;
}

/*  Initialize the Fsm, me, usually with a dummy event, e.
 */
void Hsm_init(Hsm *me)
{
    HsmStateHandler t = (HsmStateHandler)QHsm_top;
    HsmState r;
    Q_ALLEGE((me->state)(me) == HSM_RET_TRAN);      // Execute the top-most initial transition.
    do {                                                /* drill into the target. . . */
        HsmStateHandler path[HSM_MAX_NEST_DEPTH];
        int8_t ip = (int8_t)0;                          /* transition entry path index */
        path[0] = me->state;                            /* save the target of the initial transition */
        (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EMPTY_SIG;
        (void)HSM_TRIG_(me->state, HSM_EMPTY_SIG);
        while (me->state != t) {
            path[(uint8_t)++ip] = (me->state);
            (void)HSM_TRIG_(me->state, HSM_EMPTY_SIG);
        }
        me->state = path[0];                            /* restore the target of the initial tran. */
        Q_ASSERT(ip < (int8_t)HSM_MAX_NEST_DEPTH);      /* entry path must not overflow */
        do {                                            /* retrace the entry path in reverse (desired) order. . . */
            (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_ENTRY_SIG;
            HSM_ENTER_(path[(uint8_t)ip]);                       /* enter path [ip] */
        } while ((--ip) >= (int8_t)0);
        t = path[0];                                    /* current state becomes the new source */
        (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_INIT_SIG;
        r = HSM_TRIG_(t, HSM_INIT_SIG);
    } while (r == HSM_RET_TRAN);
    me->state = t;
}


/** Dispatch the event, e, to the Hsm, me, and handle state transitions, including
 *  exit and entry conditions, if needed.
 */
void Hsm_dispatch(Hsm *me)
{
    HsmStateHandler path[HSM_MAX_NEST_DEPTH];
    HsmStateHandler s;
    HsmStateHandler t;
    HsmState r;

    t = me->state;  // save the current state

    do {            // process the event hierarchically
        s = me->state;
        r = s(me);   // invoke state handler, s

        if (r == HSM_RET_UNHANDLED) {           /* unhandled due to a guard? */
            (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EMPTY_SIG;
            r = HSM_TRIG_(s, HSM_EMPTY_SIG);    /* find superstate of s, put result in r */
            s = me->state;                      /* superstate to s */
        }
    } while (r == HSM_RET_SUPER);

    if (r >= HSM_RET_TRAN) {        // transition taken
        int8_t ip = (int8_t)(-1);   /* transition entry path index */
        int8_t iq;                  /* helper transition entry path index */

        path[0] = me->state;       /* save the target of the transition */
        path[1] = t;

        while (t != s) {
//            (s)(me, &FSM_reservedEvt_[FSM_EXIT_SIG]);             //  exit the source
            (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EXIT_SIG;
            r = HSM_TRIG_(t, HSM_EXIT_SIG);
            if (r == HSM_RET_HANDLED) {    /*exit handled? */
                (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EMPTY_SIG;
                (void)HSM_TRIG_(t, HSM_EMPTY_SIG);                  /* find superstate of t */
            }
            t = me->state;                                          /* me->state holds the superstate */
        }
        t = path[0]; /* target of the transition */

        if (s == t) {       /* (a) check source==target (transition to self) */
            (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EXIT_SIG;
            HSM_EXIT_(s)    /* exit the source */
            ip = (int8_t)0; /* enter the target */
        }
        else {
            (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EMPTY_SIG;
            (void)HSM_TRIG_(t, HSM_EMPTY_SIG); /* superstate of target */
            t = me->state;
            if (s == t) {       /* (b) check source¼¼target->super */
                ip = (int8_t)0; /* enter the target */
            }
            else {
                (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EMPTY_SIG;
                (void)HSM_TRIG_(s, HSM_EMPTY_SIG); /* superstate of src */
                                                    /* (c) check source->super==target->super */
                if (me->state == t) {
                    (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EXIT_SIG;
                    HSM_EXIT_(s)                    /* exit the source */
                    ip = (int8_t)0;                 /* enter the target */
                }
                else {
                                                    /* (d) check source->super==target */
                    if (me->state == path[0]) {
                        (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EXIT_SIG;
                        HSM_EXIT_(s)                /* exit the source */
                    }
                    else { /* (e) check rest of source==target->super->super..
                            * and store the entry path along the way
                            */
                        iq = (int8_t)0; /* indicate that LCA not found */
                        ip = (int8_t)1; /* enter target and its superstate */
                        path[1] = t;   /* save the superstate of target */
                        t = me->state;  /* save source->super */
                                        /* find target->super->super */
                        (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EMPTY_SIG;
                        r = HSM_TRIG_(path[1], HSM_EMPTY_SIG);
                        while (r == HSM_RET_SUPER) {
                            path[(uint8_t)++ip] = me->state; /* store the entry path */
                            if (me->state == s) {   /* is it the source? */
                                iq = (int8_t)1;     /* indicate that LCA found */
                                                    /* entry path must not overflow */
                                Q_ASSERT(ip < (int8_t)HSM_MAX_NEST_DEPTH);
                                --ip;               /* do not enter the source */
                                r = HSM_RET_HANDLED;  /* terminate the loop */
                            }
                            else {                  /* it is not the source, keep going up */
                                (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EMPTY_SIG;
                                r = HSM_TRIG_(me->state, HSM_EMPTY_SIG);
                            }
                        }
                        if (iq == (int8_t)0) {          /* the LCA not found yet? */
                                                        /* entry path must not overflow */
                            Q_ASSERT(ip < (int8_t)HSM_MAX_NEST_DEPTH);
                            (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EXIT_SIG;
                            HSM_EXIT_(s)                /* exit the source */
                                                        /* (f) check the rest of source->super == target->super->super...*/
                            iq = ip;
                            r = HSM_RET_IGNORED;          /* indicate LCA NOT found */
                            do {
                                if (t == path[(uint8_t)iq]) {   /* is this the LCA? */
                                r = HSM_RET_HANDLED;      /* indicate LCA found */
                                ip = (int8_t)(iq - 1);  /*do not enter LCA*/
                                iq = (int8_t)(-1);      /* terminate the loop */
                                }
                                else {
                                    --iq;               /* try lower superstate of target */
                                }
                            } while (iq >= (int8_t)0);

                            if (r != HSM_RET_HANDLED) { /* LCA not found yet? */
                                                        /* (g) check each source->super->... for each target->super...*/
                                r = HSM_RET_IGNORED;    /* keep looping */
                                do {
                                                            /* exit t unhandled? */
                                    (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EXIT_SIG;
                                    if (HSM_TRIG_(t, HSM_EXIT_SIG) == HSM_RET_HANDLED) {
                                        (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EMPTY_SIG;
                                        (void)HSM_TRIG_(t, HSM_EMPTY_SIG);
                                    }
                                    t = me->state;          /* set to super of t */
                                    iq = ip;
                                    do {
                                        if (t == path[(uint8_t)iq]) {       /* is this LCA? */
                                                                    /* do not enter LCA */
                                            ip = (int8_t)(iq - 1);
                                            iq = (int8_t)(-1);      /*break inner */
                                            r = HSM_RET_HANDLED;    /*break outer */
                                        }
                                        else {
                                            --iq;
                                        }
                                    } while (iq >= (int8_t)0);
                                } while (r != HSM_RET_HANDLED);
                            }
                        }
                    }
                }
            }
        }

//        (*me->state)(me, &FSM_reservedEvt_[FSM_ENTRY_SIG]);         //  enter the target
        for (; ip >= (int8_t)0; --ip) { /* retrace the entry path in reverse (desired) order... */
            (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_ENTRY_SIG;
            HSM_ENTER_(path[(uint8_t)ip]);       /* enter path[ip] */
        }
        t = path[0];                   /* stick the target into register */
        me->state = t;                  /* update the current state */

                                        /* drill into the target hierarchy... */
        (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_INIT_SIG;
        while (HSM_TRIG_(t, HSM_INIT_SIG) == HSM_RET_TRAN) {
            ip = (int8_t)0;
            path[0] = me->state;
            (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EMPTY_SIG;
            (void)HSM_TRIG_(me->state, HSM_EMPTY_SIG); /* find superstate */
            while (me->state != t) {
                path[(uint8_t)++ip] = me->state;
                (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_EMPTY_SIG;
                (void)HSM_TRIG_(me->state, HSM_EMPTY_SIG); /* find superstate */
            }
            me->state = path[0];
                                        /* entry path must not overflow */
            Q_ASSERT(ip < (int8_t)HSM_MAX_NEST_DEPTH);

            do {                        /* retrace the entry path in reverse (correct) order... */
                (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_ENTRY_SIG;
                HSM_ENTER_(path[(uint8_t)ip])       /* enter path[ip] */
            } while ((--ip) >= (int8_t)0);
            t = path[0];
            (((Hsm *)(me))->evt.sig) = (HsmSignal)HSM_INIT_SIG;
        }
    }
    me->state = t;  /* set new state or restore the current state */
}

