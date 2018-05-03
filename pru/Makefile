PASM = pasm
PASMFLAGS = -bL
BIN = main.bin

all: $(BIN)

%.bin: %.p
	$(PASM) $(PASMFLAGS) $< $(basename $@)

clean:
	rm $(BIN)

