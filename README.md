# A scriptable guitar pedal
[![Build Status](https://travis-ci.org/thomasfaingnaert/scriptable-guitar-pedal.svg?branch=master)](https://travis-ci.org/thomasfaingnaert/scriptable-guitar-pedal)
## Building
### GNU/Linux (native compilation)
The following instructions are for Debian-based distributions.
Change the commands according to the distribution you are using.

1. Install the following prerequisites:
  - CMake
  - GNU Toolchain
  - Make
```bash
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install cmake build-essential
```

2. Clone the repository and build the project:
```bash
git clone https://github.com/thomasfaingnaert/scriptable-guitar-pedal
mkdir build
cd build
cmake ../scriptable-guitar-pedal
make
```

3. Run the executable:
```bash
chmod +x ./scriptable-guitar-pedal
./scriptable-guitar-pedal
```

### Windows (cross compilation)
1. Install the following prerequisites, and make sure they are in your PATH:
  - CMake (https://cmake.org/)
  - Beaglebone GNU Toolchain (http://gnutoolchains.com/beaglebone/)
  - NMake (included in Visual Studio or Microsoft Visual C++ Build Tools)
  - PuTTY (https://www.putty.org/)

2. Clone the repository and build the project:
```powershell
git clone https://github.com/thomasfaingnaert/scriptable-guitar-pedal
mkdir build
cd build
cmake -G"NMake Makefiles" -DCMAKE_TOOLCHAIN_FILE="../scriptable-guitar-pedal/cmake/Toolchain.cmake" ../scriptable-guitar-pedal
nmake
```

### GNU/Linux - Fedora (cross compilation)
These instructions are for Fedora based distributions.
Change the commands according to the distribution you are using.

1. Install the following prerequisites:
  - CMake
  - GNU Toolchain for ARM
  - Make
```bash
sudo dnf copr enable lantw44/arm-linux-gnueabihf-toolchain
sudo dnf install make cmake arm-linux-gnueabihf-{binutils,gcc,glibc}
```

2. Clone the repository and build the project:
```bash
git clone https://github.com/thomasfaingnaert/scriptable-guitar-pedal
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE="../scriptable-guitar-pedal/cmake/Toolchain.cmake" ../scriptable-guitar-pedal
make
```

## Building the ASoC codec driver

### GNU/Linux - Fedora (cross compilation)
These instructions are for Fedora based distributions.
Change the commands according to the distribution you are using.

1. Clone the official BeagleBoard and BeagleBone kernel repository and checkout the version the BeagleBone is running:
```bash
git clone https://github.com/beagleboard/linux
cd linux
git checkout 4.9.82-ti-r102
```

2. Make sure you have no stale .o files and dependencies lying around:
```bash
make mrproper
``` 

3. Modularize the ASoC codec driver:
```bash
mkdir ../build
make O=../build ARCH=arm bb.org_defconfig
make O=../build ARCH=arm menuconfig
```
Navigate to `Device Drivers/Sound Card Support/Advanced Linux Sound Architecture/ALSA for SoC audio support/CODEC drivers`
and modularize Cirrus Logic CS4271 CODEC (I2C).

4. Compile the kernel modules:
```bash
make O=../build ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules
```

5. Copy the modules on your to the BeagleBone Black:
```bash
scp ../build/sound/soc/codecs/snd-soc-cs4271-i2c.ko ../build/sound/soc/codecs/snd-soc-cs4271.ko debian@beaglebone.local:~
```

6. Install the modules on the BeagleBone Black:
```bash
ssh debian@beaglebone.local
sudo mv snd-soc-cs4271-i2c.ko snd-soc-cs4271.ko /lib/modules/4.9.82-ti-r102/extra/
sudo depmod -a
```

## Transferring to BeagleBone Black
To transfer the executable to your BeagleBone Black, you can use SSH, e.g. using PSCP (PuTTY Secure Copy) on Windows or SCP (Secure Copy) on GNU/Linux:
1. Copy the executable on your computer to the BeagleBone Black:
```powershell
pscp scriptable-guitar-pedal debian@192.168.7.2:/home/debian/scriptable-guitar-pedal
```
If you are using scp, change `pscp` to `scp`.
Also make sure to change the username (`debian`), destination path (`/home/debian/scriptable-guitar-pedal`) and IP address (`192.168.7.2`) if required.

2. Log in to the BeagleBone Black using SSH, and run the application:
```bash
chmod +x ./scriptable-guitar-pedal
./scriptable-guitar-pedal
```

## Using a custom Ne10 installation
The repository already includes a version of Ne10 in `include/ne10` and `lib/libNE10.a`.
If you want to use a custom Ne10 installation, you have to set the CMake variables `NE10_INCLUDE_DIRS` and `NE10_LIBRARIES` to point to the Ne10 include directory and library file, respectively.
An example when cross compiling on Windows:
```powershell
cmake -G"NMake Makefiles" -DCMAKE_TOOLCHAIN_FILE="../scriptable-guitar-pedal/cmake/Toolchain.cmake" -DNE10_INCLUDE_DIRS="/path/to/ne10/inc" -DNE10_LIBRARIES="/path/to/ne10/build/modules/libNE10.a" ../scriptable-guitar-pedal
```