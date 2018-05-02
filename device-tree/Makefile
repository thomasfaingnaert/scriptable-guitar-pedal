DTC = dtc
DTFLAGS = --in-format dts --out-format dtb --warning no-unit_address_vs_reg --warning no-avoid_default_addr_size --warning no-reg_format --symbols

DTBO = beaglebone-black-codec-alsa.dtbo beaglebone-black-codec-pru.dtbo

all: $(DTBO)

%.dtbo : %.dts
	$(DTC) $(DTFLAGS) $< --out $@

clean:
	rm $(DTBO)

install:
	cp $(DTBO) /lib/firmware/
