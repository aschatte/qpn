/**
* @file
* @brief Public QEP-nano interface.
* @ingroup qepn
* @cond
******************************************************************************
* Last updated for version 6.6.0
* Last updated on  2019-10-14
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#ifndef QEPN_H
#define QEPN_H

/****************************************************************************/
/*! The current QP version as a decimal constant XXYZ, where XX is a 2-digit
* major version number, Y is a 1-digit minor version number, and Z is
* a 1-digit release number.
*/
#define QP_VERSION      693U

/*! The current QP version number string of the form XX.Y.Z, where XX is
* a 2-digit major version number, Y is a 1-digit minor version number,
* and Z is a 1-digit release number.
*/
#define QP_VERSION_STR  "6.9.3"

/*! Encrypted current QP release (6.6.0) and date (2019-10-31) */
#define QP_RELEASE      0x8295AA8AU


/****************************************************************************/
/* typedefs for basic numerical types; MISRA-C 2004 rule 6.3(req). */

/*! typedef for character strings. */
/**
* @description
* This typedef specifies character type for exclusive use in character
* strings. Use of this type, rather than plain 'char', is in compliance
* with the MISRA-C 2004 Rules 6.1(req), 6.3(adv).
*/
typedef char char_t;

/*! typedef for line numbers in assertions and return from QF_run() */
typedef int int_t;

/*! typedef for enumerations used for event signals */
typedef int enum_t;

/*! IEEE 754 32-bit floating point number, MISRA-C 2004 rule 6.3(req) */
/**
* @note QP-nano does not use floating-point types anywhere in the internal
* implementation.
*/
typedef float float32_t;

/*! IEEE 754 64-bit floating point number, MISRA-C 2004 rule 6.3(req) */
/**
* @note QP-nano does not use floating-point types anywhere in the internal
* implementation.
*/
typedef double float64_t;

/*! Scalar type describing the signal of an event. */
#ifdef __Hsm_H
    #define QSignal HsmSignal
#else
    typedef uint8_t QSignal;
#endif  //  #ifdef __Hsm_H

#ifndef Q_PARAM_SIZE
    /*! The size of event parameter Valid values 0, 1, 2, or 4; default 0 */
    #define Q_PARAM_SIZE 0U
#endif
#if (Q_PARAM_SIZE == 0U)
#elif (Q_PARAM_SIZE == 1U)
    typedef uint8_t QParam;
#elif (Q_PARAM_SIZE == 2U)
    typedef uint16_t QParam;
#elif (Q_PARAM_SIZE == 4U)
    /*! type of the event parameter. */
    /**
    * @description
    * This typedef is configurable via the preprocessor switch #Q_PARAM_SIZE.
    * The other possible values of this type are as follows: @n
    * none when (Q_PARAM_SIZE == 0U);@n
    * uint8_t when (Q_PARAM_SIZE == 1U);@n
    * uint16_t when (Q_PARAM_SIZE == 2U);@n
    * uint32_t when (Q_PARAM_SIZE == 4U); and @n
    * uint64_t when (Q_PARAM_SIZE == 8U).
    */
    typedef uint32_t QParam;
#elif (Q_PARAM_SIZE == 8U)
    typedef uint64_t QParam;
#else
    #error "Q_PARAM_SIZE defined incorrectly, expected 0U, 1U, 2U, 4U or 8U"
#endif

/****************************************************************************/
/*! Event structure. */
/**
* @description
* ::QEvt represents events, optionally with a single scalar parameter.
* @sa Q_PARAM_SIZE
* @sa ::QParam
*/
#ifdef __Hsm_H
    #define QEvt HsmEvent
#else
    typedef struct QEventTag{
        QSignal sig; /*!< signal of the event */
        #if (Q_PARAM_SIZE != 0)
            QParam par;  /*!< scalar parameter of the event */
        #endif
    } QEvt;
#endif  //  #ifdef __Hsm_H

/****************************************************************************/
/*! Macro to access the signal of the current event of a state machine */
/**
* @param[in,out] me_ pointer to a subclass of ::QHsm (see @ref oop)
*/
#ifdef __Hsm_H
    #define Q_SIG(me_) (((Hsm *)(me_))->evt.sig)
#else
    #define Q_SIG(me_)  (((QHsm *)(me_))->evt.sig)
#endif  //  #ifdef __Hsm_H

