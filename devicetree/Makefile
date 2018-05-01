DTC = dtc
DTFLAGS = --in-format dts --out-format dtb --warning no-unit_address_vs_reg --symbols

DTBO = beaglebone-black-codec-alsa.dtbo beaglebone-black-codec-pru.dtbo

all: $(DTBO)

%.dtbo : %.dts
	$(DTC) $(DTFLAGS) $< --out $@

clean:
	rm $(DTBO)

install:
	cp $(DTBO) /lib/firmware/
