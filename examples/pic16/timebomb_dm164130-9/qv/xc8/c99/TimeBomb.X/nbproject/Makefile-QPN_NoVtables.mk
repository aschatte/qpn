#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-QPN_NoVtables.mk)" "nbproject/Makefile-local-QPN_NoVtables.mk"
include nbproject/Makefile-local-QPN_NoVtables.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=QPN_NoVtables
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/TimeBomb.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/TimeBomb.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../../DM164130-9/bsp.c ../../../../../../../src/qfn/qepn.c ../../../../../../../src/qfn/qfn.c ../../../../../../../src/qvn/qvn.c ../../../../main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1487629836/bsp.p1 ${OBJECTDIR}/_ext/2105057759/qepn.p1 ${OBJECTDIR}/_ext/2105057759/qfn.p1 ${OBJECTDIR}/_ext/2105058255/qvn.p1 ${OBJECTDIR}/_ext/1568745167/main.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1487629836/bsp.p1.d ${OBJECTDIR}/_ext/2105057759/qepn.p1.d ${OBJECTDIR}/_ext/2105057759/qfn.p1.d ${OBJECTDIR}/_ext/2105058255/qvn.p1.d ${OBJECTDIR}/_ext/1568745167/main.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1487629836/bsp.p1 ${OBJECTDIR}/_ext/2105057759/qepn.p1 ${OBJECTDIR}/_ext/2105057759/qfn.p1 ${OBJECTDIR}/_ext/2105058255/qvn.p1 ${OBJECTDIR}/_ext/1568745167/main.p1

