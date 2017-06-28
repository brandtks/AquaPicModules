include Makefile-defines.mk

PROJS=projects/PowerControl.X/$(DIST_DIR)/PowerControl.hex projects/Analog-Input-Card.X/$(DIST_DIR)/AnalogInputCard.hex \
	projects/Analog-Output-Card.X/$(DIST_DIR)/AnalogOutputCard.hex projects/Digital-Input-Card.X/$(DIST_DIR)/AnalogInputCard.hex

all: $(PROJS);

projects/PowerControl.X/$(DIST_DIR)/PowerControl.hex:
	@$(CD) projects/PowerControl.X && make --file=makefile.mk

projects/Analog-Input-Card.X/$(DIST_DIR)/AnalogInputCard.hex:
	@$(CD) projects/Analog-Input-Card.X && make --file=makefile.mk
	
projects/Analog-Output-Card.X/$(DIST_DIR)/AnalogOutputCard.hex:
	@$(CD) projects/Analog-Output-Card.X && make --file=makefile.mk
	
projects/Digital-Input-Card.X/$(DIST_DIR)/AnalogInputCard.hex:
	@$(CD) projects/Digital-Input-Card.X && make --file=makefile.mk
	
.PHONY : clean
clean :
	@$(CD) projects/PowerControl.X && make --file=makefile.mk clean
	@$(CD) projects/Analog-Input-Card.X && make --file=makefile.mk clean
	@$(CD) projects/Analog-Output-Card.X && make --file=makefile.mk clean
	@$(CD) projects/Digital-Input-Card.X && make --file=makefile.mk clean
