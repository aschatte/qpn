About this Example
==================
This directory contains the simple "Timebomb" QP-[micro] example application for
the EK-TM4C123GXL board (TivaC LauchPad). This directory contains portable
code that should compile with any C compiler.

The Timebomb application is developed from the QM-Modeler file, TimeBomb.qm.  It is opened in the QM-Modeler which is then used to "Generate Code", main_qpm.c.  The remaining files comprise the ek-tm4c123gxl board supportpackage (bsp.h, bsp_qpm.c) and the QPN porting files (qf_port.h, qpn_conf.h).  these file are used by the Arm-Keil project to build and program the applicationinto the target device on the development board.The sub-directories contain the Board Support Packages (BSPs) for the
cooperative QV kernel (sub-directory qv).

The native kernel (QV-[micro]) further provide code and project
files for the particular ARM toolchains, such as ARM (MDK-ARM), GNU, and IAR.

Please refer to the README files in the sub-directories for instructions
how to use and customize the example to your needs.


Support Code for EK-TM4C123GXL Board 
====================================
The directory qpn\3rd_party\ek-tm4c123gxl contains the CMSIS-compliant
device code for the TM4C123GH6PM MCU. Please see the README.txt file in
this folder for more details.

