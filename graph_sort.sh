#!/bin/bash

if (($# <= 1)); then #Check if at least 2 args
    echo "no argument dected"
    exit 1
fi

if [ ! -f $1 ]; then #Check if arg1 is a file
    echo "arg 1 error : not a file"
    exit 2
fi

if [ $(echo ${1##*.}) != "csv" ]; then #Check if arg1 is a .csv file
    echo "arg 1 error : not a .csv file"
    exit 3
fi

if [ ! -d "temp" ]; then #Check if temp/ exist, else create it and give all acces
    mkdir temp
    chmod u+r+w+x temp

elif [ -s "temp" ]; then #Check if temp is empty, else rm its content
    rm -f -r -d temp/*
fi

if [ ! -d "pictures" ]; then #Check if pictures/ exist, else create it and give all acces
    mkdir pictures
    chmod u+r+w+x pictures
fi

tmp=$@

for i in ${tmp#*" "}; do #if arg? == -h || --help, arrays help and exit 0
    if [ $i = "-h" ] || [ $i = "--help" ]; then
        echo help msg
        exit 0
    fi
done

for i in ${tmp#*" "}; do

    case $i in

    -d1) #10 drivers with most rides
        echo d1
        ;;

    -d2) #10 drivers with longest distances
        ;;

    -l) #10 longest rides
        ;;

    -t) #10 most crossed towns
        ;;

    -s) #min, max, averrage distances
        ;;

    *) echo "arg error : $i is not an option (-h for help)" ;;

    esac
done

echo elapsed time: $SECONDS seconds #la var SECONDS contient le temps écoulé (SECONDS="0" pour réinitialiser)
