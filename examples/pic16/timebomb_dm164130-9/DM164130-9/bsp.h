/*****************************************************************************/
/** \file bsp.h
 *  This header documents the Board Support package (BSP) for the DM164130_9.
 *  board.
 *
 * Contact information: Alvin Schatte, Consultant
 * [EmailTo]AlvinSchatte@skytex.net
 * Web:
 *
 * Copyright 2022 Schatte Family Enterprises  ALL RIGHTS RESERVED
 * Schatte Family Enterprises, 12399 S Hwy 121, Trenton, TX 75490
 *
 *****************************************************************************/
#ifndef BSP_H
#define BSP_H

#include "DM164130-9.h"

uint16_t    AN0_Result;


//  Time constants
#define NS_PER_US           (1000UL)
#define US_PER_MS           (1000UL)
#define MS_PER_S            (1000UL)


/* * NOTE: for simulation timing set "Instruction Frequence, Fcyc = _XTAL_FREQ/4
 * * in "Oscillator Options" for simulator set up in Project Options to get proper
 * * cycle counting/timing. */
#define _XTAL_FREQ          (8000000UL)             // cyc/sec
#undef MS_PER_TICK

#ifdef BSP_TICKS_PER_SEC
    #define TICK_RATE           (BSP_TICKS_PER_SEC)
#else
    #define TICK_RATE           (50UL)                  // Ticks/sec
#endif  //  #ifdef BSP_TICKS_PER_SEC

#ifndef MS_PER_TICK
#define MS_PER_TICK         (MS_PER_S / TICK_RATE)  // ms/Tick      1000/50 = 20
#endif  //    #ifndef MS_PER_TICK

#define TimeInHundredths(Hundths)   (((Hundths) *10U) / (MS_PER_TICK)) //  Ticks
#define T20ms                       (20UL)

//  Assign Timer 1 set up values
#define BSP_TMR1CS          (0UL)   //        00 = Timer1 clock source is instruction clock (FOSC/4)

//  INTOSC selections
#if     (BSP_TMR1CS == 0)               //        00 = Timer1 clock source is instruction clock (FOSC/4)
    #define CYCLE_P_FOSC        (4UL)                                           // cyc/FOSC
#elif   (BSP_TMR1CS == 1)               //        01 = Timer1 clock source is system clock (FOSC)
    #define CYCLE_P_FOSC        (1UL)                                           // cyc/FOSC
#endif

#define FOSC_P_SEC          (_XTAL_FREQ / CYCLE_P_FOSC)     // FOSC/sec  = 8000000/4 = 2000000

#define FOSC_P_TICK         (FOSC_P_SEC / (MS_PER_S / MS_PER_TICK))        // FOSC/Tick = 2000000/(1000/20) = 40000

#define TMR1_ADJUST         (15UL)
#define TMR1_MAX            (UINT_MAX)                                      // 65535

#define TMR1_INIT_CNT       (TMR1_MAX - FOSC_P_TICK + TMR1_ADJUST)  //  where TickRate = Ticks/sec

//#define TICK_Period_ms_DS164130_9(MS_P_TICK) (TMR1_MAX - ((FOSC_P_SEC / MS_PER_S) * MS_P_TICK) + TMR1_ADJUST)  //  65535 - 40000 + 24 = 25559
#define TICK_Period_ms_DS164130_9_PP(MS_P_TICK) ((TMR1_MAX) - ((FOSC_P_SEC / (MS_PER_S / MS_P_TICK))) + (TMR1_ADJUST))  //  65535 - 40000 + 24 = 25559


//  Calculate the Timer 1 pre-scaler for the given tick rate.
//      The PS Update constant is used to set the Timer 1 Pre-scaler and initialization value.
#define PS_UPDATE_CONSTANT ((TMR1_MAX - TMR1_ADJUST) / (FOSC_P_SEC / MS_PER_S))     //  ((65535 - 24) / ((8000000 / 4) / 1000)) = 32

#if   (MS_PER_TICK <= PS_UPDATE_CONSTANT)
#define BSP_PS  (0U)
#elif ((MS_PER_TICK / 2) <= PS_UPDATE_CONSTANT)
#define BSP_PS  (1U)
#elif ((MS_PER_TICK / 4) <= PS_UPDATE_CONSTANT)
#define BSP_PS  (2U)
#elif ((MS_PER_TICK / 8) <= PS_UPDATE_CONSTANT)
#define BSP_PS  (3U)
#endif  //  #if   (MS_P_TICK <= PS_UPDATE_CONSTANT)



//#define _LED RD6
#define BSP_ledOff()        PORTDbits.RD6 = 0
#define BSP_ledOn()         PORTDbits.RD6 = 1
#define IsBSP_ledOn()       (PORTDbits.RD6 == 1)
#define IsBSP_ledOff()      (PORTDbits.RD6 == 0)

//#define _BUZZER RD7
#define BSP_buzzerOff()     PORTDbits.RD7 = 0
#define BSP_buzzerOn()      PORTDbits.RD7 = 1
#define IsBSP_buzzerOn()    (PORTDbits.RD7 == 1)
#define IsBSP_buzzerOff()   (PORTDbits.RD7 == 0)

