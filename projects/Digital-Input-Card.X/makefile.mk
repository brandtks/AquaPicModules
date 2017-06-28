#################################
###Shouldn't have to edit this###
#################################
include ../../Makefile-defines.mk

CHIP=16F1936
OBJS=$(OBJ_DIR)/DigitalInputCard/DigitalInputCard.p1 $(OBJ_DIR)/uart/uart.p1 $(OBJ_DIR)/aquaPicBus/aquaPicBus.p1
PROJ_DEPS=../../lib/aquaPicBus/aquaPicBus.h ../../lib/common/common.h ../../lib/led/led.h ../../lib/pins/pins.h bsp.h

$(DIST_DIR)/DigitalInputCard.hex : $(OBJS)
	@$(RM) $(DIST_DIR)
	@$(MKDIR) $(DIST_DIR)
	$(CC) --chip=$(CHIP) -Q -G${DIST_DIR}/DigitalInputCard.sym -M${DIST_DIR}/DigitalInputCard.map --double=24 --float=24 --opt=$(OPT) --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist -DXPRJ_default=default --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" --memorysummary $(DIST_DIR)/memoryfile.xml -O$(DIST_DIR)/DigitalInputCard.elf $(OBJS)

$(OBJ_DIR)/DigitalInputCard/DigitalInputCard.p1 : DigitalInputCard.c $(PROJ_DEPS)
	@$(RM) $(OBJ_DIR)/DigitalInputCard
	@$(MKDIR) $(OBJ_DIR)/DigitalInputCard
	$(CC) --pass1 --chip=$(CHIP) -Q --double=24 --float=24 --opt=$(OPT) --addrqual=ignore --mode=$(MODE) -P --warn=$(WARN) --asmlist --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" -O$(OBJ_DIR)/DigitalInputCard/DigitalInputCard.p1 DigitalInputCard.c

$(OBJ_DIR)/uart/uart.p1 : ../../lib/uart/uart.c ../../lib/uart/uart.h
	@$(RM) $(OBJ_DIR)/uart
	@$(MKDIR) $(OBJ_DIR)/uart
	$(CC) --pass1 --chip=$(CHIP) -Q --double=24 --float=24 --opt=$(OPT) --addrqual=ignore --mode=$(MODE) -P --warn=$(WARN) --asmlist --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" -O$(OBJ_DIR)/uart/uart.p1 ../../lib/uart/uart.c

$(OBJ_DIR)/aquaPicBus/aquaPicBus.p1 : ../../lib/aquaPicBus/aquaPicBus.c ../../lib/aquaPicBus/aquaPicBus.h $(OBJ_DIR)/uart/uart.p1
	@$(RM) $(OBJ_DIR)/aquaPicBus
	@$(MKDIR) $(OBJ_DIR)/aquaPicBus
	$(CC) --pass1 --chip=$(CHIP) -Q --double=24 --float=24 --opt=$(OPT) --addrqual=ignore --mode=$(MODE) -P --warn=$(WARN) --asmlist --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" -O$(OBJ_DIR)/aquaPicBus/aquaPicBus.p1 ../../lib/aquaPicBus/aquaPicBus.c

.PHONY : clean
clean :
	@$(RM) $(DIST_DIR)
	@$(RM) $(OBJ_DIR)
