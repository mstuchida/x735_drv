#!/bin/bash

sleep 5
echo 17 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio17/direction
echo 0 > /sys/class/gpio/gpio17/value
sleep 5
echo 1 > /sys/class/gpio/gpio17/value

/bin/systemctl poweroff
