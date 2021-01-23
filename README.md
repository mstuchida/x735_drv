# x735_drv

This driver is an experimental Linux driver for the x735 expansion board.
Only the function to shut down the raspberry pi 4 when a GPIO4 interrupt occurs is implemented.

## Install

##### When DKMS is not used

```shell
$ cd driver
$ make
$ sudo make install
$ cd ..
```

##### When using DKMS

```shell
$ sudo mkdir -p /usr/src/x735_drv-0.1.0
$ sudo cp -a ./ /usr/src/x735_drv-0.1.0
$ sudo dkms add x735_drv -v 0.1.0
$ sudo dkms install x735_drv/0.1.0
```

##### Common

```shell
$ sudo cp ./etc/poweroff.sh /usr/local/bin/
$ sudo -u root echo "x735_drv" >> /etc/modules-load.d/modules.conf 
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

Checking the IRQ

```
$ cat /proc/interrupts | grep x735
 50:          0          0          0          0  pinctrl-bcm2835   4 Edge      x735
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
$ sudo dkms remove x735_drv/0.1.0 --all
$ sudo rm -rf /usr/src/x735_drv-0.1.0
```

## Notes

Cannot be used with x730pwr.sh and x730shutdown.sh at the same time.

## License

This driver is licensed under GPLv2.

