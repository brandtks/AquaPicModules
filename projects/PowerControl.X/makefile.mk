CC=xc8
OBJ_DIR=xbuild
DIST_DIR=xdist
MKDIR=mkdir -p
RM=rm -f

CHIP=16F1937
OBJS=xbuild/PowerControl.p1 xbuild/adc.p1 xbuild/uart.p1 xbuild/aquaPicBus.p1
PROJ_DEPS=../../lib/adc/adc.h ../../lib/aquaPicBus/aquaPicBus.h ../../lib/common/common.h ../../lib/led/led.h ../../lib/pins/pins.h bsp.h

$(DIST_DIR)/PowerControl.hex : $(OBJS)
	@$(MKDIR) $(DIST_DIR)
	@$(RM) $(DIST_DIR)/PowerControl.hex
	$(CC) --chip=$(CHIP) -O$(DIST_DIR)/PowerControl.hex $(OBJS)

$(OBJ_DIR)/PowerControl.p1 : PowerControl.c $(PROJ_DEPS)
	@$(MKDIR) $(OBJ_DIR)
	@-$(RM) $(OBJ_DIR)/PowerControl.p1
	$(CC) --chip=$(CHIP) --pass1 -O$(OBJ_DIR)/PowerControl.p1 PowerControl.c

$(OBJ_DIR)/adc.p1 : ../../lib/adc/adc.c ../../lib/adc/adc.h
	@$(MKDIR) $(OBJ_DIR)
	@-$(RM) $(OBJ_DIR)/adc.p1
	$(CC) --chip=$(CHIP) --pass1 -O$(OBJ_DIR)/adc.p1 ../../lib/adc/adc.c

$(OBJ_DIR)/uart.p1 : ../../lib/uart/uart.c ../../lib/uart/uart.h
	@$(MKDIR) $(OBJ_DIR)
	@$(RM) $(OBJ_DIR)/uart.p1
	$(CC) --chip=$(CHIP) --pass1 -O$(OBJ_DIR)/uart.p1 ../../lib/uart/uart.c

$(OBJ_DIR)/aquaPicBus.p1 : ../../lib/aquaPicBus/aquaPicBus.c ../../lib/aquaPicBus/aquaPicBus.h
	@$(MKDIR) $(OBJ_DIR)
	@$(RM) $(OBJ_DIR)/aquaPicBus.p1
	$(CC) --chip=$(CHIP) --pass1 -O$(OBJ_DIR)/aquaPicBus.p1 ../../lib/aquaPicBus/aquaPicBus.c
