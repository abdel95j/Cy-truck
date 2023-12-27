#!/bin/bash

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

for i in ${tmp#*" "}; do #if arg? == -h || --help, arrays help and exit 0
    if [ $i = "-h" ] || [ $i = "--help" ]; then
        echo help msg
        exit 0
    fi
done

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

tmp=$@

for i in ${tmp#*" "}; do

    case $i in

    -d1) #10 drivers with most rides
        SECONDS=0
        
        #awk part

        cut -d';' -f1,6 < $1 | awk -F';' '!arr[$1]++ {arr2[$2]++} END {for (i in arr2) print i ";" arr2[i]}' | sort -t';' -k2,2nr | head -n 10 > temp/d1.temp
        
        # cut the data.csv file to just keep column 1 and 6
        # takes the duplicates of rides ID away using a arr to see if the ID were encountred already
        # next, counts the occurences of names using arr2 and separate them by ';' in the output
        # after all this, sort the output of names numerically by occurences and from the highest to lowest
        # using ; as delimiter. To finish, take the 10 first lines of the output
        # finally it sends all in the d1.temp file in temp/

        gnuplot <<-EOFMarker
    set terminal png size 800,600
    set output 'pictures/d1.png'
    set style data boxes
    set style fill solid border
    set boxwidth 0.8
    set datafile separator ";"
    set title '10 drivers with most rides'
    plot "temp/d1.temp" using 2:xtic(1) with boxxy lc var notitle
EOFMarker
        
        echo -e "\nelapsed time for d1: $SECONDS seconds" #la var SECONDS contient le temps écoulé (SECONDS="0" pour réinitialiser)
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