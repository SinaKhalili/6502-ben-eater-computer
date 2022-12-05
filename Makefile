FQBN=arduino:avr:mega
PORT=/dev/cu.usbmodem13401
PROJECT=monitor6502
BAUD=57600

all: compile upload monitor
	@echo "Done."

upload:
	arduino-cli upload monitor6502 \
		--fqbn=$(FQBN) \
		--port=$(PORT) \
		--input-dir=out \

compile:
	arduino-cli compile -b $(FQBN) $(PROJECT) --output-dir out

monitor:
	arduino-cli monitor -b $(FQBN) --port $(PORT) --config baudrate=$(BAUD)

close:
	arudino-cli monitor -b $(FQBN) --port $(PORT) --quit