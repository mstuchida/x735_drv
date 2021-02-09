# x735_drv

This driver is an experimental Linux driver for the x735 expansion board.  
Only the function to shut down the raspberry pi 4 when a GPIO4 interrupt occurs is implemented.  
The GPIO4 interrupt is generated when the power switch is pressed.

## Install

gcc, make, kernel source / header must be installed.  

Please refer to the following.  
https://www.raspberrypi.org/documentation/linux/kernel/headers.md

##### When DKMS is not used

```shell
$ cd driver
$ make
$ sudo make install
$ cd ..
```

##### When using DKMS

```shell
$ sudo mkdir -p /usr/src/x735_drv-0.1.2
$ sudo cp -a ./ /usr/src/x735_drv-0.1.2
$ sudo dkms add x735_drv -v 0.1.2
$ sudo dkms install x735_drv/0.1.2
```

## Confirm

Checking the loading of the kernel module

```
$ lsmod | grep -e ^x735_drv
x735_drv               20480  0
```

If nothing is displayed, execute the following command, and then execute the above command again to confirm.

```shell
$ sudo modprobe x735_drv
```

## Uninstall

##### When DKMS is not used

```shell
$ cd driver
$ sudo make uninstall
$ cd ..
```

##### When using DKMS

```shell
$ sudo dkms remove x735_drv/0.1.2 --all
$ sudo rm -rf /usr/src/x735_drv-0.1.2
```

## Notes

Cannot be used with x730pwr.sh and x730shutdown.sh at the same time.

Development environment
- Raspberry Pi 4 (8GB)
- X735 - v2.1
- X829 - v1.3
- X829 - C1

## License

This driver is licensed under GPLv2.


