Prerequisites: update to the latest debian distribution by following these steps:
http://beagleboard.org/Support/BoneScript/updates/

ssh to access Beaglebone command line: 
$ ssh 192.168.7.2 -l debian
Password: temppwd

LED PIN : pin 49         
	--Output pin goes HI whenever non-zero data is read or written to the 
          device, else output is LO

INPUT GPIO PIN: pin 117
	+3.3V -> Output = 1
	GND   -> Output = 0

1) On beaglebone, run $make command to build the kernel module and compile the test application
2) On beaglebone, run $./shellscript to create device in /dev/ folder, insert 
   kernel module with the $insmod command, and run the test application test.c   

You should see the kernel module sucessfully uploaded to the kernel using $lsmod | grep char

test.c is used to test the character device driver by running read/write commands on the specified device.


