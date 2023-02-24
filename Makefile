FQBN := $(shell arduino-cli board list --format json | jq -r '.[] | select(.matching_boards[0].fqbn != null) | .matching_boards[0].fqbn')
PORT := $(shell arduino-cli board list --format json | jq -r '.[] | select(.matching_boards[0].fqbn != null) | .port.address')
PROJECT=EEPROM_programmer
ASM=helloworld
BAUD=57600

all: asm compile upload monitor
	@echo "Done."

upload:
	arduino-cli upload $(PROJECT) \
		--fqbn=$(FQBN) \
		--port=$(PORT) \
		--input-dir=out \

compile:
	arduino-cli compile -b $(FQBN) $(PROJECT) --output-dir out

monitor:
	arduino-cli monitor -b $(FQBN) --port $(PORT) --config baudrate=$(BAUD)

close:
	arudino-cli monitor -b $(FQBN) --port $(PORT) --quit

asm: 
	./vasm6502_oldstyle -Fbin -dotdir -o ./bin/out.bin ./asm_src/$(ASM).asm
	xxd -i -l 420 ./bin/out.bin > ./EEPROM_programmer/out.h

clean:
	rm -rf out
	rm bin/*