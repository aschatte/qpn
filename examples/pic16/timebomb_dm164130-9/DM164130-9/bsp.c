/*****************************************************************************/
/** \file bsp.c
 *  This file implements the Board Support Package (BSP) for the DM164130_9
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
#include <stdio.h>
#include <stdbool.h>
#include "bsp.h"
#include "qfn_port.h"
#include "qassert.h"

extern QActive *AO_TimeBomb;

//  These following are used only for the GlobalIsr().
// #include "Relay.h"
//#include "../../HsmModules/DO_Pwm/DO_Pwm.h"
//#include "../../HsmModules/AnalogInputs/sADC.h"
// #include "mTest.h"


Q_DEFINE_THIS_FILE

#ifdef USE_INTERRUPTS
bool    EndOfInterrupt = false;

void BSP_DisableInterrupts(void)
    {INTCONbits.GIE = 0;}     //  Enable Global interrupt

void BSP_EnableInterrupts(void)
    {INTCONbits.GIE = 1;}     //  Enable Global interrupt

/*****************************************************************************
    FUNCTION    GlobalIsr()

    Description:
    - Global interrupt service routine

    Entrance Conditions:
    - None

    Exit Conditions:
    - appropriate isr's processed
    - interrupt flags cleared
    - interrupts re-enabled

    args:
        void

    returns:
        void

    Note:   This ISR requires 2 stack locations to process interrupt
            1 stack location for the global interrupt
            1 stack location to call the particular isr

 *****************************************************************************/
#ifdef USE_TIMER1
static uint16_t    TMR1_TickCnt = 0;
#endif//    #ifdef USE_TIMER1

void __interrupt() GlobalIsr(void)
{
#ifdef USE_TIMER1
    if (PIR1bits.TMR1IF) {
        TMR1             = TMR1_TickCnt;
        PIR1bits.TMR1IF  = 0;

#if (QF_MAX_TICK_RATE == 0)
        //  Post the TIMEOUT signal.
        #if (QF_MARGIN == 0)
            #if (Q_PARAM_SIZE != 0)
                QACTIVE_POST_ISR(AO_TimeBomb, TIMEOUT_SIG, 0U);
            #else
                QACTIVE_POST_ISR(AO_TimeBomb, TIMEOUT_SIG);
            #endif  //  #if (Q_PARAM_SIZE != 0)
        #else
            #if (Q_PARAM_SIZE != 0)
                QACTIVE_POST_X_ISR(AO_TimeBomb, QF_NO_MARGIN, TIMEOUT_SIG, 0U);
            #else
                QACTIVE_POST_X_ISR(AO_TimeBomb, QF_NO_MARGIN, TIMEOUT_SIG);
            #endif  //  #if (Q_PARAM_SIZE != 0)
        #endif  //  #if (QF_MARGIN == 0)
#else
        for(uint8_t i=0; i<QF_MAX_TICK_RATE; ++i) {QF_tickXISR(i);}
#endif  //  #if (QF_MAX_TICK_RATE == 0)

        //  Digital sensing, de-bouncing, and change signal posting.
        {
            /* state of the button debouncing, see below */
            static struct ButtonsDebouncing {
                uint8_t depressed;
                uint8_t previous;
            } buttons = { 0U, 0U };
            uint8_t current;
            uint8_t tmp;
            uint8_t volatile SW1 = BSP_DigitalInputIsOn(_DI_SW1);
            uint8_t volatile SW2 = BSP_DigitalInputIsOn(_DI_RP1);
            uint8_t Dummy = (SW1, SW2);

            (void)Dummy; (void)SW1;
            /* Perform the debouncing of buttons. The algorithm for debouncing
            * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
            * and Michael Barr, page 71.
            */
    //        current = ~GPIOF_AHB->DATA_Bits[BTN_SW1 | BTN_SW2]; /* read SW1 & SW2 */
            current = (uint8_t)~((BSP_DigitalInputIsOn(_DI_SW1) << DI_SW1) | (BSP_DigitalInputIsOn(_DI_RP1) << DI_RP1)); /* read SW1 & SW2 */
            tmp = buttons.depressed; /* save the debounced depressed buttons */
            buttons.depressed |= (uint8_t)(buttons.previous & current); /* set depressed */
            buttons.depressed &= (uint8_t)(buttons.previous | current); /* clear released */
            buttons.previous   = current; /* update the history */
            tmp ^= buttons.depressed;     /* changed debounced depressed */
            if ((uint8_t)(tmp & (1 << DI_SW1)) != 0U) {  /* debounced SW1 state changed? */
                if ((uint8_t)(buttons.depressed & (1 << DI_SW1)) != 0U) { /* is SW1 depressed? */
                    /* post the "button-pressed" event from ISR */
                    #if (QF_MARGIN == 0)
                        #if (Q_PARAM_SIZE != 0)
                            QACTIVE_POST_ISR(AO_TimeBomb, BUTTON_PRESSED_SIG, 0xAAU);
                        #else
                            QACTIVE_POST_ISR(AO_TimeBomb, BUTTON_PRESSED_SIG);
                        #endif
                    #else
                            #if (Q_PARAM_SIZE != 0)
                                QACTIVE_POST_X_ISR(AO_TimeBomb, QF_NO_MARGIN, BUTTON_PRESSED_SIG, 0xAAU);
                                if (!QACTIVE_POST_res_) Q_ERROR();  // Post did not occur, no room.
                            #else
                                QACTIVE_POST_X_ISR(AO_TimeBomb, QF_NO_MARGIN, BUTTON_PRESSED_SIG);
                                if (!QACTIVE_POST_res_) Q_ERROR();  // Post did not occur, no room.
                            #endif
                    #endif  //  #if (QF_MARGIN == 0)
                }
                else { /* the button is released */
                    /* post the "button-released" event from ISR */
                    #if (QF_MARGIN == 0)
                        #if (Q_PARAM_SIZE != 0)
                            QACTIVE_POST_ISR(AO_TimeBomb, BUTTON_RELEASED_SIG, 0x55U);
                        #else
                            QACTIVE_POST_ISR(AO_TimeBomb, BUTTON_RELEASED_SIG);
                        #endif
                    #else
                        #if (Q_PARAM_SIZE != 0)
                            QACTIVE_POST_X_ISR(AO_TimeBomb, QF_NO_MARGIN, BUTTON_RELEASED_SIG, 0x55U);
                            if (!QACTIVE_POST_res_) Q_ERROR();  // Post did not occur, no room.
                        #else
                            QACTIVE_POST_X_ISR(AO_TimeBomb, QF_NO_MARGIN, BUTTON_RELEASED_SIG);
                            if (!QACTIVE_POST_res_) Q_ERROR();  // Post did not occur, no room.
                        #endif
                    #endif  //  #if (QF_MARGIN == 0)
                }
            }
            if ((uint8_t)(tmp & (1 << DI_RP1)) != 0U) {  /* debounced SW2 state changed? */
                if ((uint8_t)(buttons.depressed & (1 << DI_RP1)) != 0U) { /* is SW2 depressed? */
                    /* post the "button-pressed" event from ISR */
                    #if (QF_MARGIN == 0)
                        #if (Q_PARAM_SIZE != 0)
                            QACTIVE_POST_ISR(AO_TimeBomb, BUTTON2_PRESSED_SIG, 0xA5U);
                        #else
                            QACTIVE_POST_ISR(AO_TimeBomb, BUTTON2_PRESSED_SIG);
                        #endif
                    #else
                        #if (Q_PARAM_SIZE != 0)
                            QACTIVE_POST_X_ISR(AO_TimeBomb, QF_NO_MARGIN, BUTTON2_PRESSED_SIG, 0xA5U);
                            if (!QACTIVE_POST_res_) Q_ERROR();  // Post did not occur, no room.
                        #else
                            QACTIVE_POST_X_ISR(AO_TimeBomb, QF_NO_MARGIN, BUTTON2_PRESSED_SIG);
                            if (!QACTIVE_POST_res_) Q_ERROR();  // Post did not occur, no room.
                        #endif
                    #endif  //  #if (QF_MARGIN == 0)
                }
                else { /* the button is released */
                    /* post the "button-released" event from ISR */
                    #if (QF_MARGIN == 0)
                        #if (Q_PARAM_SIZE != 0)
                            QACTIVE_POST_ISR(AO_TimeBomb, BUTTON2_RELEASED_SIG, 0x5AU);
                        #else
                            QACTIVE_POST_ISR(AO_TimeBomb, BUTTON2_RELEASED_SIG);
                        #endif
                    #else
                        #if (Q_PARAM_SIZE != 0)
                            QACTIVE_POST_X_ISR(AO_TimeBomb, QF_NO_MARGIN, BUTTON2_RELEASED_SIG, 0x5AU);
                            if (!QACTIVE_POST_res_) Q_ERROR();  // Post did not occur, no room.
                        #else
                            QACTIVE_POST_X_ISR(AO_TimeBomb, QF_NO_MARGIN, BUTTON2_RELEASED_SIG);
                            if (!QACTIVE_POST_res_) Q_ERROR();  // Post did not occur, no room.
                        #endif
                    #endif  //  #if (QF_MARGIN == 0)
                }
            }
        }
        EndOfInterrupt   = true;
    }
#endif//    #ifdef USE_TIMER1
}
#endif  //  #ifdef USE_INTERRUPTS


