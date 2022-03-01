/*****************************************************************************/
/** \file DM164130_9.h
 *   This header documents DM164130_9 global object declarations.
 *
 * Contact information: Alvin Schatte, Consultant
 * [EmailTo]AlvinSchatte@skytex.net
 * Web:
 *
 * Copyright 2022 Schatte Family Enterprises  ALL RIGHTS RESERVED
 * Schatte Family Enterprises, 12399 S Hwy 121, Trenton, TX 75490
 *
 *****************************************************************************/
#ifndef DM164130_9_H
#define DM164130_9_H

#undef USE_BUILD_CONFIG_MSG
////  Following shows the operation of Configurations
//#if   (defined XPRJ_DM164130_9)
    #ifdef USE_BUILD_CONFIG_MSG
        #warning DM164130-9: Using the DM164130-9 board.
#endif  //  #ifdef USE_BUILD_CONFIG_MSG
    #ifndef DM164130_9
        #define DM164130_9
    #endif  //  #define DM164130_9
    #ifndef PIC16F1829
        #define PIC16F1829
    #endif  //  #ifndef PIC16F1829
//#else   //  #if   (defined XPRJ_DM164130_9)
//    #warning Incorrect or no board is specified.
//#endif  //  #if   (defined XPRJ_DM164130_9)

#undef  USE_PRINTF          // Costs >850 bytes of program space
#define USE_INTERRUPTS
#define USE_TIMER1
#define USE_UART
//#define USE_ADC
#define USE_DIGIN
#define USE_DIGOUT
//#define USE_PWM

#if (defined USE_DIGIN)
    #define DIGITAL_INPUT_H
#endif  //  #if (defined USE_DIGIN)

#if (defined USE_DIGOUT)
    #define DIG_OUT
#endif  //  #if (defined USE_DIGOUT)

#ifdef USE_BUILD_CONFIG_MSG
    #warning DM164130-9: Using Device = PIC16F1829
#endif  //  #ifdef USE_BUILD_CONFIG_MSG


// PIC16F1829 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC     = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE     = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE    = OFF       // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE    = ON        // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP       = OFF       // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD      = OFF       // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN    = OFF       // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF       // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO     = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN    = OFF       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT      = OFF       // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN    = OFF       // PLL Enable (4x PLL disabled)
#pragma config STVREN   = ON        // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV     = HI        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), high trip point selected.)
#pragma config LVP      = OFF       // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//  Include files.
#include <xc.h>
#include <pic16f1829.h>

#include <stdint.h>     ///<  Standard types
#include <limits.h>     ///<  Standard limits
#include <stdbool.h>    ///<  bool, true, false
//#include "Hsm.h"        ///<  Hierarchical State Machine Interface.

