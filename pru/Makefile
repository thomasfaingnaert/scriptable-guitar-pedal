PASM = pasm
PASMFLAGS = -bL
BIN = main.bin ram.bin

all: $(BIN)

%.bin: %.p
	$(PASM) $(PASMFLAGS) $< $(basename $@)

install:
	scp $(BIN) debian@beaglebone.local:~

clean:
	rm $(BIN)
