#!/bin/bash

wall -n -t 3 "Shutting down the system."

sleep 3
echo 17 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio17/direction
echo 0 > /sys/class/gpio/gpio17/value
sleep 3
echo 1 > /sys/class/gpio/gpio17/value

/bin/systemctl poweroff
