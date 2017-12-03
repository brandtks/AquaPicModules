#################################
###Shouldn't have to edit this###
#################################
include ../../Makefile-defines.mk

MPROC=32MM0064GPM028
OBJS=$(OBJ_DIR)/AnalogOutputCard/main.o $(OBJ_DIR)/aquaPicBus/aquaPicBus.o $(OBJ_DIR)/nvm/nvm.o
PROJ_DEPS=../../lib/aquaPicBus/aquaPicBus.h ../../lib/PIC32MM/nvm/nvm.h

$(DIST_DIR)/AnalogOutputCard.hex : $(DIST_DIR)/AnalogOutputCard.elf
	$(CC32_HEX) $(DIST_DIR)/AnalogOutputCard.elf

$(DIST_DIR)/AnalogOutputCard.elf : $(OBJS)
	@$(RM) $(DIST_DIR)
	@$(MKDIR) $(DIST_DIR)
	$(CC32) -mprocessor=$(MPROC) -o $(DIST_DIR)/AnalogOutputCard.elf $(OBJS) libmcc.a -DXPRJ_default=default -legacy-libc -Wl,--defsym=__MPLAB_BUILD=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="$(DIST_DIR)/AnalogOutputCard.map",--memorysummary,$(DIST_DIR)/memoryfile.xml

$(OBJ_DIR)/AnalogOutputCard/main.o : main.c $(PROJ_DEPS)
	@$(RM) $(OBJ_DIR)/AnalogOutputCard
	@$(MKDIR) $(OBJ_DIR)/AnalogOutputCard
	$(CC32) -g -x c -c -mprocessor=$(MPROC) -O1 -MMD -MF $(OBJ_DIR)/AnalogOutputCard/main.o.d -o $(OBJ_DIR)/AnalogOutputCard/main.o main.c -DXPRJ_default=default -legacy-libc 

$(OBJ_DIR)/aquaPicBus/aquaPicBus.o : ../../lib/aquaPicBus/aquaPicBus.c ../../lib/aquaPicBus/aquaPicBus.h
	@$(RM) $(OBJ_DIR)/aquaPicBus
	@$(MKDIR) $(OBJ_DIR)/aquaPicBus
	$(CC32) -g -x c -c -mprocessor=$(MPROC) -O1 -MMD -MF $(OBJ_DIR)/aquaPicBus/aquaPicBus.o.d -o $(OBJ_DIR)/aquaPicBus/aquaPicBus.o ../../lib/aquaPicBus/aquaPicBus.c -DXPRJ_default=default -legacy-libc 

$(OBJ_DIR)/nvm/nvm.o : ../../lib/PIC32MM/nvm/nvm.c ../../lib/PIC32MM/nvm/nvm.h
	@$(RM) $(OBJ_DIR)/nvm
	@$(MKDIR) $(OBJ_DIR)/nvm
	$(CC32) -g -x c -c -mprocessor=$(MPROC) -O1 -MMD -MF $(OBJ_DIR)/nvm/nvm.o.d -o $(OBJ_DIR)/nvm/nvm.o  ../../lib/PIC32MM/nvm/nvm.c -DXPRJ_default=default -legacy-libc 

.PHONY : clean
clean :
	@$(RM) $(DIST_DIR)
	@$(RM) $(OBJ_DIR)