#if (Q_PARAM_SIZE != 0U)
    /*! Macro to access the parameter of the current event of a state machine */
    /**
    * @param[in,out] me_ pointer to a subclass of ::QHsm (see @ref oop)
    */
    #ifdef __Hsm_H
        #define Q_PAR(me_)  (((Hsm *)(me_))->evt.par)
    #else
        #define Q_PAR(me_)  (((QHsm *)(me_))->evt.par)
    #endif  //  #ifdef __Hsm_H
#endif  /* (Q_PARAM_SIZE != 0U) */

/****************************************************************************/
/*! Type returned from  a state-handler function. */
#ifdef __Hsm_H
    #define QState HsmState
#else
    typedef uint_fast8_t QState;
#endif  //  #ifdef __Hsm_H

/*! the signature of a state handler function */
#ifdef __Hsm_H
    #define QStateHandler HsmStateHandler
#else
    typedef QState (*QStateHandler)(void * me);
#endif  //  #ifdef __Hsm_H

/****************************************************************************/
/*! virtual table for the ::QHsm class. */
#ifdef NO_QActiveVtable
#else
    typedef struct QHsmVtable QHsmVtable;
#endif  //  #ifdef NO_QActiveVtable

/*! Hierarchical State Machine */
/**
* @description
* QHsm represents a Hierarchical Finite State Machine (HSM) with full
* support for hierarchical nesting of states, entry/exit actions,
* and initial transitions in any composite state.
*
* @note QHsm is not intended to be instantiated directly, but rather serves
* as the base structure for derivation of state machines in the application
* code.
*
* @usage
* The following example illustrates how to derive a state machine structure
* from QHsm. Please note that the QHsm member 'super' is defined as the
* _first_ member of the derived struct.
* @include qepn_qhsm.c
*
* @sa @ref oop
*/
#ifdef __Hsm_H
    #define QHsm Hsm
#else
    typedef struct {
        #ifdef NO_QActiveVtable
        #else
            QHsmVtable const *vptr; /*!< virtual pointer */
        #endif  //  #ifndef NO_QActiveVtable
        QStateHandler state;  /*!< current active state (state-variable) */
        QStateHandler temp;   /*!< temporary: tran. chain, target state, etc. */
        QEvt evt;  /*!< currently processed event in the HSM (protected) */
    } QHsm;
#endif  //  #ifdef __Hsm_H

/*! Virtual table for the QHsm class */
#ifdef NO_QActiveVtable
#else
    struct QHsmVtable {
        /*! Triggers the top-most initial transition in a HSM. */
        void (*init)(void *me);

        /*! Dispatches an event to a HSM. */
        void (*dispatch)(void *me);
    };
#endif  //  #ifdef NO_QActiveVtable

/*! Polymorphically executes the top-most initial transition in a SM. */
/**
* @param[in,out] me_ pointer (see @ref oop)
*
* @note Must be called only ONCE after the SM "constructor".
*
* @usage
* The following example illustrates how to initialize a SM, and dispatch
* events to it:
* @include qepn_qhsm_use.c
*/
#ifdef __Hsm_H
    #define QHSM_INIT(me_) Hsm_init(me_)
#else
    #ifdef NO_QActiveVtable
        #define QHSM_INIT(me_) QHsm_init_(me_)
    #else
        #define QHSM_INIT(me_) do {                 \
            Q_ASSERT((me_)->vptr);                  \
            (*((me_)->vptr->init))((QHsm *)(me_));  \
        } while (0)
    #endif  //  #ifdef NO_QActiveVtable
#endif  //  #ifdef __Hsm_H

/*! Polymorphically dispatches an event to a HSM. */
/**
* @description
* Processes one event at a time in Run-to-Completion fashion.
*
* @param[in,out] me_ pointer (see @ref oop)
*
* @note Must be called after the "constructor" and after QHSM_INIT().
*/
#ifdef __Hsm_H
    #define QHSM_DISPATCH(me_) Hsm_dispatch(me_);
#else
    #ifdef NO_QActiveVtable
        #define QHSM_DISPATCH(me_) QHsm_dispatch_(me_);
    #else
        #define QHSM_DISPATCH(me_) (*((me_)->vptr->dispatch))((QHsm *)(me_))
    #endif  //  #ifdef NO_QActiveVtable
#endif  //  #ifdef __Hsm_H

/* public methods */
/*! "constructor" of a HSM.
* @protected @memberof QHsm
*/
void QHsm_ctor(QHsm * const me, QStateHandler initial);

