#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/b0c5a78b/common.o \
	${OBJECTDIR}/_ext/7d1870e3/pins.o \
	${OBJECTDIR}/_ext/f8b2fa93/uart.o \
	${OBJECTDIR}/_ext/5c0/aquaPicBus.o \
	${OBJECTDIR}/testAquaPicBus.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/b0c5a78b/common.o: ../../common/common.c
	${MKDIR} -p ${OBJECTDIR}/_ext/b0c5a78b
	${RM} "$@.d"
	$(COMPILE.c) -g -DTEST -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/b0c5a78b/common.o ../../common/common.c

${OBJECTDIR}/_ext/7d1870e3/pins.o: ../../pins/test/pins.c
	${MKDIR} -p ${OBJECTDIR}/_ext/7d1870e3
	${RM} "$@.d"
	$(COMPILE.c) -g -DTEST -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/7d1870e3/pins.o ../../pins/test/pins.c

${OBJECTDIR}/_ext/f8b2fa93/uart.o: ../../uart/test/uart.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f8b2fa93
	${RM} "$@.d"
	$(COMPILE.c) -g -DTEST -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f8b2fa93/uart.o ../../uart/test/uart.c

${OBJECTDIR}/_ext/5c0/aquaPicBus.o: ../aquaPicBus.c
	${MKDIR} -p ${OBJECTDIR}/_ext/5c0
	${RM} "$@.d"
	$(COMPILE.c) -g -DTEST -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/5c0/aquaPicBus.o ../aquaPicBus.c

${OBJECTDIR}/testAquaPicBus.o: testAquaPicBus.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DTEST -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/testAquaPicBus.o testAquaPicBus.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
