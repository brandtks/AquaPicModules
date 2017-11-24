#################################
###Shouldn't have to edit this###
#################################
include ../../Makefile-defines.mk

CHIP=16F1936
OBJS=$(OBJ_DIR)/AnalogOutputCard/AnalogOutputCard.p1 $(OBJ_DIR)/uart/uart.p1 $(OBJ_DIR)/aquaPicBus/aquaPicBus.p1 $(OBJ_DIR)/pwm/pwm.p1
PROJ_DEPS=../../lib/aquaPicBus/aquaPicBus.h ../../lib/common/common.h ../../lib/led/led.h ../../lib/pins/pins.h ../../lib/PIC16F/pwm/pwm.h ../../lib/PIC16F/uart/uart.h bsp.h

$(DIST_DIR)/AnalogOutputCard.hex : $(OBJS)
	@$(RM) $(DIST_DIR)
	@$(MKDIR) $(DIST_DIR)
	$(CC) --chip=$(CHIP) -Q -G${DIST_DIR}/AnalogOutputCard.sym -M${DIST_DIR}/AnalogOutputCard.map --double=24 --float=24 --opt=$(OPT) --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist -DXPRJ_default=default --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" --memorysummary $(DIST_DIR)/memoryfile.xml -O$(DIST_DIR)/AnalogOutputCard.elf $(OBJS)

$(OBJ_DIR)/AnalogOutputCard/AnalogOutputCard.p1 : AnalogOutputCard.c $(PROJ_DEPS)
	@$(RM) $(OBJ_DIR)/AnalogOutputCard
	@$(MKDIR) $(OBJ_DIR)/AnalogOutputCard
	$(CC) --pass1 --chip=$(CHIP) -Q --double=24 --float=24 --opt=$(OPT) --addrqual=ignore --mode=$(MODE) -P --warn=$(WARN) --asmlist --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" -O$(OBJ_DIR)/AnalogOutputCard/AnalogOutputCard.p1 AnalogOutputCard.c

$(OBJ_DIR)/uart/uart.p1 : ../../lib/PIC16F/uart/uart.c ../../lib/PIC16F/uart/uart.h
	@$(RM) $(OBJ_DIR)/uart
	@$(MKDIR) $(OBJ_DIR)/uart
	$(CC) --pass1 --chip=$(CHIP) -Q --double=24 --float=24 --opt=$(OPT) --addrqual=ignore --mode=$(MODE) -P --warn=$(WARN) --asmlist --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" -O$(OBJ_DIR)/uart/uart.p1 ../../lib/PIC16F/uart/uart.c

$(OBJ_DIR)/aquaPicBus/aquaPicBus.p1 : ../../lib/aquaPicBus/aquaPicBus.c ../../lib/aquaPicBus/aquaPicBus.h
	@$(RM) $(OBJ_DIR)/aquaPicBus
	@$(MKDIR) $(OBJ_DIR)/aquaPicBus
	$(CC) --pass1 --chip=$(CHIP) -Q --double=24 --float=24 --opt=$(OPT) --addrqual=ignore --mode=$(MODE) -P --warn=$(WARN) --asmlist --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" -O$(OBJ_DIR)/aquaPicBus/aquaPicBus.p1 ../../lib/aquaPicBus/aquaPicBus.c

$(OBJ_DIR)/pwm/pwm.p1 : ../../lib/PIC16F/pwm/pwm.c ../../lib/PIC16F/pwm/pwm.h
	@$(RM) $(OBJ_DIR)/pwm
	@$(MKDIR) $(OBJ_DIR)/pwm
	$(CC) --pass1 --chip=$(CHIP) -Q --double=24 --float=24 --opt=$(OPT) --addrqual=ignore --mode=$(MODE) -P --warn=$(WARN) --asmlist --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" -O$(OBJ_DIR)/pwm/pwm.p1 ../../lib/PIC16F/pwm/pwm.c

.PHONY : clean
clean :
	@$(RM) $(DIST_DIR)
	@$(RM) $(OBJ_DIR)
