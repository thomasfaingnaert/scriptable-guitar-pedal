all:
	dtc -I dts -o beaglebone-black-codec.dtbo -O dtb -b 0 -@ beaglebone-black-codec.dts

clean:
	rm beaglebone-black-codec.dtbo

install:
	cp beaglebone-black-codec.dtbo /lib/firmware/

