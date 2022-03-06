![QP-nano framework](https://www.state-machine.com/img/qpn_banner.jpg)

> **NOTE:** If your company has a policy forbidding open source in your
product, all QP frameworks can be
[licensed commercially](https://www.state-machine.com/licensing),
in which case you don't use any open source license and you do not violate
your policy.


---------------------------------------------------------------------------
# What's New?

**NOTE** QP-nano has been **discontinued** from active development
and support and is **not recommended** for new designs. This QP-nano
repository is preserved for the existing user base.

[QPN-PIC16] is an adaptation of the [QP-nano] framework to the Microchip PIC16
architecture as compiled by the MPALB-X IDE using the XC8 compiler (C90/C99).
It allows [QP-nano] models developed using the QM modeling tool to be integrated
with the QV-nano kernel to build [active objects][Active] applications.

---------------------------------------------------------------------------
# Documentation
The offline HTML documentation for **this** particular version of QP-nano
is located in the folder html/. To view the offline documentation, open
the file html/index.html in your web browser.

The online HTML documention for the **latest** version of QP-nano is located
at: https://www.state-machine.com/qpn/


---------------------------------------------------------------------------
# About QP-nano
QP-nano (Quantum Platform Nano) is an ultra-lightweight, open source
[Real-Time Embedded Framework (RTEF)][RTEF] for building modern embedded
software as systems of asynchronous, event-driven [active objects][Active]
(actors). The [QP-nano] framework is a member of a larger [QP] family
consisting of [QP/C], [QP/C++], and [QP-nano] frameworks, which are all
strictly quality controlled, thoroughly documented, and [commercially
licensable][Lic].

## Safer Model of Concurrency
The [QP] framework family is based on the [Active Object][Active] (**actor**)
design pattern, which inherently supports and automatically enforces the
following best practices of concurrent programming:

- Keep data isolated and bound to active objects' threads. Threads should
hide (**encapsulate**) their private data and other resources, and not
share them with the rest of the system.

- Communicate among active object threads **asynchronously** via event
objects. Using asynchronous events keeps the threads running truly
independently, **without blocking** on each other.

- Active object threads should spend their lifetime responding to incoming
events, so their mainline should consist of an **event-loop** that handles
events one at a time (to completion), thus avoiding any concurrency hazards
within an active object thread itself.

This architecture is generally **safer**, more responsive and easier to
understand and maintain than the shared-state concurrency of a conventional
RTOS. It also provides higher level of abstraction and the *correct*
abstractions to effectively apply **modeling** and **code generation** to
deeply embedded real-time systems.

## Hierarchical State Machines
The behavior of active objects is specified in QP-nano by means of
[Hierarchical State Machines][HSM] (UML statecharts). The framework
supports manual coding of UML state machines in C as well as automatic
**code generation** by means of the free [QM modeling tool][QM].

## Built-in Real-Time Kernels
The QP-nano framework can run on bare-metal single-chip microcontrollers,
completely replacing a traditional "superloop" or an RTOS. The framework
contains a selection of **built-in real-time kernels**, such as the
cooperative QV-nano kernel and the preemptive non-blocking QK-nano kernel.
Native QP-nano ports and ready-to-use examples are provided for such CPUs
PIC16, MSP430, AVRmega, and ARM Cortex-M (M0/M0+/M3/M4).

## Maturity
With 60,000 downloads a year, the [QP] family is the most popular such
solution on the embedded software market. It provides a modern, reusable
architecture for embedded applications, which combines the active-object
model of concurrency with hierarchical state machines.

---------------------------------------------------------------------------
# Getting Started with QP-nano
The [QP-nano Reference Manual](https://www.state-machine.com/qpn/) provides
instructions on how to download, install, and get started with QP-nano quickly.

The [AppNote: "Getting Started with QP-nano"][AN] contains also a tutorial,
in which you build a simple "Blinky" application.


---------------------------------------------------------------------------
# QP-nano Licensing
QP-nano is licensed under the increasingly popular [dual licensing model][Lic],
in which both the open source software distribution mechanism and
traditional closed source software distribution models are combined.

> **NOTE:** If your company has a policy forbidding open source in your
product, all QP frameworks can be [licensed commercially][Lic], in which case
you don't use any open source license and you do not violate your policy.

---------------------------------------------------------------------------
# QP-nano Documentation
The **QP-nano Manual** is located online at: https://www.state-machine.com/qpn

---------------------------------------------------------------------------
# [QPN-PIC16](https://github.com/aschatte/qpn)
[QPN-PIC16] is an adaptation of the QP-nano framework to the Microchip PIC16
architecture as compiled by the MPALB-X IDE using the XC8 compiler (C90/C99).
It allows QP-nano models developed using the QM modeling tool to be integrated
with the QV-nano kernel to build [Active Object] applications.  The very 
limited resources of the PIC16 family of MCUs, primarily the hardware stack,
required a special version of QP-nano and a QM-Modeler editing post-processor, 
QM2HSM.exe, to effect.  

## [QPN-PIC16] Implementation
The [QPN-PIC16] firmware was developed directly from the QP-nano firmware with 
changes implemented as needed to allow the MPLAB-X IDE using the XC8 (C90/C99)
compiler to build working firmware in the target PIC16 MCU.  Complications
were encountered prilarily in pointers to compound function arguments and 
structures.  Application development should be the same as any other QP-nano
project since the same API was maintained with the exception that some pointer
arguments are passed as void pointers.  Additionally, some macros accessing 
virtual table functions had to be implemented differently and require modified 
return value processing (cf., QACTIVE_POST, QACTIVE_POST_ISR).  The global 
variable QACTIVE_POST_res_ is provided for this return value and is useful for
the QACTIVE_POST_X macros where any value but QF_NO_MARGIN is specified for a 
posting margin.

## [QPN-PIC16] Configuration
[QPN-PIC16] is configured the same as a normal QP-nano application with several
additions to the qpn_conf.h file to control the build configuration and include
MPLAB-X, XC8 header files.

1.  The [QPN-PIC16] build is controlled by three preprocessor switches, one of 
which **must** be defined:

	- XPRJ_Hsm: A minimalist implementation completely replacing the QEPN state
	machine implementation, but keeping some parts of the QACTIVE framework.

	- XPRJ_QPN_NoVtables: the full QEPN state-machine and QACTIVE framework but
	with fixed functions for init, dispatch, post, and postISR

	- XPRJ_QPN_Vtables: the full QEP state-machine and QACTIVE framework, including
	the virtual handler tables for init, dispatch, post, and postISR.  The static
	and hidden nature of these tables was compromised to a global, visible set of
	tables for the XC8 compiler.

	> **NOTE:** The TimeBomb example has MPLAB-X set up with these build 
	configurations selectable using the IDE which automatically defines the 
	appropriate macro.  In lieu of this, the desired macro may be set in the 
	qpn_conf.h file.

2.  MPLAB-X XC compilers require that the xc.h header file be included in all 
source files.  This is done directly at the top of the qpn_conf.h file.

## QM-Modeler Integration
[QPN-PIC16] models are developed using QM-Modeler exactly as they are for any 
other QP-nano application.  The final step, "Generate Code" is handled by
an External Tool, QM2HSM.exe, instead of the "Generate Code" tool button.
The QM2HSM.exe is installed as an External Tool in the QM-Modeler and it is 
used instead of the "Generate Code" tool button to generate code from the model 
targetted to the MPLAB-X, XC8 (C90/C99) compiler for the Microchip PIC family 
of MCUs.  The "Generate Code" button can still be used to generate QM model 
code for other target MCUs from the same model, if desired.  Please refer to
the \examples\pic16\timebomb_dm164130-9\README.txt for more information on how
to set up and use the QM-Modeler tools to generate [QPN-PIC16] files. The example, 
\examples\pic16\timebomb_dm164130-9\TimeBomb.qm, is provided as a working 
demonstration of the tools, development workflow, and operating result for the
Microchip DM164130-9 Demo Board with the PIC161829 MCU.

## [QPN-PIC16] Footprint
The TimeBomb example application was tested for the XC8, PIC16F1829, and the 
specified configurations:

### c90:
define QF_MAX_TICK_RATE    1<br>
define QF_TIMEEVT_CTR_SIZE 2<br>
undef QF_TIMEEVT_PERIODIC<br>
undef QF_TIMEEVT_USAGE<br>
define QF_MARGIN           0   // 0xFF- QF_NO_MARGIN, 0- no margin function calls<br>
undef USE_FULL_TEST            // undef- no extra test code<br>
|      Configuration         |   Program Space  |    Data Space   | 
| -------------------------- | ---------------- | --------------- | 
| QPN/Q_PARAM_SIZE=0/Vtable  | 4445/8192 (54.3%)| 143/1024 (14.0%)| 
| QPN/Q_PARAM_SIZE=0/NoVtable| 4392/8192 (53.6%)| 131/1024 (12.8%)| 
| Hsm/Q_PARAM_SIZE=0/X       | 4239/8192 (51.7%)| 126/1024 (12.3%)| 
| QPN/Q_PARAM_SIZE=2/Vtable  | 4610/8192 (56.3%)| 169/1024 (16.5%)| 
| QPN/Q_PARAM_SIZE=2/NoVtable| 4657/8192 (56.8%)| 157/1024 (15.3%)| 
| Hsm/Q_PARAM_SIZE=2/X       | 4275/8192 (52.2%)| 168/1024 (16.4%)| 
| QPN/Q_PARAM_SIZE=4/Vtable  | 4703/8192 (57.4%)| 197/1024 (19.2%)| 
| QPN/Q_PARAM_SIZE=4/NoVtable| 4618/8192 (56.4%)| 185/1024 (18.1%)| 
| Hsm/Q_PARAM_SIZE=4/X       | 4314/8192 (52.7%)| 212/1024 (20.7%)| 

### c99:
define QF_MAX_TICK_RATE    1<br>
define QF_TIMEEVT_CTR_SIZE 2<br>
undef QF_TIMEEVT_PERIODIC<br>
undef QF_TIMEEVT_USAGE<br>
define QF_MARGIN           0   // 0xFF- QF_NO_MARGIN, 0- no margin function calls<br>
undef USE_FULL_TEST            // undef- no extra test code<br>
|      Configuration         |   Program Space  |    Data Space   | 
| -------------------------- | ---------------- | --------------- | 
| QPN/Q_PARAM_SIZE=0/Vtable  | 4554/8192 (55.7%)| 145/1024 (14.2%)| 
| QPN/Q_PARAM_SIZE=0/NoVtable| 4554/8192 (55.6%)| 134/1024 (13.1%)| 
| Hsm/Q_PARAM_SIZE=0/X       | 4290/8192 (52.4%)| 128/1024 (12.5%)| 
| QPN/Q_PARAM_SIZE=2/Vtable  | 4720/8192 (57.6%)| 171/1024 (16.7%)| 
| QPN/Q_PARAM_SIZE=2/NoVtable| 4533/8192 (55.3%)| 159/1024 (15.5%)| 
| Hsm/Q_PARAM_SIZE=2/X       | 4340/8192 (53.0%)| 170/1024 (16.6%)| 
| QPN/Q_PARAM_SIZE=4/Vtable  | 4752/8192 (58.0%)| 199/1024 (19.4%)| 
| QPN/Q_PARAM_SIZE=4/NoVtable| 4545/8192 (55.5%)| 187/1024 (18.3%)| 
| Hsm/Q_PARAM_SIZE=4/X       | 4424/8192 (54.0%)| 214/1024 (20.9%)|

These measurements indicate that the TimeBomb application would fit into a 
PIC16F88<6,7>, also.

## [QPN-PIC16] Getting Started
Following the enbedded software dictum: "nothing works until everything works",
it is recommended that the TimeBomb project for the Microchip DM164130-9 deno
board using the PIC16F1829 MCU be used as a starting point.  The <br>

examples\pic16\timebomb_dm164130-9\TimeBomb.qm<br>

QM-model may be opened in QM-Modeler and the main.c file built by pressing
the First External Tool button.  This should return something similar to:

```
{{{ External tool "QModeler to PIC16 HSM conversion"
INFO> Code generation started (11:48:6 am)
INFO> Entire model: C:\Projects\qpn-PICnano\examples\pic16\timebomb_dm164130-9\TimeBomb.qm
INFO> Code generation ended (time elapsed 0.003s)
INFO> 0 file(s) generated, 1 file(s) processed, 0 error(s) and 0 warning(s)

..\..\..\qtools\bin\QM2HSM.exe ${ModelFile}

..\..\..\qtools\bin\QM2HSM.exe ${ModelFile}
Processing QM Model file:#<path:C:\Projects\qpn-PICnano\examples\pic16\timebomb_dm164130-9\TimeBomb.qm>.
	Processing model file path: #<path:C:\Projects\qpn-PICnano\examples\pic16\timebomb_dm164130-9\.\main.c>
cpu time: 15 real time: 21 gc time: 0

}}} External tool finished normally with status 0
```

MPLAB-X IDE may now be opened and using the "Open Project" button the MPLAB-X
project opened by navigating to the c90 or c99 folder of the 
examples\pic16\timebomb_dm164130-9\qv\xc8 folder and selecting the TimeBomb.X
project.  This should open the TimeBomb project.  After the project sets up
the "Open and Build Project" button can be used to compile the project, and 
the "Run Project" button used to program the PIC16F1829 MCU on the DM164130-9
demo board using a programmer/debugger like the PICkit 3.  The TimeBomb can now
be tested for proper operation on the DM164130-9 demo board using "SW1" as 
"Button 1" and "RP1" as "Button 2".  

> **NOTE:**  "Button 2" is implemented by the potentiometer, RP1, entering
the MCU as a digital input.  It may take dome experimentation to get it to work
correctly.  When RP1 is rotated counter-clockwise the voltage level at the 
MCU pin decreases, and when RP1 is rotated clockwise, the voltage at the MCU
pin increases.  There is a threshold where the sensed voltage level is low 
changing to high as RP1 is rotated clockwise.  The low level represents the 
"PRESSED" state, and the high level represents the "RELEASED" state of the 
"button".  

Succinctly, 
```
Counter-clockwise => PRESSED
Clockwise         => RELEASED
```
Once it is verified that the program works correctly, the timebomb_dm164130-9
project may be copied and changed to implement the new functionality of your
project.  If the folder structure/source file relationship changes, it will be
necessary to ensure that all the appropriate source/header files aree properly
referenced by the MPLAB-X IDE for the new project location.  A good maxim is to
make small, incremental changes and check your work frequently.

## [QPN-PIC16] Licensing
[QPN-PIC16] is licensed in the same way as QP-nano, above.

## [QPN-PIC16] Documentation
The **QP-nano Manual** is located online at: https://www.state-machine.com/qpn
and can be used for [QPN-PIC16].

---------------------------------------------------------------------------
# How to get help?
- [Free Support Forum](https://sourceforge.net/p/qpc/discussion/668726)
- [Bug Reports](https://sourceforge.net/p/qpc/bugs/)
- [Feature Requests](https://sourceforge.net/p/qpc/feature-requests/)
- [Quantum Leaps website](https://www.state-machine.com)
- [Quantum Leaps licensing](https://www.state-machine.com/licensing)
- [info@state-machine.com](mailto:info@state-machine.com)

   [RTEF]: <https://www.state-machine.com/doc/concepts#RTEF>
   [QP]: <https://www.state-machine.com/products/#QP>
   [QP/C]: <https://www.state-machine.com/qpc>
   [QP/C++]: <https://www.state-machine.com/qpcpp>
   [QP-nano]: <https://www.state-machine.com/qpn>
   [QM]: <https://www.state-machine.com/qm>
   [Active]: <https://www.state-machine.com/doc/concepts#Active>
   [HSM]: <https://www.state-machine.com/doc/concepts#HSM>
   [Lic]: <https://www.state-machine.com/licensing>
   [AN]: <https://www.state-machine.com/doc/AN_Getting_Started_with_QP-nano.pdf>
   [QPN-PIC16]: <https://github.com/aschatte/qpn)>
   