# Source Files
SOURCEFILES=../../../../DM164130-9/bsp.c ../../../../../../../src/qfn/qepn.c ../../../../../../../src/qfn/qfn.c ../../../../../../../src/qvn/qvn.c ../../../../main.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-QPN_NoVtables.mk dist/${CND_CONF}/${IMAGE_TYPE}/TimeBomb.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=16F1829
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1487629836/bsp.p1: ../../../../DM164130-9/bsp.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1487629836" 
	@${RM} ${OBJECTDIR}/_ext/1487629836/bsp.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1487629836/bsp.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -DDM164130_9 -xassembler-with-cpp -I"../../../../" -I"../../../../DM164130-9" -I"../../../../../../../include" -mwarn=-3 -Wa,-a -DXPRJ_QPN_NoVtables=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mdownload -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1487629836/bsp.p1 ../../../../DM164130-9/bsp.c 
	@-${MV} ${OBJECTDIR}/_ext/1487629836/bsp.d ${OBJECTDIR}/_ext/1487629836/bsp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1487629836/bsp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2105057759/qepn.p1: ../../../../../../../src/qfn/qepn.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2105057759" 
	@${RM} ${OBJECTDIR}/_ext/2105057759/qepn.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2105057759/qepn.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -DDM164130_9 -xassembler-with-cpp -I"../../../../" -I"../../../../DM164130-9" -I"../../../../../../../include" -mwarn=-3 -Wa,-a -DXPRJ_QPN_NoVtables=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mdownload -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/2105057759/qepn.p1 ../../../../../../../src/qfn/qepn.c 
	@-${MV} ${OBJECTDIR}/_ext/2105057759/qepn.d ${OBJECTDIR}/_ext/2105057759/qepn.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2105057759/qepn.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2105057759/qfn.p1: ../../../../../../../src/qfn/qfn.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2105057759" 
	@${RM} ${OBJECTDIR}/_ext/2105057759/qfn.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2105057759/qfn.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -DDM164130_9 -xassembler-with-cpp -I"../../../../" -I"../../../../DM164130-9" -I"../../../../../../../include" -mwarn=-3 -Wa,-a -DXPRJ_QPN_NoVtables=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mdownload -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/2105057759/qfn.p1 ../../../../../../../src/qfn/qfn.c 
	@-${MV} ${OBJECTDIR}/_ext/2105057759/qfn.d ${OBJECTDIR}/_ext/2105057759/qfn.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2105057759/qfn.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2105058255/qvn.p1: ../../../../../../../src/qvn/qvn.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2105058255" 
	@${RM} ${OBJECTDIR}/_ext/2105058255/qvn.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2105058255/qvn.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -DDM164130_9 -xassembler-with-cpp -I"../../../../" -I"../../../../DM164130-9" -I"../../../../../../../include" -mwarn=-3 -Wa,-a -DXPRJ_QPN_NoVtables=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mdownload -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/2105058255/qvn.p1 ../../../../../../../src/qvn/qvn.c 
	@-${MV} ${OBJECTDIR}/_ext/2105058255/qvn.d ${OBJECTDIR}/_ext/2105058255/qvn.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2105058255/qvn.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1568745167/main.p1: ../../../../main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1568745167" 
	@${RM} ${OBJECTDIR}/_ext/1568745167/main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1568745167/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -DDM164130_9 -xassembler-with-cpp -I"../../../../" -I"../../../../DM164130-9" -I"../../../../../../../include" -mwarn=-3 -Wa,-a -DXPRJ_QPN_NoVtables=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mdownload -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1568745167/main.p1 ../../../../main.c 
	@-${MV} ${OBJECTDIR}/_ext/1568745167/main.d ${OBJECTDIR}/_ext/1568745167/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1568745167/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1487629836/bsp.p1: ../../../../DM164130-9/bsp.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1487629836" 
	@${RM} ${OBJECTDIR}/_ext/1487629836/bsp.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1487629836/bsp.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -DDM164130_9 -xassembler-with-cpp -I"../../../../" -I"../../../../DM164130-9" -I"../../../../../../../include" -mwarn=-3 -Wa,-a -DXPRJ_QPN_NoVtables=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mdownload -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1487629836/bsp.p1 ../../../../DM164130-9/bsp.c 
	@-${MV} ${OBJECTDIR}/_ext/1487629836/bsp.d ${OBJECTDIR}/_ext/1487629836/bsp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1487629836/bsp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2105057759/qepn.p1: ../../../../../../../src/qfn/qepn.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2105057759" 
	@${RM} ${OBJECTDIR}/_ext/2105057759/qepn.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2105057759/qepn.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -DDM164130_9 -xassembler-with-cpp -I"../../../../" -I"../../../../DM164130-9" -I"../../../../../../../include" -mwarn=-3 -Wa,-a -DXPRJ_QPN_NoVtables=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mdownload -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/2105057759/qepn.p1 ../../../../../../../src/qfn/qepn.c 
	@-${MV} ${OBJECTDIR}/_ext/2105057759/qepn.d ${OBJECTDIR}/_ext/2105057759/qepn.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2105057759/qepn.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2105057759/qfn.p1: ../../../../../../../src/qfn/qfn.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2105057759" 
	@${RM} ${OBJECTDIR}/_ext/2105057759/qfn.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2105057759/qfn.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -DDM164130_9 -xassembler-with-cpp -I"../../../../" -I"../../../../DM164130-9" -I"../../../../../../../include" -mwarn=-3 -Wa,-a -DXPRJ_QPN_NoVtables=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mdownload -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/2105057759/qfn.p1 ../../../../../../../src/qfn/qfn.c 
	@-${MV} ${OBJECTDIR}/_ext/2105057759/qfn.d ${OBJECTDIR}/_ext/2105057759/qfn.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2105057759/qfn.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2105058255/qvn.p1: ../../../../../../../src/qvn/qvn.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2105058255" 
	@${RM} ${OBJECTDIR}/_ext/2105058255/qvn.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2105058255/qvn.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -DDM164130_9 -xassembler-with-cpp -I"../../../../" -I"../../../../DM164130-9" -I"../../../../../../../include" -mwarn=-3 -Wa,-a -DXPRJ_QPN_NoVtables=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mdownload -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/2105058255/qvn.p1 ../../../../../../../src/qvn/qvn.c 
	@-${MV} ${OBJECTDIR}/_ext/2105058255/qvn.d ${OBJECTDIR}/_ext/2105058255/qvn.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2105058255/qvn.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1568745167/main.p1: ../../../../main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1568745167" 
	@${RM} ${OBJECTDIR}/_ext/1568745167/main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1568745167/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -DDM164130_9 -xassembler-with-cpp -I"../../../../" -I"../../../../DM164130-9" -I"../../../../../../../include" -mwarn=-3 -Wa,-a -DXPRJ_QPN_NoVtables=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mdownload -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1568745167/main.p1 ../../../../main.c 
	@-${MV} ${OBJECTDIR}/_ext/1568745167/main.d ${OBJECTDIR}/_ext/1568745167/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1568745167/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/TimeBomb.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/TimeBomb.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_QPN_NoVtables=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -DDM164130_9 -xassembler-with-cpp -I"../../../../" -I"../../../../DM164130-9" -I"../../../../../../../include" -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mdownload -mno-stackcall -std=c99 -gdwarf-3 -mstack=compiled:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/TimeBomb.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/TimeBomb.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/TimeBomb.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/TimeBomb.X.${IMAGE_TYPE}.map  -DXPRJ_QPN_NoVtables=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -DDM164130_9 -xassembler-with-cpp -I"../../../../" -I"../../../../DM164130-9" -I"../../../../../../../include" -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mdownload -mno-stackcall -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/TimeBomb.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
	@echo Normalizing hex file
	@"C:/Program Files/Microchip/MPLABX/v5.50/mplab_platform/platform/../mplab_ide/modules/../../bin/hexmate" --edf="C:/Program Files/Microchip/MPLABX/v5.50/mplab_platform/platform/../mplab_ide/modules/../../dat/en_msgs.txt" dist/${CND_CONF}/${IMAGE_TYPE}/TimeBomb.X.${IMAGE_TYPE}.hex -odist/${CND_CONF}/${IMAGE_TYPE}/TimeBomb.X.${IMAGE_TYPE}.hex

endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/QPN_NoVtables
	${RM} -r dist/QPN_NoVtables

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
