#Edit this file for the executing environment

#XC8 compiler command or path to binary
CC=xc8
#XC32 compiler command or path to binary
CC32=xc32
#Command for making directories
MKDIR=mkdir -p
#Command for removing directories and files
RM=rm -rf
#Command for navigating directories
CD=cd

#The following paths will be under each project subdirectory
#Intermediate build location
OBJ_DIR=build/manual
#Final destination of the HEX output
DIST_DIR=dist/manual

#Compiler options, shouldn't have to be editted unless using Standard or Pro license
MODE=free
WARN=0
OPT=+asm,+asmfile,-speed,+space,-debug