#ifdef SADC_H
void BSP_AnalogInput_ADC_On(void)
{ADCON0bits.ADON  = 1;}   //  Enable ADC: Turn on ADC module

void BSP_AnalogInput_ADC_Off(void)
{ADCON0bits.ADON  = 0;}   //  Disable ADC: Turn off ADC module

void BSP_AnalogInput_Config(AI_Config Config)
{
    //  Configure the ADC module:
#if (defined PIC16F1829)
    ADCON1bits.ADNREF  = Config.ADNREF;
    ADCON1bits.ADPREF  = Config.ADPREF;
    ADCON1bits.ADCS    = Config.ADCS;
#endif  //  #if (defined PIC16F1829)

    ADCON1bits.ADFM  = Config.ADFM;
    PIE1bits.ADIE    = Config.ADIE;
}

void BSP_AnalogInput_Init(AnalogAssigns Ch)
{
    /*  Set up Ch for analog input. */
    switch (Ch) {
#if ((defined PIC16F1822) || (defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
        case _AN0:
            TRISAbits.TRISA0 = 1;
            ANSELAbits.ANSA0 = 1;
            break;
        case _AN1:
            TRISAbits.TRISA1 = 1;
            ANSELAbits.ANSA1 = 1;
            break;
        case _AN2:
            TRISAbits.TRISA2 = 1;
            ANSELAbits.ANSA2 = 1;
            break;
        case _AN3:
            TRISAbits.TRISA4 = 1;
            ANSELAbits.ANSA4 = 1;
            break;
        case _AN4:
            TRISCbits.TRISC0 = 1;
            ANSELCbits.ANSC0 = 1;
            break;
        case _AN5:
            TRISCbits.TRISC1 = 1;
            ANSELCbits.ANSC1 = 1;
            break;
#endif  //  #if ((defined PIC16F1822) || (defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
        case _AN6:
            TRISCbits.TRISC2 = 1;
            ANSELCbits.ANSC2 = 1;
            break;
        case _AN7:
            TRISCbits.TRISC3 = 1;
            ANSELCbits.ANSC3 = 1;
            break;
        case _AN8:
            TRISCbits.TRISC6 = 1;
            ANSELCbits.ANSC6 = 1;
            break;
        case _AN9:
            TRISCbits.TRISC7 = 1;
            ANSELCbits.ANSC7 = 1;
            break;
        case _AN10:
            TRISBbits.TRISB4 = 1;
            ANSELBbits.ANSB4 = 1;
            break;
        case _AN11:
            TRISBbits.TRISB5 = 1;
            ANSELBbits.ANSB5 = 1;
            break;
        case _TEMP:
        case _DAC:
        case _FVR1:
        default:
            break;
    }
    //  Configure the ADC module:
    ADCON1bits.ADNREF = 0;
    ADCON1bits.ADPREF = 0;
    ADCON1bits.ADCS   = 2;

    if (Ch <= _AN11) ADCON0bits.CHS = Ch; else ADCON0bits.CHS = Ch | 0x10;  //  Select ADC input channel
    ADCON1bits.ADFM  = 1;   //  1 = Right justified: Select result format

    //  Configure ADC interrupt
    PIR1bits.ADIF = 0;      //  Clear ADC interrupt flag
    PIE1bits.ADIE = 0;      //  Disable ADC interrupt
}


void BSP_AnalogInputChannelSelect(AnalogAssigns Ch) {
#if (defined PIC16F1829)
    if (Ch <= _AN11) ADCON0bits.CHS = Ch; else ADCON0bits.CHS = Ch | 0x10;  //  Select ADC input channel
#endif  //  #if (defined PIC16F1829)
}

void BSP_AnalogInputConvert(void)
{
    //  Wait the required acquisition time (4.67uS with 10K ohms input impedance)
    ADCON0bits.GO = 1;              //  Start conversion by setting the GO/DONE bit.
    while (ADCON0bits.GO == 1) ;    //  Wait for ADC conversion to complete by Polling the GO/DONE bit
    PIR1bits.ADIF = 0;              //  Clear the ADC interrupt flag (required if interrupt is enabled).
}

