
#scp ars2000.ko root@192.168.1.211:/home/root
scp ars2000-legacy.ko root@192.168.1.210:/home/root/ars2000.ko

#ssh root@192.168.1.211 '/bin/sync && /sbin/insmod /home/root/ars2000.ko && /sbin/rmmod ars2000'
ssh root@192.168.1.210 '/bin/sync && /sbin/insmod /home/root/ars2000.ko && /sbin/rmmod ars2000'
