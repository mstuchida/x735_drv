#!/bin/bash

wall -n -t 3 "Shutting down the system."

sleep 3
echo 17 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio17/direction
echo 0 > /sys/class/gpio/gpio17/value
sleep 3
echo 1 > /sys/class/gpio/gpio17/value

sleep 3
echo 18 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio18/direction
echo 1 > /sys/class/gpio/gpio18/value
/bin/sleep 5
echo 0 > /sys/class/gpio/gpio18/value

/bin/systemctl poweroff