uint16_t BSP_AnalogInputResult(void)
    {return ((uint16_t)ADRESH << 8) + (uint16_t)ADRESL;}   //  Read ADC Result
#endif  //  #ifdef SADC_H


#ifdef DIGITAL_INPUT_H
void BSP_DigitalInputsInit(DigitalInAssigns Indx)
{
//    BSP_DigitalOutputClr(Indx);
    switch (Indx) {
#if ((defined PIC16F1822) || (defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
        case _RAI0:
            TRISAbits.TRISA0 = 1;
            LATAbits.LATA0   = 0;
            ANSELAbits.ANSA0 = 0;
            WPUAbits.WPUA0   = 1;
            INLVLAbits.INLVLA0 = 0;
           break;
         case _RAI1:
            TRISAbits.TRISA1   = 1;
            LATAbits.LATA1     = 0;
            ANSELAbits.ANSA1   = 0;
            WPUAbits.WPUA1     = 1;
            INLVLAbits.INLVLA1 = 0;
            break;
        case _RAI2:
            TRISAbits.TRISA2   = 1;
            LATAbits.LATA2     = 0;
            ANSELAbits.ANSA2   = 0;
            WPUAbits.WPUA2     = 1;
            INLVLAbits.INLVLA2 = 0;
            break;
        case _RAI3:
            //  ra3 IS INPUT ONLY
            WPUAbits.WPUA3     = 0;
            INLVLAbits.INLVLA3 = 0;
            break;
        case _RAI4:
            TRISAbits.TRISA4   = 1;
            LATAbits.LATA4     = 0;
            ANSELAbits.ANSA4   = 0;
            WPUAbits.WPUA4     = 1;
            INLVLAbits.INLVLA4 = 0;
            break;
        case _RAI5:
            TRISAbits.TRISA5   = 1;
            LATAbits.LATA5     = 0;
            WPUAbits.WPUA5     = 1;
            INLVLAbits.INLVLA5 = 0;
            break;
#endif  //  #if ((defined PIC16F1822) || (defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
//        case _RAI6:
//            TRISAbits.TRISA6 = 1;
//            ANSELbits.ANS6   = 0;
//            break;
//        case _RAI7:
//            TRISAbits.TRISA7 = 1;
//            ANSELbits.ANS7   = 0;
//            break;
//        case _RBI0:
//            TRISBbits.TRISB0 = 1;
//            ANSELHbits.ANS12 = 0;
//            WPUBbits.WPUB0   = 0;
//            IOCBbits.IOCB0   = 0;
//            break;
//        case _RBI1:
//            TRISBbits.TRISB1 = 1;
//            ANSELHbits.ANS10 = 0;
//            WPUBbits.WPUB1   = 0;
//            IOCBbits.IOCB1   = 0;
//            break;
//        case _RBI2:
//            TRISBbits.TRISB2 = 1;
//            ANSELHbits.ANS8  = 0;
//            WPUBbits.WPUB2   = 0;
//            IOCBbits.IOCB2   = 0;
//            break;
//        case _RBI3:
//            TRISBbits.TRISB3 = 1;
//            ANSELHbits.ANS9  = 0;
//            WPUBbits.WPUB3   = 0;
//            IOCBbits.IOCB3   = 0;
//            break;
#if ((defined PIC16F1828) || (defined PIC16F1829))
        case _RBI4:
            TRISBbits.TRISB4   = 1;
            LATBbits.LATB4     = 0;
            ANSELBbits.ANSB4   = 0;
            WPUBbits.WPUB4     = 0;
            INLVLBbits.INLVLB4 = 0;
            break;
        case _RBI5:
            TRISBbits.TRISB5   = 1;
            LATBbits.LATB5     = 0;
            ANSELBbits.ANSB5   = 0;
            WPUBbits.WPUB5     = 0;
            INLVLBbits.INLVLB5 = 0;
            break;
        case _RBI6:
            TRISBbits.TRISB6   = 1;
            LATBbits.LATB6     = 0;
            WPUBbits.WPUB6     = 0;
            INLVLBbits.INLVLB6 = 0;
            break;
        case _RBI7:
            TRISBbits.TRISB7   = 1;
            LATBbits.LATB7     = 0;
            WPUBbits.WPUB7     = 0;
            INLVLBbits.INLVLB7 = 0;
            break;
#endif  //  #if ((defined PIC16F1828) || (defined PIC16F1829))
#if ((defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
        case _RCI0:
            TRISCbits.TRISC0   = 1;
            LATCbits.LATC0     = 0;
            ANSELCbits.ANSC0   = 0;
            WPUCbits.WPUC0     = 1;
            INLVLCbits.INLVLC0 = 0;
            break;
        case _RCI1:
            TRISCbits.TRISC1   = 1;
            LATCbits.LATC1     = 0;
            ANSELCbits.ANSC1   = 0;
            WPUCbits.WPUC1     = 1;
            INLVLCbits.INLVLC1 = 0;
            break;
        case _RCI2:
            TRISCbits.TRISC2   = 1;
            LATCbits.LATC2     = 0;
            ANSELCbits.ANSC2   = 0;
            WPUCbits.WPUC2     = 1;
            INLVLCbits.INLVLC2 = 0;
            break;
        case _RCI3:
            TRISCbits.TRISC3   = 1;
            LATCbits.LATC3     = 0;
            ANSELCbits.ANSC3   = 0;
            WPUCbits.WPUC3     = 1;
            INLVLCbits.INLVLC3 = 0;
            break;
        case _RCI4:
            TRISCbits.TRISC4   = 1;
            LATCbits.LATC4     = 0;
            WPUCbits.WPUC4     = 1;
            INLVLCbits.INLVLC4 = 0;
            break;
        case _RCI5:
            TRISCbits.TRISC5   = 1;
            LATCbits.LATC5     = 0;
            WPUCbits.WPUC5     = 1;
            INLVLCbits.INLVLC5 = 0;
            break;
#endif  //  #if ((defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
#if ((defined PIC16F1829))
        case _RCI6:
            TRISCbits.TRISC6   = 1;
            LATCbits.LATC6     = 0;
            ANSELCbits.ANSC6   = 0;
            WPUCbits.WPUC6     = 1;
            INLVLCbits.INLVLC6 = 0;
            break;
        case _RCI7:
            TRISCbits.TRISC7   = 1;
            LATCbits.LATC7     = 0;
            ANSELCbits.ANSC7   = 0;
            WPUCbits.WPUC7     = 1;
            INLVLCbits.INLVLC7 = 0;
            break;
#endif  //  #if ((defined PIC16F1829))
//        case _RDI0:
//            TRISDbits.TRISD0 = 1;
//            break;
//        case _RDI1:
//            TRISDbits.TRISD1 = 1;
//            break;
//        case _RDI2:
//            TRISDbits.TRISD2 = 1;
//            break;
//        case _RDI3:
//            TRISDbits.TRISD3 = 1;
//            break;
//        case _RDI4:
//            TRISDbits.TRISD4 = 1;
//            break;
//        case _RDI5:
//            TRISDbits.TRISD5 = 1;
//            break;
//        case _RDI6:
//            TRISDbits.TRISD6 = 1;
//            break;
//        case _RDI7:
//            TRISDbits.TRISD7 = 1;
//            break;
//        case _REI0:
//            TRISEbits.TRISE0 = 1;
//            break;
//        case _REI1:
//            TRISEbits.TRISE1 = 1;
//            break;
//        case _REI2:
//            TRISEbits.TRISE2 = 1;
//            break;
//        case _REI3:
//            TRISEbits.TRISE3 = 1;
//            break;
//        case _REI4:
//            TRISEbits.TRISE4 = 1;
//            break;
//        case _REI5:
//            TRISEbits.TRISE5 = 1;
//            break;
//        case _REI6:
//            TRISEbits.TRISE6 = 1;
//            break;
//        case _REI7:
//            TRISEbits.TRISE7 = 1;
//            break;
        default:
            break;
    }
}