/*! Obtain the current active state from a HSM (read only). */
/**
* @param[in] me_ pointer (see @ref oop)
*
* @returns the current active state of a HSM
*/
#define QHsm_state(me_) (Q_STATE_CAST(Q_HSM_UPCAST(me_)->state))

/*! Obtain the current active child state of a given parent in QHsm
* @public @memberof QHsm
*/
/**
* @param[in] me_     pointer (see @ref oop)
* @param[in] parent_ pointer to the parent state-handler
* @returns the current active child state-handler of a given parent
* @note this macro is used in QM for auto-generating code for state history
*/
#define QHsm_childState(me_, parent_) \
    QHsm_childState_(Q_HSM_UPCAST(me_), Q_STATE_CAST(parent_))

/*! Helper function to obtain the current active child state of a parent */
QStateHandler QHsm_childState_(void *me,
                               QStateHandler const parent);

/*! Implementation of the top-most initial transition in QHsm.
* @private @memberof QHsm
*/
void QHsm_init_(void *me);

/*! Implementation of dispatching events to QHsm.
* @private @memberof QHsm
*/
void QHsm_dispatch_(void *me);

/*! the top-state.
* @protected @memberof QHsm
*/
#ifdef __Hsm_H
    HsmState QHsm_top(Hsm *me);
#else
    QState QHsm_top(void const * const me);
#endif  //  #ifdef __Hsm_H


/****************************************************************************/
/*! All possible values returned from state/action handlers */
#ifdef __Hsm_H
    #define Q_IGNORED   HSM_IGNORED
    #define Q_HANDLED   HSM_HANDLED
    #define Q_UNHANDLED HSM_UNHANDLED
#else
#endif  //  #ifdef __Hsm_H

enum {
    #ifdef __Hsm_H
        /* unhandled and need to "bubble up" */
        Q_RET_SUPER = HSM_RET_SUPER,     /*!< event passed to superstate to handle */
        Q_RET_UNHANDLED = HSM_RET_UNHANDLED, /*!< event unhandled due to a guard */

        /* handled and do not need to "bubble up" */
        Q_RET_HANDLED = HSM_RET_HANDLED,   /*!< event handled (internal transition) */
        Q_RET_IGNORED = HSM_RET_IGNORED,   /*!< event silently ignored (bubbled up to top) */
        Q_RET_TRAN = HSM_RET_TRAN,      /*!< event handled (regular transition) */
    #else
        /* unhandled and need to "bubble up" */
        Q_RET_SUPER,     /*!< event passed to superstate to handle */
        Q_RET_UNHANDLED, /*!< event unhandled due to a guard */

        /* handled and do not need to "bubble up" */
        Q_RET_HANDLED,   /*!< event handled (internal transition) */
        Q_RET_IGNORED,   /*!< event silently ignored (bubbled up to top) */
        Q_RET_TRAN,      /*!< event handled (regular transition) */
    #endif  //  #ifdef __Hsm_H

    Q_RET_SUPER_SUB, /*!< event passed to submachine superstate */
    /* entry/exit */
    Q_RET_ENTRY,     /*!< state entry action executed */
    Q_RET_EXIT,      /*!< state exit  action executed */

    /* no side effects */
    Q_RET_NULL,      /*!< return value without any effect */

    /* transitions need to execute transition-action table in QHsm */
//    Q_RET_TRAN,      /*!< event handled (regular transition) (PREVIOUSLY DECLARED)*/
    Q_RET_TRAN_INIT, /*!< initial transition in a state or submachine */
    Q_RET_TRAN_HIST  /*!< event handled (transition to history) */
};

/*! Perform upcast from a subclass of ::QHsm to the base class ::QHsm */
/**
* @description
* Upcasting from a subclass to superclass is a very frequent and __safe__
* operation in object-oriented programming and object-oriented languages
* (such as C++) perform such upcasting automatically. However, OOP is
* implemented in C just as a set of coding conventions (see @ref oop),
* and the C compiler does not "know" that certain types are related by
* inheritance. Therefore for C, the upcast must be performed explicitly.
* Unfortunately, pointer casting violates the advisory MISRA-C 2004 rule 11.4
* "cast pointer to pointer". This macro encapsulates this deviation and
* provides a descriptive name for the reason of this cast.
*/
#define Q_HSM_UPCAST(ptr_) ((QHsm *)(ptr_))

