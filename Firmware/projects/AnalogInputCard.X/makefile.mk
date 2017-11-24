#################################
###Shouldn't have to edit this###
#################################
include ../../Makefile-defines.mk

MPROC=32MM0064GPM028
OBJS=$(OBJ_DIR)/AnalogInputCard/main.o $(OBJ_DIR)/aquaPicBus/aquaPicBus.o $(OBJ_DIR)/mcp3428/mcp3428.o
PROJ_DEPS=../../lib/aquaPicBus/aquaPicBus.h mcp3428/mcp3428.h

$(DIST_DIR)/AnalogInputCard.hex : $(DIST_DIR)/AnalogInputCard.elf
	$(CC32_HEX) $(DIST_DIR)/AnalogInputCard.elf

$(DIST_DIR)/AnalogInputCard.elf : $(OBJS)
	@$(RM) $(DIST_DIR)
	@$(MKDIR) $(DIST_DIR)
	$(CC32) -mprocessor=$(MPROC) -o $(DIST_DIR)/AnalogInputCard.elf $(OBJS) libmcc.a -DXPRJ_default=default -legacy-libc -Wl,--defsym=__MPLAB_BUILD=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="$(DIST_DIR)/AnalogInputCard.map",--memorysummary,$(DIST_DIR)/memoryfile.xml

$(OBJ_DIR)/AnalogInputCard/main.o : main.c $(PROJ_DEPS)
	@$(RM) $(OBJ_DIR)/AnalogInputCard
	@$(MKDIR) $(OBJ_DIR)/AnalogInputCard
	$(CC32) -g -x c -c -mprocessor=$(MPROC) -O1 -MMD -MF $(OBJ_DIR)/AnalogInputCard/main.o.d -o $(OBJ_DIR)/AnalogInputCard/main.o main.c -DXPRJ_default=default -legacy-libc 

$(OBJ_DIR)/aquaPicBus/aquaPicBus.o : ../../lib/aquaPicBus/aquaPicBus.c ../../lib/aquaPicBus/aquaPicBus.h
	@$(RM) $(OBJ_DIR)/aquaPicBus
	@$(MKDIR) $(OBJ_DIR)/aquaPicBus
	$(CC32) -g -x c -c -mprocessor=$(MPROC) -O1 -MMD -MF $(OBJ_DIR)/aquaPicBus/aquaPicBus.o.d -o $(OBJ_DIR)/aquaPicBus/aquaPicBus.o ../../lib/aquaPicBus/aquaPicBus.c -DXPRJ_default=default -legacy-libc 

$(OBJ_DIR)/mcp3428/mcp3428.o : mcp3428/mcp3428.c mcp3428/mcp3428.h
	@$(RM) $(OBJ_DIR)/mcp3428
	@$(MKDIR) $(OBJ_DIR)/mcp3428
	$(CC32) -g -x c -c -mprocessor=$(MPROC) -O1 -MMD -MF $(OBJ_DIR)/mcp3428/mcp3428.o.d -o $(OBJ_DIR)/mcp3428/mcp3428.o mcp3428/mcp3428.c -DXPRJ_default=default -legacy-libc 

.PHONY : clean
clean :
	@$(RM) $(DIST_DIR)
	@$(RM) $(OBJ_DIR)