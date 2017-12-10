#################################
###Shouldn't have to edit this###
#################################
include ../../Makefile-defines.mk

PROJECT=DigitalInputCard
MPROC=32MM0064GPM028
OBJS=$(OBJ_DIR)/$(PROJECT)/main.o $(OBJ_DIR)/aquaPicBus/aquaPicBus.o
PROJ_DEPS=../../lib/aquaPicBus/aquaPicBus.h

$(DIST_DIR)/$(PROJECT).hex : $(DIST_DIR)/$(PROJECT).elf
	$(CC32_HEX) $(DIST_DIR)/$(PROJECT).elf

$(DIST_DIR)/$(PROJECT).elf : $(OBJS)
	@$(RM) $(DIST_DIR)
	@$(MKDIR) $(DIST_DIR)
	$(CC32) -mprocessor=$(MPROC) -o $(DIST_DIR)/$(PROJECT).elf $(OBJS) libmcc.a -DXPRJ_default=default -legacy-libc -Wl,--defsym=__MPLAB_BUILD=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="$(DIST_DIR)/$(PROJECT).map",--memorysummary,$(DIST_DIR)/memoryfile.xml

$(OBJ_DIR)/$(PROJECT)/main.o : main.c $(PROJ_DEPS)
	@$(RM) $(OBJ_DIR)/$(PROJECT)
	@$(MKDIR) $(OBJ_DIR)/$(PROJECT)
	$(CC32) -g -x c -c -mprocessor=$(MPROC) -O1 -MMD -MF $(OBJ_DIR)/$(PROJECT)/main.o.d -o $(OBJ_DIR)/$(PROJECT)/main.o main.c -DXPRJ_default=default -legacy-libc 

$(OBJ_DIR)/aquaPicBus/aquaPicBus.o : ../../lib/aquaPicBus/aquaPicBus.c ../../lib/aquaPicBus/aquaPicBus.h
	@$(RM) $(OBJ_DIR)/aquaPicBus
	@$(MKDIR) $(OBJ_DIR)/aquaPicBus
	$(CC32) -g -x c -c -mprocessor=$(MPROC) -O1 -MMD -MF $(OBJ_DIR)/aquaPicBus/aquaPicBus.o.d -o $(OBJ_DIR)/aquaPicBus/aquaPicBus.o ../../lib/aquaPicBus/aquaPicBus.c -DXPRJ_default=default -legacy-libc 

.PHONY : clean
clean :
	@$(RM) $(DIST_DIR)
	@$(RM) $(OBJ_DIR)