/*! Perform cast to ::QStateHandler. */
/**
* @description
* This macro encapsulates the cast of a specific state handler function
* pointer to QStateHandler, which violates MISRA-C 2004 rule 11.4(advisory).
* This macro helps to localize this deviation.
*
* @usage
* @include qepn_qhsm_ctor.c
*/
#ifdef __Hsm_H
    #define Q_STATE_CAST HSM_STATE_CAST
#else
    #define Q_STATE_CAST(handler_)  ((QStateHandler)(handler_))
#endif  //  #ifdef __Hsm_H

/*! Macro to call in a state-handler when it executes a regular
* or and initial transition. Applicable to both HSMs and FSMs.
* @include qepn_qtran.c
*/
#ifdef __Hsm_H
    #define Q_TRAN HSM_TRAN
#else
    #define Q_TRAN(target_)  \
        ((Q_HSM_UPCAST(me))->temp = Q_STATE_CAST(target_), (QState)Q_RET_TRAN)
#endif  //  #ifdef __Hsm_H

/*! Macro to call in a state-handler when it executes a transition
* to history. Applicable only to HSMs.
*
* @usage
* @include qepn_qhist.c
*/
#define Q_TRAN_HIST(hist_)  \
    ((Q_HSM_UPCAST(me))->temp = (hist_), (QState)Q_RET_TRAN_HIST)

/*! Macro to call in a state-handler when it designates the
* superstate of a given state. Applicable only to HSMs.
*
* @usage
* @include qepn_qtran.c
*/
#ifdef __Hsm_H
    #define Q_SUPER HSM_SUPER
#else
    #define Q_SUPER(super_)  \
        ((Q_HSM_UPCAST(me))->temp = Q_STATE_CAST(super_), (QState)Q_RET_SUPER)
#endif  //  #ifdef __Hsm_H

/*! Macro to call in a state-handler when it handles an event.
*  Applicable to both HSMs and FSMs.
*/
#ifdef __Hsm_H
    #define Q_IGNORED   HSM_IGNORED
    #define Q_HANDLED   HSM_HANDLED
#else
    #define Q_HANDLED()      ((QState)Q_RET_HANDLED)
#endif  //  #ifdef __Hsm_H

/*! Macro to call in a state-handler when it attempts to handle
* an event but a guard condition evaluates to 'false' and there is no other
* explicit way of handling the event. Applicable only to HSMs.
*/
#ifdef __Hsm_H
    #define Q_UNHANDLED HSM_UNHANDLED
#else
    #define Q_UNHANDLED()    ((QState)Q_RET_UNHANDLED)
#endif  //  #ifdef __Hsm_H


/*! QP reserved signals */
#ifdef __Hsm_H
    #define Q_EMPTY_SIG HSM_EMPTY_SIG
#else
#endif  //  #ifdef __Hsm_H
enum {
    #ifdef __Hsm_H
        Q_ENTRY_SIG = HSM_ENTRY_SIG,  /*!< signal for coding entry actions */
    #else
        Q_ENTRY_SIG = 1,  /*!< signal for coding entry actions */
    #endif  //  #ifdef __Hsm_H
    Q_EXIT_SIG,       /*!< signal for coding exit actions */
    Q_INIT_SIG,       /*!< signal for coding nested initial transitions */
    Q_TIMEOUT_SIG,    /*!< timeout signal at the default tick rate 0 */
    Q_TIMEOUT1_SIG,   /*!< timeout signal at tick rate 1 */
    Q_TIMEOUT2_SIG,   /*!< timeout signal at tick rate 2 */
    Q_TIMEOUT3_SIG,   /*!< timeout signal at tick rate 3 */
    Q_USER_SIG        /*!< first signal for the user applications */
};

/*! Perform cast from unsigned integer to a pointer of type @a type_ */
/**
* @description
* This macro encapsulates the cast to (type_ *), which QP ports or
* application might use to access embedded hardware registers.
* Such uses can trigger PC-Lint "Note 923: cast from int to pointer" and
* this macro helps to encapsulate this deviation.
*/
#define Q_UINT2PTR_CAST(type_, uintptr_)  ((type_ *)(uintptr_))

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


/****************************************************************************/
/*! the current QP version number string in ROM, based on QP_VERSION_STR */
extern char_t const Q_ROM QP_versionStr[7];

/*! get the current QP-nano version number string of the form "X.Y.Z" */
#define QP_getVersion() (QP_versionStr)

#endif /* QEPN_H */

