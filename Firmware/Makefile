include Makefile-defines.mk

POWER=projects/PowerControl.X/$(DIST_DIR)/PowerControl.hex
ANALOGINPUT=projects/AnalogInputCard.X/$(DIST_DIR)/AnalogInputCard.hex
ANALOGOUTPUT=projects/AnalogOutputCard.X/$(DIST_DIR)/AnalogOutputCard.hex
DIGITALINPUT=projects/DigitalInputCard.X/$(DIST_DIR)/DigitalInputCard.hex
PHORP=projects/PhOrpCard.X/$(DIST_DIR)/PhOrpCard.hex

PROJS=$(POWER) $(ANALOGINPUT) $(ANALOGOUTPUT) $(DIGITALINPUT) $(PHORP)

all: $(PROJS);

$(POWER):
	@$(CD) projects/PowerControl.X && make --file=makefile.mk

$(ANALOGINPUT):
	@$(CD) projects/AnalogInputCard.X && make --file=makefile.mk
	
$(ANALOGOUTPUT):
	@$(CD) projects/AnalogOutputCard.X && make --file=makefile.mk
	
$(DIGITALINPUT):
	@$(CD) projects/DigitalInputCard.X && make --file=makefile.mk

$(PHORP):
	@$(CD) projects/PhOrpCard.X && make --file=makefile.mk
	
.PHONY : clean
clean:
	@$(CD) projects/PowerControl.X && make --file=makefile.mk clean
	@$(CD) projects/AnalogInputCard.X && make --file=makefile.mk clean
	@$(CD) projects/AnalogOutputCard.X && make --file=makefile.mk clean
	@$(CD) projects/DigitalInputCard.X && make --file=makefile.mk clean
	@$(CD) projects/PhOrpCard.X && make --file=makefile.mk clean