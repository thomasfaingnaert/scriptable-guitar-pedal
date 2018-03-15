all:
	dtc -I dts -o alsa/beaglebone-black-codec.dtbo -O dtb -b 0 -@ beaglebone-black-codec-alsa.dts
	dtc -I dts -o direct/beaglebone-black-codec.dtbo -O dtb -b 0 -@ beaglebone-black-codec-direct.dts

clean:
	rm beaglebone-black-codec-alsa.dtbo
	rm beaglebone-black-codec-direct.dtbo

install:
	cp beaglebone-black-codec-alsa.dtbo /lib/firmware/
	cp beaglebone-black-codec-direct.dtbo /lib/firmware/