bool BSP_DigitalInputIsOn(DigitalInAssigns Indx)
{
    switch (Indx) {
#if ((defined PIC16F1822) || (defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
        case _RAI0: return (uint8_t)(PORTAbits.RA0 == 1);
        case _RAI1: return (uint8_t)(PORTAbits.RA1 == 1);
        case _RAI2: return (uint8_t)(PORTAbits.RA2 == 1);
        case _RAI3: return (uint8_t)(PORTAbits.RA3 == 1);
        case _RAI4: return (uint8_t)(PORTAbits.RA4 == 1);
        case _RAI5: return (uint8_t)(PORTAbits.RA5 == 1);
#endif  //  #if ((defined PIC16F1822) || (defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
//        case _RAI6: return (uint8_t)(PORTAbits.RA6 == 1);
//        case _RAI7: return (uint8_t)(PORTAbits.RA7 == 1);
//
//        case _RBI0: return (uint8_t)(PORTBbits.RB0 == 1);
//        case _RBI1: return (uint8_t)(PORTBbits.RB1 == 1);
//        case _RBI2: return (uint8_t)(PORTBbits.RB2 == 1);
//        case _RBI3: return (uint8_t)(PORTBbits.RB3 == 1);
#if ((defined PIC16F1828) || (defined PIC16F1829))
        case _RBI4: return (uint8_t)(PORTBbits.RB4 == 1);
        case _RBI5: return (uint8_t)(PORTBbits.RB5 == 1);
        case _RBI6: return (uint8_t)(PORTBbits.RB6 == 1);
        case _RBI7: return (uint8_t)(PORTBbits.RB7 == 1);
#endif  //  #if ((defined PIC16F1828) || (defined PIC16F1829))
//
#if ((defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
        case _RCI0: return (uint8_t)(PORTCbits.RC0 == 1);
        case _RCI1: return (uint8_t)(PORTCbits.RC1 == 1);
        case _RCI2: return (uint8_t)(PORTCbits.RC2 == 1);
        case _RCI3: return (uint8_t)(PORTCbits.RC3 == 1);
        case _RCI4: return (uint8_t)(PORTCbits.RC4 == 1);
        case _RCI5: return (uint8_t)(PORTCbits.RC5 == 1);
#endif  //  #if ((defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
#if ((defined PIC16F1828) || (defined PIC16F1829))
        case _RCI6: return (uint8_t)(PORTCbits.RC6 == 1);
        case _RCI7: return (uint8_t)(PORTCbits.RC7 == 1);
#endif  //  #if ((defined PIC16F1828) || (defined PIC16F1829))
//        case _RDI0: return (uint8_t)(PORTDbits.RD0 == 1);
//        case _RDI1: return (uint8_t)(PORTDbits.RD1 == 1);
//        case _RDI2: return (uint8_t)(PORTDbits.RD2 == 1);
//        case _RDI3: return (uint8_t)(PORTDbits.RD3 == 1);
//        case _RDI4: return (uint8_t)(PORTDbits.RD4 == 1);
//        case _RDI5: return (uint8_t)(PORTDbits.RD5 == 1);
//        case _RDI6: return (uint8_t)(PORTDbits.RD6 == 1);
//        case _RDI7: return (uint8_t)(PORTDbits.RD7 == 1);

//        case _REI0: return (uint8_t)(PORTEbits.RE0 == 1);
//        case _REI1: return (uint8_t)(PORTEbits.RE1 == 1);
//        case _REI2: return (uint8_t)(PORTEbits.RE2 == 1);
//        case _REI3: return (uint8_t)(PORTEbits.RE3 == 1);
//        case _REI4: return (uint8_t)(PORTEbits.RE4 == 1);
//        case _REI5: return (uint8_t)(PORTEbits.RE5 == 1);
//        case _REI6: return (uint8_t)(PORTEbits.RE6 == 1);
//        case _REI7: return (uint8_t)(PORTEbits.RE7 == 1);
        default: return (false);
    }
}
#endif  //  #ifdef DIGITAL_INPUT_H

/* Local-scope objects -----------------------------------------------------*/
/* LEDs on the board */
#define LED_RED      (1U << 1)
#define LED_GREEN    (1U << 3)
#define LED_BLUE     (1U << 2)

/* Buttons on the board */
#define BTN_SW1      (1U << 4)
#define BTN_SW2      (1U << 0)