//#ifndef APP_H
//#define APP_H

    #include "qfn_port.h"

    #define BSP_TICKS_PER_SEC 100

    enum SystemSignals {
        //Q_TIMEOUT_SIG,                // Provided by the QPN framework.
        BUTTON_PRESSED_SIG = Q_USER_SIG,
        BUTTON_RELEASED_SIG,
        BUTTON2_PRESSED_SIG,
        BUTTON2_RELEASED_SIG,
        TIMEOUT_SIG,                    // Provided for QPC implementation
        TRIG1_SIG,
        /* ... */
        MAX_SIG
    };

    //enum SystemSignals {
    //    TIMEOUT_SIG = HSM_USER_SIG,         ///<  4: 20 ms Timer tick sent to all controllers
    //
    //    //  Enumerate the mADC module signals
    //    MIN_MADC_SIG,						///<  5:Minimum S-ADC signal
    //    MADC_MULTI_IS_READY_SIG = MIN_MADC_SIG,  	///<  5: S-ADC broadcast signal providing the channel and value when an conversion cycle has completed.
    //    MADC_ADC_DONE,                      ///<  6: S-ADC ADC conversion done from an ISR.
    //    MADC_IS_BUSY,               	   	///<  7: The S-ADC is currently busy processing another channel.
    //    MADC_MULTI_AVERAGE_SAMPLE_INPUT,    ///<  8:
    //    MADC_MUTLI_AVERAGE_SAMPLE_STREAM,   ///<  9:
    //    MADC_STOP,                  		///<  10: Stop the current S-ADC operation
    //    MADC_RESUME,                		///<  11: Resume the current S-ADC operation resetting to the current parameterization
    ////    MADC_SAMPLE,                        ///<  19: Sample with the most previous ADC configuration.
    //    MAX_MADC_SIG = MADC_RESUME,			///<  11: Maximum S-ADC signal
    //
    //    //  Enumerate the sADC module signals
    //    MIN_SADC_SIG,						///<  12: Minimum S-ADC signal
    //    SADC_IS_READY_SIG = MIN_SADC_SIG,  	///<  12: S-ADC broadcast signal providing the channel and value when an conversion cycle has completed.
    //    SADC_ADC_DONE,                      ///<  13: S-ADC ADC conversion done from an ISR.
    //    SADC_IS_BUSY,               	   	///<  14: The S-ADC is currently busy processing another channel.
    //    SADC_AVERAGE_SAMPLE_INPUT,  		///<  15: Set up the S-ADC to capture, sample, and average the analog value of an input.
    //    SADC_AVERAGE_SAMPLE_STREAM, 		///<  16: Set up the S-ADC to provide a stream of averaged data
    //    SADC_STOP,                  		///<  17: Stop the current S-ADC operation
    //    SADC_RESUME,                		///<  18: Resume the current S-ADC operation resetting to the current parameterization
    //    SADC_SAMPLE,                        ///<  19: Sample with the most previous ADC configuration.
    //    SADC_STREAMING,                     ///<  20: Start Streaming the previous ADC sample command.
    //    SADC_AVERAGE_SAMPLE_INPUT_FULL,     ///<  21: Set up the S-ADC to capture, sample, and average the analog value of an input.
    //    SADC_AVERAGE_SAMPLE_STREAM_FULL, 	///<  22: Set up the S-ADC to provide a stream of averaged data
    //    MAX_SADC_SIG = SADC_STREAMING,      ///<  20: Maximum S-ADC signal
    //
    ////    RELAY_IS_READY_SIG,         ///<  Relay broadcast signal provides the channel and values.
    ////    DI_IS_READY,                ///<  TBD: Digital Input broadcast signal indicating the specified channel value.
    ////    MAX_PUB_SIG = DI_IS_READY,                ///<  the last published signal
    //    MIN_DO_SIG,                 ///<  21: the first digital output signal
    //    DO_ON_SIG = MIN_DO_SIG,         ///<  21: Turn the digital output On.
    //    DO_NOFF = DO_ON_SIG,            ///<  21:
    //    DO_OFF_SIG,                     ///<  22: Turn the digital output Off.
    //    DO_NON = DO_OFF_SIG,            ///<  22:
    //    DOP_PWM_SIG,                    ///<  23:
    //    DOP_IS_ON_SIG,              ///<  24: the output is on
    //    DOP_IS_OFF_SIG,             ///<  25: the output is off
    //    DO_PWM_SIG,                     ///<  26:
    //    MAX_DO_SIG = DO_PWM_SIG,        ///<  26: the last digital output signal.
    ////    MIN_RELAY_SIG,              ///<  the first relay signal
    ////    RELAY_SELECT_SIG = MIN_RELAY_SIG,   ///<  Relay request to acquire the Channel A side value of the Relay
    ////    RELAY_DESELECT_SIG,         ///<  Relay command to deselect the Relay, even during an active acquisition cycle.
    ////    RELAY_GET_VALUES,           ///<  TBD: Request Relay current values without starting an acquisition cycle
    ////    MAX_RELAY_SIG = RELAY_GET_VALUES,
    ////    DI_RESET_DEBOUNCE,          ///<  TBD: Digital Input command to reset the debounce of the inidcated digital input channel.
    ////    MAX_DIGITAL_INPUT_SIG = DI_RESET_DEBOUNCE,
    //
    //    MIN_TEST_SIG,                             ///<  27:
    //    ANALOG_INPUT_SAMPLE_INPUT = MIN_TEST_SIG, ///<  27:Set up the ADC to capture, and sample the analog value of an input.
    //    ANALOG_INPUT_AVERAGE_SAMPLE_INPUT,        ///<  28:Set up the S-ADC to capture, sample, and average the analog value of an input.
    //    ANALOG_INPUT_AVERAGE_SAMPLE_STREAM,       ///<  29: Set up the S-ADC to provide a stream of averaged data
    //    MADC_TEST_START_SIG,                      ///<  30:
    //    SADC_TEST_START_SIG,                      ///<  31:
    //    SADC_AVERAGE_SAMPLE_STREAM_STOP,          ///<  32: State value, not a signal, indicating that a stream had previously been stopped that is eligible for restarting.
    //    MAX_TEST_SIG = SADC_TEST_START_SIG,       ///<  30:
    //    GJ_MAX_TEST_SIG = MAX_TEST_SIG            ///<  30:
    //};
    //
    //#define SADC_SIGNAL_CNT (MAX_SADC_SIG - MIN_SADC_SIG +1)   ///<  Total number of S-ADC signals
    //#define ANALOG_INPUT_SIGNAL_CNT (MAX_MADC_SIG - MIN_MADC_SIG +1)   ///<  Total number of S-ADC signals
    //
    ////#define SADC_AVERAGE_POWER_OF_2
    ////#define SADC_AVERAGE_NOT_POWER_OF_2

    typedef enum SystemSignals SystemSignals;

