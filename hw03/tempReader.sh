#! /bin/bash


while true; do
    TMP1=$(printf "%d" $(i2cget -y 1 0x45 1 w))
    TMP2=$(printf "%d" `i2cget -y 1 0x48 0`)
    TMP3=$(printf "%d" `i2cget -y 1 0x49 0`)
    
    ALARM=""
    if [ "$TMP1" -gt "13" ]; then
        ALARM="TMP1 > 23"
    fi
    if [ $TMP2 -gt "23" ]; then
        ALARM="$ALARM TMP2 > 23"
    fi
    if [ $TMP3 -gt "23" ]; then
        ALARM="$ALARM TMP3 > 23"
    fi
    echo $ALARM
    echo "TMP1: $TMP1, TMP2: $TMP2, TMP3: $TMP3"
    sleep 1
done