#if (defined DIG_OUT)
void BSP_DigOutInit(DigitalOutAssigns Indx, bool V)
{
    switch (Indx) {
#if ((defined PIC16F1822) || (defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
        case _RAO0:
            TRISAbits.TRISA0 = 0;
            ANSELAbits.ANSA0 = 0;
            break;
        case _RAO1:
            TRISAbits.TRISA1 = 0;
            ANSELAbits.ANSA1 = 0;
            break;
        case _RAO2:
            TRISAbits.TRISA2 = 0;
            ANSELAbits.ANSA2 = 0;
            break;
        case _RAO3:
            TRISAbits.TRISA3 = 0;
            break;
        case _RAO4:
            TRISAbits.TRISA4 = 0;
            ANSELAbits.ANSA4 = 0;
            break;
        case _RAO5:
            TRISAbits.TRISA5 = 0;
            break;
#endif  //  #if ((defined PIC16F1822) || (defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
#if ((defined PIC16F1828) || (defined PIC16F1829))
        case _RBO4:
            TRISBbits.TRISB4 = 0;
            ANSELBbits.ANSB4 = 0;
            break;
        case _RBO5:
            TRISBbits.TRISB5 = 0;
            ANSELBbits.ANSB5 = 0;
            break;
        case _RBO6:
            TRISBbits.TRISB6 = 0;
            break;
        case _RBO7:
            TRISBbits.TRISB7 = 0;
            break;
#endif  //  #if ((defined PIC16F1828) || (defined PIC16F1829))
#if ((defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
        case _RCO0:
            TRISCbits.TRISC0 = 0;
            ANSELCbits.ANSC0 = 0;
            break;
        case _RCO1:
            TRISCbits.TRISC1 = 0;
            ANSELCbits.ANSC1 = 0;
            break;
        case _RCO2:
            TRISCbits.TRISC2 = 0;
            ANSELCbits.ANSC2 = 0;
            break;
        case _RCO3:
            TRISCbits.TRISC3 = 0;
            ANSELCbits.ANSC3 = 0;
            break;
        case _RCO4:
            TRISCbits.TRISC4 = 0;
            break;
        case _RCO5:
            TRISCbits.TRISC5 = 0;
            break;
#endif  //  #if ((defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
#if ((defined PIC16F1828) || (defined PIC16F1829))
        case _RCO6:
            TRISCbits.TRISC6 = 0;
            ANSELCbits.ANSC6 = 0;
            SSPCONbits.SSPEN = 0;
            break;
        case _RCO7:
            TRISCbits.TRISC7 = 0;
            ANSELCbits.ANSC7 = 0;
            break;
#endif  //  #if ((defined PIC16F1828) || (defined PIC16F1829))
        default:
            break;
    }
    BSP_DigitalOutput(Indx, V);
}

void BSP_DigitalOutput   (DigitalOutAssigns Indx, bool V)
{
    switch (Indx) {
#if ((defined PIC16F1822) || (defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
        case _RAO0: LATAbits.LATA0 = V; break;
        case _RAO1: LATAbits.LATA1 = V; break;
        case _RAO2: LATAbits.LATA2 = V; break;
        case _RAO4: LATAbits.LATA4 = V; break;
        case _RAO5: LATAbits.LATA5 = V; break;
#endif  //  #if ((defined PIC16F1822) || (defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))

#if ((defined PIC16F1828) || (defined PIC16F1829))
        case _RBO4: LATBbits.LATB4 = V; break;
        case _RBO5: LATBbits.LATB5 = V; break;
        case _RBO6: LATBbits.LATB6 = V; break;
        case _RBO7: LATBbits.LATB7 = V; break;
#endif  //  #if ((defined PIC16F1828) || (defined PIC16F1829))

#if ((defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
        case _RCO0: LATCbits.LATC0 = V; break;
        case _RCO1: LATCbits.LATC1 = V; break;
        case _RCO2: LATCbits.LATC2 = V; break;
        case _RCO3: LATCbits.LATC3 = V; break;
        case _RCO4: LATCbits.LATC4 = V; break;
        case _RCO5: LATCbits.LATC5 = V; break;
#endif  //  #if ((defined PIC16F1823) || (defined PIC16F1828) || (defined PIC16F1829))
#if ((defined PIC16F1828) || (defined PIC16F1829))
        case _RCO6: LATCbits.LATC6 = V; break;
        case _RCO7: LATCbits.LATC7 = V; break;
#endif  //  #if ((defined PIC16F1828) || (defined PIC16F1829))

        default: break;
    }
}
#endif  //  #if (defined DIG_OUT)


#ifdef RELAY_H
void BSP_RelayInit(AnalogAssigns AChn)
{
    switch (AChn) {
        case _AN7:
            //  Set up RB0, RC7 as a digital output controlling the Relay
            BSP_DigOutInit(_RBO0, 0);
            BSP_DigOutInit(_RCO7, 0);
            BSP_AnalogInput_Init(_AN7);
            BSP_RelayDeselect(_AN7);
            break;
        case _AN6:
            //  Set up RD4, RC6 as a digital output controlling the Relay
            BSP_DigOutInit(_RDO4, 0);
            BSP_DigOutInit(_RCO6, 0);
            BSP_AnalogInput_Init(_AN6);
            BSP_RelayDeselect(_AN6);
            break;
    }
}

void BSP_RelayDeselect(AnalogAssigns AChn)
{
    switch (AChn) {
        case _AN7:
            PORTBbits.RB0 = 1;
            PORTCbits.RC7 = 0;
            break;
        case _AN6:
            PORTDbits.RD4 = 1;
            PORTCbits.RC6 = 0;
            break;
    }
}

void BSP_RelaySelectA(AnalogAssigns AChn)
{
    switch (AChn) {
        case _AN7:
            PORTBbits.RB0 = 0;
            PORTCbits.RC7 = 0;
            break;
        case _AN6:
            PORTDbits.RD4 = 0;
            PORTCbits.RC6 = 0;
            break;
    }
}

void BSP_RelaySelectB(AnalogAssigns AChn)
{
    switch (AChn) {
        case _AN7:
            PORTBbits.RB0 = 1;
            PORTCbits.RC7 = 1;
            break;
        case _AN6:
            PORTDbits.RD4 = 1;
            PORTCbits.RC6 = 1;
            break;
    }
}
#endif  //  #ifdef RELAY_H


#ifdef DO_PWM_H
#endif  //  #ifdef DO_PWM_H
#ifdef PWM
/**
 *  11.5.7 SETUP FOR PWM OPERATION
 *      The following steps should be taken when configuring the CCP module for
 *      PWM operation:
 *
 *  1.  Disable the PWM pin (CCPx) output drivers as an input by setting the associated
 *      TRIS bit.
 *  2.  Set the PWM period by loading the PR2 register.
 *  3.  Configure the CCP module for the PWM mode by loading the CCPxCON register
 *      with the appropriate values.
 *  4.  Set the PWM duty cycle by loading the CCPRxL register and DCxB<1:0> bits
 *      of the CCPxCON register.
 *  5.  Configure and start Timer2:
 *      ?   Clear the TMR2IF interrupt flag bit of the PIR1 register.
 *      ?   Set the Timer2 prescale value by loading the T2CKPS bits of the T2CON register.
 *      ?   Enable Timer2 by setting the TMR2ON bit of the T2CON register.
 *  6.  Enable PWM output after a new PWM cycle has started:
 *      ?   Wait until Timer2 overflows (TMR2IF bit of the PIR1 register is set).
 *      ?   Enable the CCPx pin output driver by clearing the associated TRIS bit.
 */
