#! /bin/bash

greet() {
    echo "Welcome to the temp detector!"
    echo "This will poll a tmp101 sensor's alert pin, attached to GP1_4"
    echo "If a value (in celcius) is passed to this function, it will set that as the alarm value, otherwise it will default to 20c"
}

setup() {
    echo 97 > /sys/class/gpio/export
    i2cset -y 1 "$1" 0x1 0x00
    i2cset -y 1 "$1" 0x2 "0x$2"
    i2cset -y 1 "$1" 0x3 "0x$2"
}

desetup() {
    echo 97 > /sys/class/gpio/unexport
    exit 0
}

trap desetup INT

SENSOR=0x48
TEMP=20
DEBUG=0

while getopts :t:s:hd opt; do
    case $opt in
        t)
        echo "Temp set to $OPTARG" >&2
        TEMP=$OPTARG
        ;;
        s)
        echo "Sensor set to $OPTARG" >&2
        SENSOR=$OPTARG
        ;;
        h)
        echo "Use -s <sensor(hex)> and -t <temp(c)> to pass target temp and sensor, -d is debug mode" >&2
        exit 1
        ;;
        d)
        DEBUG=1
        ;;
        \?)
        echo "Invalid Argument: -$OPTARG" >&2 
        exit 1
        ;;
    esac
done

TEMP=$(echo "obase=16; $TEMP" | bc)
greet
setup $SENSOR $TEMP

while true; do

    PINVAL=$(cat /sys/class/gpio/gpio97/value)
    
    if [ $PINVAL -eq 0 ]; then
        echo "Alert!" 
        echo "Temp = $(echo $(printf "%d*9/5+32" $(i2cget -y 1 $SENSOR 0)) | bc)°f"
    fi
    if [ $DEBUG -eq 1 ]; then
        echo "TEMP = 0x$TEMP, READ = $(i2cget -y 1 $SENSOR 0)"
    fi
    sleep 1
done

