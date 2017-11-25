#################################
###Shouldn't have to edit this###
#################################
include ../../Makefile-defines.mk

MPROC=32MM0064GPM028
OBJS=$(OBJ_DIR)/PhOrpCard/main.o $(OBJ_DIR)/aquaPicBus/aquaPicBus.o $(OBJ_DIR)/ltc2483/ltc2483.o
PROJ_DEPS=../../lib/aquaPicBus/aquaPicBus.h ltc2483/ltc2483.h

$(DIST_DIR)/PhOrpCard.hex : $(DIST_DIR)/PhOrpCard.elf
	$(CC32_HEX) $(DIST_DIR)/PhOrpCard.elf

$(DIST_DIR)/PhOrpCard.elf : $(OBJS)
	@$(RM) $(DIST_DIR)
	@$(MKDIR) $(DIST_DIR)
	$(CC32) -mprocessor=$(MPROC) -o $(DIST_DIR)/PhOrpCard.elf $(OBJS) libmcc.a -DXPRJ_default=default -legacy-libc -Wl,--defsym=__MPLAB_BUILD=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="$(DIST_DIR)/PhOrpCard.map",--memorysummary,$(DIST_DIR)/memoryfile.xml

$(OBJ_DIR)/PhOrpCard/main.o : main.c $(PROJ_DEPS)
	@$(RM) $(OBJ_DIR)/PhOrpCard
	@$(MKDIR) $(OBJ_DIR)/PhOrpCard
	$(CC32) -g -x c -c -mprocessor=$(MPROC) -O1 -MMD -MF $(OBJ_DIR)/PhOrpCard/main.o.d -o $(OBJ_DIR)/PhOrpCard/main.o main.c -DXPRJ_default=default -legacy-libc 

$(OBJ_DIR)/aquaPicBus/aquaPicBus.o : ../../lib/aquaPicBus/aquaPicBus.c ../../lib/aquaPicBus/aquaPicBus.h
	@$(RM) $(OBJ_DIR)/aquaPicBus
	@$(MKDIR) $(OBJ_DIR)/aquaPicBus
	$(CC32) -g -x c -c -mprocessor=$(MPROC) -O1 -MMD -MF $(OBJ_DIR)/aquaPicBus/aquaPicBus.o.d -o $(OBJ_DIR)/aquaPicBus/aquaPicBus.o ../../lib/aquaPicBus/aquaPicBus.c -DXPRJ_default=default -legacy-libc 

$(OBJ_DIR)/ltc2483/ltc2483.o : ltc2483/ltc2483.c ltc2483/ltc2483.h
	@$(RM) $(OBJ_DIR)/ltc2483
	@$(MKDIR) $(OBJ_DIR)/ltc2483
	$(CC32) -g -x c -c -mprocessor=$(MPROC) -O1 -MMD -MF $(OBJ_DIR)/ltc2483/ltc2483.o.d -o $(OBJ_DIR)/ltc2483/ltc2483.o ltc2483/ltc2483.c -DXPRJ_default=default -legacy-libc 

.PHONY : clean
clean :
	@$(RM) $(DIST_DIR)
	@$(RM) $(OBJ_DIR)