void BSP_PWM_Setup()
{
    /*  *   1.  Disable the PWM pin (CCPx) output drivers as an input by setting the associated
     *          TRIS bit.  The PWM output is on RC3.
     */
    TRISCbits.TRISC2 = 1;

    /**  *  2.  Set the PWM period by loading the PR2 register. */
    PR2 = 0xFF;

    /** *   3.  Configure the CCP module for the PWM mode by loading the CCPxCON register
        *       with the appropriate values.    */
    CCP1CONbits.CCP1M = 0;      //  0000 = Capture/Compare/PWM off (resets ECCP module)
    CCP1CONbits.CCP1M = 0xC;    //  1100 = PWM mode; P1A, P1C active-high; P1B, P1D active-high
    CCP1CONbits.P1M   = 0;    //  00 = Single output; P1A modulated; P1B, P1C, P1D assigned as port pins

    /** 4.  Set the PWM duty cycle by loading the CCPRxL register and DCxB<1:0> bits
        *       of the CCPxCON register.    */
    CCPR1L = 0;
    CCP1CONbits.DC1B = 0;     //  DC1B<1:0>: PWM Duty Cycle Least Significant bits

    /** *   5.  Configure and start Timer2:
        *       ?   Clear the TMR2IF interrupt flag bit of the PIR1 register.
        *       ?   Set the Timer2 prescale value by loading the T2CKPS bits of the T2CON register.
        *       ?   Enable Timer2 by setting the TMR2ON bit of the T2CON register.  */
    PIR1bits.TMR2IF  = 0;
    T2CONbits.T2CKPS = 0;     //  T2CKPS<1:0>: Timer2 Clock Prescale Select bits; 00 = Prescaler is 1
    T2CONbits.TMR2ON = 1;     //  1 = Timer2 is on

    /*  *   6.  Enable PWM output after a new PWM cycle has started:
        *       ?   Wait until Timer2 overflows (TMR2IF bit of the PIR1 register is set).
        *       ?   Enable the CCPx pin output driver by clearing the associated TRIS bit.  */
    while (!PIR1bits.TMR2IF) {;}
}


void BSP_PWM_Start(uint8_t Period, uint16_t DutyCycle_x4)
{
    /** Since we are only using the PWM to set a voltage level, it may not be required
     *  to synch the output enable with the Period rollover. */
    PR2 = Period;
    CCPR1L = (uint8_t)(DutyCycle_x4 >> 2);
    CCP1CONbits.DC1B = (uint8_t)(DutyCycle_x4 & 3);
    TRISCbits.TRISC2 = 0;   //  Enable the output
}

void BSP_PWM_Stop(void) {TRISCbits.TRISC2 = 1;}   //  Disable the output
#endif  //  #ifdef PWM


#ifdef USE_TIMER1
#if (defined PIC16F1829)
void BSP_Timer1Init(uint16_t ms)
{
    //  Set up Timer1 for 10ms tick rate (without WDT effects)
    /*  It does not look like Timer0 is able to generate a 10ms timeout without
     *  the Pre-scaler.  It may not be suitable for system tick operation.
     */
    uint8_t ps = 0U;
    if      ((uint16_t)(ms>>0) <= PS_UPDATE_CONSTANT) ps   = 0U;
    else if ((uint16_t)(ms>>1) <= PS_UPDATE_CONSTANT) ps   = 1U;
    else if ((uint16_t)(ms>>2) <= PS_UPDATE_CONSTANT) ps   = 2U;
    else if ((uint16_t)(ms>>3) <= PS_UPDATE_CONSTANT) ps   = 3U;

    //  Set up Timer1 for 10ms tick rate
    /*  TABLE 21-1: TIMER1 ENABLE SELECTIONS
     *  TMR1ON TMR1GE Timer1 Operation
     *  0       0       Off
     *  0       1       Off
     *  1       0       Always On
     *  1       1       Count Enabled   */

    /*  21.12 Timer1 Gate Control Register
     *      The Timer1 Gate Control register (T1GCON), shown in
     *      Register 21-2, is used to control Timer1 Gate.
     *
     * REGISTER 21-2: T1GCON: TIMER1 GATE CONTROL REGISTER
     * R/W-0/u  R/W-0/u R/W-0/u R/W-0/u R/W/HC-0/u  R-x/x   R/W-0/u R/W-0/u
     * TMR1GE   T1GPOL  T1GTM   T1GSPM  T1GGO/DONE  T1GVAL  T1GSS<1:0>
     * bit 7                                                        bit 0
     * Legend:
     * R = Readable bit W = Writable bit U = Unimplemented bit, read as ?0?
     * u = Bit is unchanged x = Bit is unknown -n/n = Value at POR and BOR/Value at all other Resets
     * ?1? = Bit is set ?0? = Bit is cleared HC = Bit is cleared by hardware
     *
     * bit 7 TMR1GE: Timer1 Gate Enable bit
     *  If TMR1ON = 0:
     *      This bit is ignored
     *  If TMR1ON = 1:
     *      1 = Timer1 counting is controlled by the Timer1 gate function
     *      0 = Timer1 counts regardless of Timer1 gate function    */
    T1GCONbits.TMR1GE  = 0;
    /*
     * bit 6 T1GPOL: Timer1 Gate Polarity bit
     *  1 = Timer1 gate is active-high (Timer1 counts when gate is high)
     *  0 = Timer1 gate is active-low (Timer1 counts when gate is low) */
//    T1GCONbits.T1GPOL  = 0;
    /*
     * bit 5 T1GTM: Timer1 Gate Toggle Mode bit
     *  1 = Timer1 Gate Toggle mode is enabled
     *  0 = Timer1 Gate Toggle mode is disabled and toggle flip-flop is cleared
     *      Timer1 gate flip-flop toggles on every rising edge.
     * bit 4 T1GSPM: Timer1 Gate Single-Pulse Mode bit
     *  1 = Timer1 gate Single-Pulse mode is enabled and is controlling Timer1 gate
     *  0 = Timer1 gate Single-Pulse mode is disabled
     * bit 3 T1GGO/DONE: Timer1 Gate Single-Pulse Acquisition Status bit
     *  1 = Timer1 gate single-pulse acquisition is ready, waiting for an edge
     *  0 = Timer1 gate single-pulse acquisition has completed or has not been started
     * bit 2 T1GVAL: Timer1 Gate Current State bit
     *  Indicates the current state of the Timer1 gate that could be provided to TMR1H:TMR1L.
     *  Unaffected by Timer1 Gate Enable (TMR1GE).
     * bit 1-0 T1GSS<1:0>: Timer1 Gate Source Select bits
     *  00 = Timer1 Gate pin
     *  01 = Timer0 overflow output
     *  10 = Comparator 1 optionally synchronized output (SYNCC1OUT)
     *  11 = Comparator 2 optionally synchronized output (SYNCC2OUT)      */
//    T1GCONbits.T1GSS = 1;


    /*
     * 21.11 Timer1 Control Register
     *  The Timer1 Control register (T1CON), shown in
     *  Register 21-1, is used to control Timer1 and select the
     *  various features of the Timer1 module.
     *
     * REGISTER 21-1: T1CON: TIMER1 CONTROL REGISTER
     * R/W-0/u R/W-0/u  R/W-0/u R/W-0/u R/W-0/u  R/W-0/u U-0 R/W-0/u
     * TMR1CS<1:0>     T1CKPS<1:0>     T1OSCEN  T1SYNC  ?   TMR1ON
     * bit 7                                                 bit 0
     * Legend:
     *  R = Readable bit W = Writable bit U = Unimplemented bit, read as ?0?
     *  u = Bit is unchanged x = Bit is unknown -n/n = Value at POR and BOR/Value at all other Resets
     *  ?1? = Bit is set ?0? = Bit is cleared
     *
     * bit 7-6 TMR1CS<1:0>: Timer1 Clock Source Select bits
     *  11 = Timer1 clock source is Capacitive Sensing Oscillator (CAPOSC)
     *  10 = Timer1 clock source is pin or oscillator:
     *      If T1OSCEN = 0:
     *          External clock from T1CKI pin (on the rising edge)
     *      If T1OSCEN = 1:
     *          Crystal oscillator on T1OSI/T1OSO pins
     *  01 = Timer1 clock source is system clock (FOSC)
     *  00 = Timer1 clock source is instruction clock (FOSC/4)  */
    T1CONbits.TMR1CS   = BSP_TMR1CS;    //  =00
    /*
     * bit 5-4 T1CKPS<1:0>: Timer1 Input Clock Prescale Select bits
     *  11 = 1:8 Prescale value
     *  10 = 1:4 Prescale value
     *  01 = 1:2 Prescale value
     *  00 = 1:1 Prescale value     */
    T1CONbits.T1CKPS   = ps;
    /*
     * bit 3 T1OSCEN: LP Oscillator Enable Control bit
     *  1 = Dedicated Timer1 oscillator circuit enabled
     *  0 = Dedicated Timer1 oscillator circuit disabled    */
    T1CONbits.T1OSCEN  = 0;
    /*
     * bit 2 T1SYNC: Timer1 External Clock Input Synchronization Control bit
     *  TMR1CS<1:0> = 1X:
     *      1 = Do not synchronize external clock input
     *      0 = Synchronize external clock input with system clock (FOSC)
     *  TMR1CS<1:0> = 0X:
     *      This bit is ignored.    */
    T1CONbits.nT1SYNC  = 0;
    /*
     * bit 1 Unimplemented: Read as ?0?
     * bit 0 TMR1ON: Timer1 On bit
     *  1 = Enables Timer1
     *  0 = Stops Timer1
     *      Clears Timer1 Gate flip-flop         */
    T1CONbits.TMR1ON   = 0;


    PIE1bits.TMR1GIE   = 0; //  Disable Timer 1 gate interrupt.
//    PIR1bits.TMR1GIF   = 0; //  Disable any active gate interrupt.

    TMR1_TickCnt = (uint16_t)TICK_Period_ms_DS164130_9_PP((uint16_t)(ms>>ps));
    PIR1bits.TMR1IF  = 0;   //  Reset interrupt
}
#endif  //  #if (defined PIC16F1829)


