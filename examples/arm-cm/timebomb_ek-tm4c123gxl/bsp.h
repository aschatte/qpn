/*****************************************************************************
* BSP for EK-TM4C123GXL with QP/C framework
*****************************************************************************/
#ifndef BSP_H
#define BSP_H

#include "qfn_port.h"
#include "qassert.h"

//#define EK_TM4C123GXL

#define BSP_TICKS_PER_SEC 100

void QV_onIdle(void);
void QF_onStartup(void);

void BSP_init(void);
void BSP_start(void);

void BSP_ledRedOn(void);
void BSP_ledRedOff(void);
void BSP_ledBlueOn(void);
void BSP_ledBlueOff(void);
void BSP_ledGreenOn(void);
void BSP_ledGreenOff(void);

/* These macros are useful for debugging. */
 #define BSP_allLedsOff()     BSP_ledGreenOff(); BSP_ledBlueOff(); BSP_ledRedOff()
 #define BSP_ledRedOnlyOn()   BSP_allLedsOff(); BSP_ledRedOn()
 #define BSP_ledGreenOnlyOn() BSP_allLedsOff(); BSP_ledGreenOn()
 #define BSP_ledBlueOnlyOn()  BSP_allLedsOff(); BSP_ledBlueOn()

/*---------------------------------------------------------------------------*/
enum EventSignals {
    //Q_TIMEOUT_SIG,                // Provided by the QPN framework.
    BUTTON_PRESSED_SIG = Q_USER_SIG,
    BUTTON_RELEASED_SIG,
    BUTTON2_PRESSED_SIG,
    BUTTON2_RELEASED_SIG,
    TIMEOUT_SIG,                    // Provided for QPC implementation
    /* ... */
    MAX_SIG
};

/* Active objects... */
extern QActive *AO_TimeBomb;

#endif /* BSP_H */