//#endif  //  #ifndef APP_H


/*
 * REGISTER 12-6: ANSELA: PORTA ANALOG SELECT REGISTER
 * U-0 U-0 U-0 R/W-1/1 U-0 R/W-1/1 R/W-1/1 R/W-1/1
 *   ?   ?   ?   ANSA4   ?   ANSA2   ANSA1   ANSA0
 *
 * REGISTER 12-12: ANSELB: PORTB ANALOG SELECT REGISTER
 * U-0 U-0 R/W-1/1 R/W-1/1 U-0 U-0 U-0 U-0
 *   ?   ?   ANSB5   ANSB4   ?   ?   ?   ?
 *
 * REGISTER 12-18: ANSELC: PORTC ANALOG SELECT REGISTER
 * R/W-1/1  R/W-1/1  U-0 U-0 R/W-1/1 R/W-1/1 R/W-1/1 R/W-1/1
 * ANSC7(2) ANSC6(2)   ?   ?   ANSC3   ANSC2   ANSC1   ANSC0
 *
 * ANSA0 ANSA1 ANSA2 ANSA4 ANSB4 ANSB5 ANSC0 ANSC1 ANSC2 ANSC3 ANSC6 ANSC7tr5fggfv
 */

typedef enum ANALOG_INPUT_Assignments {
    _AN0=0,_AN1,_AN2,_AN3,_AN4,_AN5,_AN6,_AN7,_AN8,_AN9,_AN10,_AN11,_ADN0,_TEMP,_DAC,_FVR1,_AN_MAX
}AnalogAssigns;

#define AChnlMap_MAX _AN_MAX
#define ADC_VALUE_MAX   0x3FF
#define ADC_VALUE_MIN   0x000