void BSP_Timer1Start(void)
{
    TMR1 = TMR1_TickCnt;    //  Load timer register based on BSP_Timer1Init calculation.
    T1CONbits.TMR1ON = 1;   //  1 = Enables Timer1
    PIR1bits.TMR1IF  = 0;   //  Reset interrupt
    PIE1bits.TMR1IE  = 1;   //  Enable Timer1 interrupt
}
#endif  //  #ifdef USE_TIMER1


void BSP_init(void)
{
    //  Set up the internal oscillator
#if (defined PIC16F1829)
    //  bit 7   SPLLEN: Software Enable bit
    //          If PLLEN in Configuration Word 1 = 1:
    //          SPLLEN bit is ignored.  4x PLL always enabled (subject to oscillator requirements))
    //
    //          If PLLEN in Configuration Word 1 = 0:
//    OSCCONbits.SPLLEN = 1   //  1 = 4xPLL is enabled
//    OSCCONbits.SPLLEN = 0   //  0 = 4xPLL is disabled

    //  bit 6-3 IRCF<3:0>: Internal Oscillator Frequency Select bits
//    OSCCONbits.IRCF = 0b0000;    //  000x = 31 kHz LF
//    OSCCONbits.IRCF = 0b0010;    //  0010 = 31.25 kHz MF
//    OSCCONbits.IRCF = 0b0011;    //  0011 = 31.25 kHz HF(1)
//    OSCCONbits.IRCF = 0b0100;    //  0100 = 62.5 kHz MF
//    OSCCONbits.IRCF = 0b0101;    //  0101 = 125 kHz MF
//    OSCCONbits.IRCF = 0b0110;    //  0110 = 250 kHz MF
//    OSCCONbits.IRCF = 0b0111;    //  0111 = 500 kHz MF (default upon Reset)
//    OSCCONbits.IRCF = 0b1000;    //  1000 = 125 kHz HF(1)
//    OSCCONbits.IRCF = 0b1001;    //  1001 = 250 kHz HF(1)
//    OSCCONbits.IRCF = 0b1010;    //  1010 = 500 kHz HF(1)
//    OSCCONbits.IRCF = 0b1011;    //  1011 = 1 MHz HF
//    OSCCONbits.IRCF = 0b1100;    //  1100 = 2 MHz HF
//    OSCCONbits.IRCF = 0b1101;    //  1101 = 4 MHz HF
    OSCCONbits.IRCF = 0b1110;    //  1110 = 8 MHz or 32 MHz HF(see Section 5.2.2.1 ?HFINTOSC?)
//    OSCCONbits.IRCF = 0b1111;    //  1111 = 16 MHz HF

    //  bit 1-0 SCS<1:0>: System Clock Select bits
//    OSCCONbits.SCS = 2; //  1x = Internal oscillator block
//    OSCCONbits.SCS = 1; //  01 = Timer 1 oscillator
//    OSCCONbits.SCS = 0; //  00 = Clock determined by FOSC<2:0> in Configuration Word 1
#endif  //  #if (defined PIC16F1829)

#ifdef USE_INTERRUPTS
    BSP_DisableInterrupts();
    INTCONbits.PEIE = 0;    //  Disable peripheral interrupt
#endif  //  #ifdef USE_INTERRUPTS

//    for (AnalogAssigns Idx=_AN0; Idx<_AN8; Idx++)
//    {
//        BSP_AnalogInput_Init(Idx);
//    }

    //  Digital Input pin initialization is done in the DigitalInputs module.
//    //  Set up RB<1,2,4,5> as a digital input port
//    for (uint8_t Idx=_RB1; Idx<_RB6; Idx++)
//    {
//        BSP_DigitalInputsInit(Idx);
//    }
//    // I2B = PORTBbits.RB1;    //  Read port bit.
//    //  Debounce the digital input

//    BSP_RelayInit(_AN7);
//    BSP_RelayInit(_AN6);
    //PORTBbits.RB0 = RelayB; //  Write port bit.

#ifdef USE_TIMER1
    BSP_Timer1Init(MS_PER_TICK);         // Default 20ms period, can be overridden in start up.
#endif //   #ifdef USE_TIMER1

//    BSP_LEDInit();
//    BSP_BuzzerInit();

#ifdef USE_INTERRUPTS
    INTCONbits.PEIE = 1;    //  Enable Peripheral interrupt
#endif  //    #ifdef USE_INTERRUPTS

    {   // Initialize LEDs for output & turn off.
        DO_Assigns Indx;
        for (uint8_t i=MIN_DOT; i<N_DOT_OUTPUTS; ++i) {
            switch (i) {
                case DOT_DS1Buz: Indx = DS1; break;
                case DOT_DS2Red: Indx = DS2; break;
                case DOT_DS3Blu: Indx = DS3; break;
                case DOT_DS4Grn: Indx = DS4; break;
            }
            BSP_DigOutInit((DigitalOutAssigns)Indx, false);
        }
    }
    {   // Initialize switch and potentiometer as TTL-level, digital inputs
        DigitalInAssigns Indx;
        for (uint8_t i=MIN_DI; i<N_DIGITAL_INPUTS; ++i) {
            switch (i) {
                case DI_SW1: Indx = _DI_SW1; break;
                case DI_RP1: Indx = _DI_RP1; break;
            }
            BSP_DigitalInputsInit(Indx);
        }
    }
}