#define BSP_relayA()        do { PORTBbits.RB0 = 0; PORTCbits.RC7 = 0; } while(0);
#define BSP_relayB()        do { PORTBbits.RB0 = 1; PORTCbits.RC7 = 1; } while(0);
#define IsBSP_relayB()      (PORTBbits.RB0 == 1)
#define IsBSP_relayA()      (PORTBbits.RB0 == 0)


//  ADC Constants
#define FULL_RANGE      {1023,   0}
#define LOW_HALF_RANGE  { 511,   0}
#define HIGH_HALF_RANGE {1023, 512}


// Register: PORTA
// bitfield definitions
typedef union {
    struct {
        unsigned RA0                    :1;
        unsigned RA1                    :1;
        unsigned RA2                    :1;
        unsigned RA3                    :1;
        unsigned RA4                    :1;
        unsigned RA5                    :1;
        unsigned RA6                    :1;
        unsigned RA7                    :1;
    };
    unsigned Reg;
} PORTA_bits_t;

// Register: PORTB
// bitfield definitions
typedef union {
    struct {
        unsigned RB0                    :1;
        unsigned RB1                    :1;
        unsigned RB2                    :1;
        unsigned RB3                    :1;
        unsigned RB4                    :1;
        unsigned RB5                    :1;
        unsigned RB6                    :1;
        unsigned RB7                    :1;
    };
    unsigned Reg;
} PORTB_bits_t;

// Register: PORTC
// bitfield definitions
typedef union {
    struct {
        unsigned RC0                    :1;
        unsigned RC1                    :1;
        unsigned RC2                    :1;
        unsigned RC3                    :1;
        unsigned RC4                    :1;
        unsigned RC5                    :1;
        unsigned RC6                    :1;
        unsigned RC7                    :1;
    };
    unsigned Reg;
} PORTC_bits_t;

// Register: PORTD
// bitfield definitions
typedef union {
    struct {
        unsigned RD0                    :1;
        unsigned RD1                    :1;
        unsigned RD2                    :1;
        unsigned RD3                    :1;
        unsigned RD4                    :1;
        unsigned RD5                    :1;
        unsigned RD6                    :1;
        unsigned RD7                    :1;
    };
    unsigned Reg;
} PORTD_bits_t;

// Register: PORTE
// bitfield definitions
typedef union {
    struct {
        unsigned RE0                    :1;
        unsigned RE1                    :1;
        unsigned RE2                    :1;
        unsigned RE3                    :1;
    };
    unsigned Reg;
} PORTE_bits_t;

extern bool    EndOfInterrupt;

void BSP_DisableInterrupts(void);
void BSP_EnableInterrupts(void);

#ifdef USE_ADC
    void BSP_AnalogInput_ADC_On(void);
    void BSP_AnalogInput_ADC_Off(void);
    void BSP_AnalogInput_Config(AI_Config Config);
    void BSP_AnalogInput_Init(AnalogAssigns Idx);
    void BSP_AnalogInputChannelSelect(AnalogAssigns Ch);
    void BSP_AnalogInputConvert(void);
    uint16_t BSP_AnalogInputResult(void);
#endif  //  #ifdef USE_ADC

#ifdef USE_DIGIN
    void BSP_DigitalInputsInit(DigitalInAssigns Indx);
    bool BSP_DigitalInputIsOn(DigitalInAssigns Indx);
#endif  //  #ifdef USE_DIGIN

#ifdef USE_DIGOUT
    void BSP_DigOutInit(DigitalOutAssigns Indx, bool V);
    //void BSP_DigitalOutputSet(DigitalOutAssigns Indx);
    //void BSP_DigitalOutputClr(DigitalOutAssigns Indx);
    #define BSP_DigitalOutputSet(Indx_) {BSP_DigitalOutput(Indx_, true);}
    #define BSP_DigitalOutputClr(Indx_) {BSP_DigitalOutput(Indx_, false);}
    void BSP_DigitalOutput(DigitalOutAssigns Indx, bool V);
#endif  //  #ifdef USE_DIGOUT

#ifdef USE_PWM
void BSP_PWM_Setup(void);
void BSP_PWM_Start(uint8_t Period, uint16_t DutyCycle_x4);
void BSP_PWM_Stop(void);
#endif  //  #ifdef USE_PWM

void BSP_RelayInit(AnalogAssigns AChn);
void BSP_RelayDeselect(AnalogAssigns AChn);
void BSP_RelaySelectA(AnalogAssigns AChn);
void BSP_RelaySelectB(AnalogAssigns AChn);

void BSP_Timer1Init(uint16_t ms);
void BSP_Timer1Start(void);

void BSP_init(void);

#if   !((defined XPRJ_DM164130_9) || (defined XPRJ_DM164130_9_QM) || (defined DM164130_9))
    void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line);
#endif  //  #if   !(defined XPRJ_DM164130_9)
void UART_SetUp();
void UART_Start(void);
void UART_Stop(void);
void BSP_display(char const *msg);

void BSP_ledRedOn(void);
void BSP_ledRedOff(void);
void BSP_ledBlueOn(void);
void BSP_ledBlueOff(void);
void BSP_ledGreenOn(void);
void BSP_ledGreenOff(void);

#endif /* BSP_H */
