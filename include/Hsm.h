/*****************************************************************************/
/** \file Hsm.h
 *  This header documents the Hierarchical State Machine (Hsm) package.
 *
 * Contact information: Alvin Schatte, Consultant
 * [EmailTo]AlvinSchatte@skytex.net
 * Web:
 *
 * Copyright 2022 Schatte Family Enterprises  ALL RIGHTS RESERVED
 * Schatte Family Enterprises, 12399 S Hwy 121, Trenton, TX 75490
 *
 *****************************************************************************/
#ifndef __Hsm_H
#define __Hsm_H

//****************************************
//  include files
//****************************************
#include <stdint.h>
#include <stdbool.h>    ///<  bool, true, false
#include "qpn_conf.h"   ///< Q_PARAM_SIZE

#ifndef Q_PARAM_SIZE
    /*! The size of event parameter Valid values 0, 1, 2, or 4; default 0 */
    #define Q_PARAM_SIZE 0
#endif
#if (Q_PARAM_SIZE == 0)
#elif (Q_PARAM_SIZE == 1)
    typedef uint8_t HsmParam;
#elif (Q_PARAM_SIZE == 2)
    typedef uint16_t HsmParam;
#elif (Q_PARAM_SIZE == 4)
    /*! type of the event parameter. */
    /**
    * @description
    * This typedef is configurable via the preprocessor switch #Q_PARAM_SIZE.
    * The other possible values of this type are as follows: @n
    * none when (Q_PARAM_SIZE == 0);@n
    * uint8_t when (Q_PARAM_SIZE == 1);@n
    * uint16_t when (Q_PARAM_SIZE == 2);@n
    * uint32_t when (Q_PARAM_SIZE == 4); and @n
    * uint64_t when (Q_PARAM_SIZE == 8).
    */
    typedef uint32_t HsmParam;
#elif (Q_PARAM_SIZE == 8)
    typedef uint64_t HsmParam;
#else
    #error "Q_PARAM_SIZE defined incorrectly, expected 0, 1, 2, 4 or 8"
#endif


//****************************************
// structure definitions
//****************************************
typedef uint8_t HsmSignal;

typedef struct HsmEventTag		// the event structure
{
	HsmSignal sig;				// Signal of the event
    #if (Q_PARAM_SIZE != 0)
        HsmParam par;  /*!< scalar parameter of the event */
    #endif
} HsmEvent;

/** Self-Address, Stamped Envelope with pointer to the Returned event */
typedef struct HsmSASE_Event {
    HsmEvent  Event;        ///< Event/signal signature
    HsmEvent *ReturnEvent;  ///< Pointer to the event used to return the results.
} HsmSASEvent;

typedef uint8_t HsmState;		// Status returned from a state-handler function

struct HsmTag;					// Forward declaration
// pointer to function type definition
typedef HsmState (*HsmStateHandler) (void * me);
typedef struct HsmTag {
    HsmStateHandler state;
    HsmEvent evt;
} Hsm;		// Current active state

//****************************************
// constant definitions
//****************************************
enum HsmReservedSignals
{
	HSM_EMPTY_SIG,  // signal used internally to the FSM.
    HSM_ENTRY_SIG,	// signal for coding entry actions
	HSM_EXIT_SIG,	// signal for coding exit actions
	HSM_INIT_SIG,	// signal for coding initial transitions
	HSM_USER_SIG = 8	// first signal that can be used in user applications
};

#define HSM_MAX_NEST_DEPTH  7

#define HSM_RET_SUPER       ((HsmState)0)
#define HSM_RET_UNHANDLED	((HsmState)1)
#define HSM_RET_HANDLED     ((HsmState)2)
#define HSM_RET_IGNORED     ((HsmState)3)
#define HSM_RET_TRAN        ((HsmState)4)

#define HSM_IGNORED()		(HSM_RET_IGNORED)
#define HSM_HANDLED()		(HSM_RET_HANDLED)
#define HSM_UNHANDLED()		(HSM_RET_UNHANDLED)

#define HSM_STATE_CAST(handler_)  ((HsmStateHandler)(handler_))

/** helper macro to trigger reserved event in an HSM */
#define HSM_TRIG_(state_, sig_)                         \
    ((HsmState)(*(state_))(me))

/** helper macro to trigger entry action in an HSM */
#define HSM_EXIT_(state_) \
    if (HSM_TRIG_(state_, HSM_EXIT_SIG) == HSM_RET_HANDLED) { \
        /* QS software tracing instrumentation for state entry */\
    }

/** helper macro to trigger exit action in an HSM */
#define HSM_ENTER_(state_) \
    if (HSM_TRIG_(state_, HSM_ENTRY_SIG) == HSM_RET_HANDLED) { \
        /* QS software tracing instrumentation for state exit */\
    }

#define HSM_SUPER(super_)   \
    (((Hsm *)me)->state = (HsmStateHandler)(super_), HSM_RET_SUPER)

#define HSM_TRAN(target_) \
	(((Hsm *)me)->state = (HsmStateHandler)(target_), HSM_RET_TRAN)

#define Hsm_ctor(me_, initial_) ((me_)->state = (initial_))

//****************************************
// global variable definitions
//****************************************
extern HsmEvent HSM_reservedEvt_[HSM_USER_SIG];


//****************************************
// function prototypes
//****************************************
void Hsm_init(Hsm *me);
void Hsm_dispatch(Hsm *me);

#include "qepn.h"

#endif  //  #ifndef __Hsm_H
