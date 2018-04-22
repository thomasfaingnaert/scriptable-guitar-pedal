all:
	dtc -I dts -o beaglebone-black-codec-alsa.dtbo -O dtb -b 0 -@ beaglebone-black-codec-alsa.dts
	dtc -I dts -o beaglebone-black-codec-pru.dtbo -O dtb -b 0 -@ beaglebone-black-codec-pru.dts

clean:
	rm beaglebone-black-codec-alsa.dtbo
	rm beaglebone-black-codec-pru.dtbo

install:
	cp beaglebone-black-codec-alsa.dtbo /lib/firmware/
	cp beaglebone-black-codec-pru.dtbo /lib/firmware/

