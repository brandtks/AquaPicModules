#################################
###Shouldn't have to edit this###
#################################
include ../../Makefile-defines.mk

UPROC=32MM0064GPM028
OBJS=$(OBJ_DIR)/AnalogOutputCard.o $(OBJ_DIR)/aquaPicBus/aquaPicBus.o
PROJ_DEPS=../../lib/aquaPicBus/aquaPicBus.h

$(DIST_DIR)/AnalogOutputCard.hex : $(OBJS)
	@$(RM) $(DIST_DIR)
	@$(MKDIR) $(DIST_DIR)
	$(CC) --chip=$(CHIP) -Q -G${DIST_DIR}/AnalogOutputCard.sym -M${DIST_DIR}/AnalogOutputCard.map --double=24 --float=24 --opt=$(OPT) --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist -DXPRJ_default=default --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" --memorysummary $(DIST_DIR)/memoryfile.xml -O$(DIST_DIR)/AnalogOutputCard.elf $(OBJS)

$(OBJ_DIR)/AnalogOutputCard/AnalogOutputCard.p1 : AnalogOutputCard.c $(PROJ_DEPS)
	@$(RM) $(OBJ_DIR)/AnalogOutputCard
	@$(MKDIR) $(OBJ_DIR)/AnalogOutputCard
	$(CC) --pass1 --chip=$(CHIP) -Q --double=24 --float=24 --opt=$(OPT) --addrqual=ignore --mode=$(MODE) -P --warn=$(WARN) --asmlist --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" -O$(OBJ_DIR)/AnalogOutputCard/AnalogOutputCard.p1 AnalogOutputCard.c

$(OBJ_DIR)/uart/uart.p1 : ../../lib/uart/uart.c ../../lib/uart/uart.h
	@$(RM) $(OBJ_DIR)/uart
	@$(MKDIR) $(OBJ_DIR)/uart
	$(CC) --pass1 --chip=$(CHIP) -Q --double=24 --float=24 --opt=$(OPT) --addrqual=ignore --mode=$(MODE) -P --warn=$(WARN) --asmlist --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" -O$(OBJ_DIR)/uart/uart.p1 ../../lib/uart/uart.c

$(OBJ_DIR)/aquaPicBus/aquaPicBus.o : ../../lib/aquaPicBus/aquaPicBus.c ../../lib/aquaPicBus/aquaPicBus.h
	@$(RM) $(OBJ_DIR)/aquaPicBus
	@$(MKDIR) $(OBJ_DIR)/aquaPicBus
	$(CC) --pass1 --chip=$(CHIP) -Q --double=24 --float=24 --opt=$(OPT) --addrqual=ignore --mode=$(MODE) -P --warn=$(WARN) --asmlist --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" -O$(OBJ_DIR)/aquaPicBus/aquaPicBus.p1 ../../lib/aquaPicBus/aquaPicBus.c
	$(CC32) -g -x c -c -mprocessor=$(UPROC) -O1 -MMD -MF $(OBJ_DIR)/aquaPicBus/aquaPicBus.o.d -o $(OBJ_DIR)/aquaPicBus/aquaPicBus.o ../../lib/aquaPicBus/aquaPicBus.c -DXPRJ_default=default -legacy-libc 

$(OBJ_DIR)/pwm/pwm.p1 : ../../lib/pwm/pwm.c ../../lib/pwm/pwm.h
	@$(RM) $(OBJ_DIR)/pwm
	@$(MKDIR) $(OBJ_DIR)/pwm
	$(CC) --pass1 --chip=$(CHIP) -Q --double=24 --float=24 --opt=$(OPT) --addrqual=ignore --mode=$(MODE) -P --warn=$(WARN) --asmlist --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" -O$(OBJ_DIR)/pwm/pwm.p1 ../../lib/pwm/pwm.c

.PHONY : clean
clean :
	@$(RM) $(DIST_DIR)
	@$(RM) $(OBJ_DIR)

$(CC32) -g -x c -c -mprocessor=$(UPROC) -O1 -MMD -MF build/default/production/mcc_generated_files/pin_manager.o.d -o build/default/production/mcc_generated_files/pin_manager.o mcc_generated_files/pin_manager.c -DXPRJ_default=default -legacy-libc 