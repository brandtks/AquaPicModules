include Makefile-defines.mk

POWER=projects/PowerControl.X/$(DIST_DIR)/PowerControl.hex
ANALOGINPUT=projects/Analog-Input-Card.X/$(DIST_DIR)/AnalogInputCard.hex
ANALOGOUTPUT=projects/Analog-Output-Card.X/$(DIST_DIR)/AnalogOutputCard.hex
DIGITALINPUT=projects/Digital-Input-Card.X/$(DIST_DIR)/AnalogInputCard.hex

PROJS=$(POWER) $(ANALOGINPUT) $(ANALOGOUTPUT) $(DIGITALINPUT)

all: $(PROJS);

$(POWER):
	@$(CD) projects/PowerControl.X && make --file=makefile.mk

$(ANALOGINPUT):
	@$(CD) projects/Analog-Input-Card.X && make --file=makefile.mk
	
$(ANALOGOUTPUT):
	@$(CD) projects/Analog-Output-Card.X && make --file=makefile.mk
	
$(DIGITALINPUT):
	@$(CD) projects/Digital-Input-Card.X && make --file=makefile.mk
	
.PHONY : clean
clean:
	@$(CD) projects/PowerControl.X && make --file=makefile.mk clean
	@$(CD) projects/Analog-Input-Card.X && make --file=makefile.mk clean
	@$(CD) projects/Analog-Output-Card.X && make --file=makefile.mk clean
	@$(CD) projects/Digital-Input-Card.X && make --file=makefile.mk clean