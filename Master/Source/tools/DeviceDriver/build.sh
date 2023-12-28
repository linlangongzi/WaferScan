export OLD_PATH=$PATH

rm ars2000.ko

export PATH=/home/llxibo/SBC37x_B2_DISC-V3.0/arm-2009q1/bin:$OLD_PATH
echo Making module for SBC3730 with path $PATH
#make ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi- -C /home/llxibo/SBC37x_B2_DISC-V3.0/ars500/linux M=`pwd` modules
make ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi- -C /home/llxibo/SBC37x_B2_DISC-V3.0/linux-03.00.01.06 M=`pwd` modules

mv ars2000.ko ars2000-legacy.ko
echo

export PATH=/home/llxibo/ti-processor-sdk-linux-am437x-evm-01.00.00.03/linux-devkit/sysroots/i686-arago-linux/usr/bin:$OLD_PATH
echo Making module for AM4379 with path $PATH
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C /home/llxibo/linux-4379 M=`pwd` modules