/*..........................................................................*/
void QV_onIdle(void) {
#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M MCU.
    */
    QV_CPU_SLEEP();  /* atomically go to sleep and enable interrupts */
#else
    QF_INT_ENABLE(); /* just enable interrupts */
#endif
}

#ifdef USE_UART
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    volatile int8_t dummy;
    QF_INT_DISABLE(); /* make sure that interrupts are disabled */
#ifdef USE_PRINTF
    printf("%s,%i", file, line);
#endif  //  #ifdef USE_PRINTF
    while (1) {
        for(DO_Assigns i=MIN_DM164130_9_DO; i<N_DM164130_9_DO_OUTPUTS; i++) {
//            BSP_DigitalOutputSet(((DigOut *)AO_DigOut[i])->DOO_Assignment);
            BSP_DigitalOutputSet((DigitalOutAssigns)DS1);
            BSP_DigitalOutputSet((DigitalOutAssigns)DS2);
            BSP_DigitalOutputSet((DigitalOutAssigns)DS3);
            BSP_DigitalOutputSet((DigitalOutAssigns)DS4);
        }
        dummy = (file, line, 1);
        (void)file;
        (void)line;
    }
}


void UART_SetUp(void) {
    TXSTAbits.SYNC = 0;
    SPBRG = 29;
    SPBRGH = 0;
    TXSTAbits.BRGH = 0;
#if (defined PIC16F1829)
    BAUDCONbits.BRG16 = 0;
    BSP_DigOutInit(_RBO5, true);
    BSP_DigOutInit(_RBO7, true);
#endif  //  #if (defined PIC16F1829)

    TXSTAbits.TX9 = 0;
    PIE1bits.TXIE = 0;
}
void UART_Start(void) {
    RCSTAbits.SPEN = 1;
    TXSTAbits.SYNC = 0;
    TXSTAbits.TX9  = 0;
    PIE1bits.TXIE  = 0;     // NOTE: Interrupt processing is not enabled for the TX
    TXSTAbits.TXEN = 1;
}
void UART_Stop(void) {
    RCSTAbits.SPEN = 0;
    TXSTAbits.SYNC = 1;
    TXSTAbits.TX9  = 0;
    PIE1bits.TXIE  = 0;
    TXSTAbits.TXEN = 0;
}

void BSP_display(char const *msg) {
#ifdef USE_PRINTF
    (void) printf(msg);
#endif  //  #ifdef USE_PRINTF
}

void putch(char c) {
    while( ! TXSTAbits.TRMT) // check buffer
        continue; // wait till ready
    TXREG = c; // send data
}
#else
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {}
void UART_SetUp(void) {}
void UART_Start(void) {}
void UART_Stop(void) {}
void BSP_display(char const *msg) {}
void putch(char c) {}

#endif  //  #ifdef USE_UART

/*..........................................................................*/
void QF_onStartup(void) {
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate
    * NOTE: do NOT call OS_CPU_SysTickInit() from uC/OS-II
    */
//    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    /* set priorities of ALL ISRs used in the system, see NOTE1 */
//    NVIC_SetPriority(SysTick_IRQn,  QF_AWARE_ISR_CMSIS_PRI + 1U);
    /* ... */

    /* enable IRQs in the NVIC... */
    /* ... */


    //  Set up the UART for 9600 baud, 8 data bits.
    // UART_SetUp();
    // UART_Start();

    #ifdef USE_TIMER1
        BSP_Timer1Init((MS_PER_S)/(BSP_TICKS_PER_SEC));    //  Set up Timer1 for 20ms tick rate
        BSP_Timer1Start();
    #endif //   #ifdef USE_TIMER1

    BSP_EnableInterrupts();

        //  Start the test.
    #ifdef USE_PRINTF
        (void)printf("Start of program.\n");
    #endif  //  #ifdef USE_PRINTF

}
/*..........................................................................*/
void QF_onCleanup(void) {
}

/*..........................................................................*/
void BSP_ledRedOn(void) {
    BSP_DigitalOutput((DigitalOutAssigns)DS2Red, true);

}

/*..........................................................................*/
void BSP_ledRedOff(void) {
    BSP_DigitalOutput((DigitalOutAssigns)DS2Red, false);
}
/*..........................................................................*/
void BSP_ledBlueOn(void) {
    BSP_DigitalOutput((DigitalOutAssigns)DS3Blu, true);
}

/*..........................................................................*/
void BSP_ledBlueOff(void) {
    BSP_DigitalOutput((DigitalOutAssigns)DS3Blu, false);
}

/*..........................................................................*/
void BSP_ledGreenOn(void) {
    BSP_DigitalOutput((DigitalOutAssigns)DS4Grn, true);
}

/*..........................................................................*/
void BSP_ledGreenOff(void) {
    BSP_DigitalOutput((DigitalOutAssigns)DS4Grn, false);
}