typedef union AI_ConfigTag {
    struct {
        uint8_t ADIE    :1; //  ADC interrupt enable [0- disable ADC interrupt, 1- enable ADC interrupt]]
        uint8_t ADCS    :2; //  ADC Conversion Clock Select bits:
                            //  00 = Fosc/2
                            //  01 = Fosc/8
                            //  10 = Fosc/32 => 3.2us @ 10MHz - Select ADC conversion clock
                            //  11 = Frc (clock derived from a dedicated internal oscillator = 500 kHz max))
//        uint8_t ADCS    :3; //  ADC Conversion Clock Select bits:
//                            //  000 = Fosc/2
//                            //  001 = Fosc/8
//                            //  010 = Fosc/32 => 3.2us @ 10MHz - Select ADC conversion clock
//                            //  x11 = Frc (clock derived from a dedicated internal oscillator = 500 kHz max))
//                            //  100 = Fosc/4
//                            //  101 = Fosc/16
//                            //  110 = Fosc/64
        uint8_t ADNREF  :1; //  0 =VREF- is connected to VSS
                            //  1 =VREF- is connected to external VREF- pin
        uint8_t ADPREF  :2; //  00 =VREF+ is connected to VDD
                            //  01 = Reserved
                            //  10 =VREF+ is connected to external VREF+ pin
                            //  11 =VREF+ is connected to internal Fixed Voltage Reference (FVR) module

        uint8_t ADFM    :1; //  ADC Conversion Results Format Select bit:
                            //  1 = Right justified
                            //  0 - Left justified
    };
    uint8_t Value;
}AI_Config;


typedef enum DIGITAL_INPUT_Assignments {
     _RAI0=1,_RAI1,_RAI2,_RAI3,_RAI4,_RAI5
    ,_RBI4  ,_RBI5,_RBI6,_RBI7
    ,_RCI0  ,_RCI1,_RCI2,_RCI3,_RCI4,_RCI5,_RCI6,_RCI7
    ,_DI_ALL
}DigitalInAssigns;

enum {
      DI_Rd1
    , DI_Rd2
    , DI_Rd3
    , DI_Rd4
    , MIN_DI
    , DI_SW1 = MIN_DI
    , DI_RP1
    , MAX_DI = DI_RP1
    , N_DIGITAL_INPUTS      ///< Number of digital inputs total
    , _DI_Rd1 = _RCI1
    , _DI_Rd2 = _RCI2
    , _DI_Rd3 = _RCI3
    , _DI_Rd4 = _RCI4
    , _DI_SW1 = _RAI2
    , _DI_RP1 = _RAI4
};


typedef enum DIGITAL_OUTPUT_Assignments {
    _RAO0=1,_RAO1,_RAO2,_RAO3,_RAO4,_RAO5,
    _RBO4  ,_RBO5,_RBO6,_RBO7,
    _RCO0  ,_RCO1,_RCO2,_RCO3,_RCO4,_RCO5,_RCO6,_RCO7,
    _DO_ALL,_DO_NONE,
    _DO_INTERNAL = -1
}DigitalOutAssigns;


/*  NOTE: If changing a particular output to be a new MIN_DM164130_9_DO, set any previous
 *  enum assignments to negative values to allow using MIN_DM164130_9_DO and N_DM164130_9_DO_OUTPUTS
 *  as for limits.  */
typedef enum {
      MIN_DM164130_9_DO
    , DO_DS1 = MIN_DM164130_9_DO
    , DO_DS2
    , DO_DS3
    , DO_DS4
    , MAX_DM164130_9_DO = DO_DS4
    , N_DM164130_9_DO_OUTPUTS      ///< Number of digital outputs
    , DS1=_RCO0
    , DS2=_RCO1
    , DS3=_RCO2
    , DS4=_RCO3
} DO_Assigns;

enum DO_Pwm_Outputs {
    MIN_DOT    = MIN_DM164130_9_DO,
    DOT_DS1Buz = DO_DS1,
    DOT_DS2Red = DO_DS2,
    DOT_DS3Blu = DO_DS3,
    DOT_DS4Grn = DO_DS4,
    MAX_DOT    = MAX_DM164130_9_DO,
    N_DOT_OUTPUTS = N_DM164130_9_DO_OUTPUTS,      ///< Number of digital outputs
    DS1Buz = DS1,
    DS2Red = DS2,
    DS3Blu = DS3,
    DS4Grn = DS4
} DO_Pwm_Outputs_t;

#endif /* DM164130_9_H */
