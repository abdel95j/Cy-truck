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

    -d1) #10 drivers with most Nrides
        SECONDS=0
        
        #awk part

        cut -d';' -f1,6 "$1" |awk -F';' '!arr[$1]++ {arr2[$2]++} END {for (i in arr2) printf "%s;%d\n",i,arr2[i]}' | sort -t';' -k2nr | head > temp/d1.temp
        
        # cut the data.csv file to just keep column 1 and 6
        # takes the duplicates of rides ID away using an arr to see if the ID were encountred already
        # next, counts the occurences of names using arr2 and separate them by ';' in the output
        # after all this, sort the output of names numerically by occurences and from the highest to lowest
        # using ; as delimiter. To finish, take the 10 first lines of the output
        # finally it sends all in the d1.temp file in temp/

gnuplot <<-EOF
        reset
        set terminal pngcairo enhanced font "arial,10"
        set output 'pictures/d1_output.png'
        set style fill solid noborder
        set autoscale yfix
        set offset 0,1,0.5,0.5
        set xrange [0:*]
        plot 'temp/d1.temp' using (\${2}*0.5):0:(\${2}*0.5):(0.4):yticlabel(1) with boxxyerrorbars notitle
EOF

        
        echo -e "\nelapsed time for -d1: $SECONDS seconds"
        eog pictures/d1_output.png 
        ;;

    -d2) #10 drivers with longest distances
        ;;

    -l) #10 longest rides
        SECONDS=0
       
        #awk part

        cut -d';' -f1,5 $1 |LC_NUMERIC=en_US.UTF-8 awk -F';' '{arr[$1]+=$2} END {for (i in arr) printf "%s;%f\n", i, arr[i]}' |sort -t';' -k2,2nr |head -n 10 | sort -t';' -k1,2nr > temp/l.temp
        
        # Using Gnuplot to create the chart
gnuplot << EOF
        set terminal pngcairo enhanced font "arial,10"
        set output 'pictures/l_output.png'
        set title "Option -l : Distance = f(Route)"
        set xlabel "ROUTE ID"
        set ylabel "DISTANCE (Km)"
        set style data histograms
        set style fill solid
        set boxwidth 2.0
        set yrange [0:3000]
        set datafile separator ";"
        plot "temp/l.temp" using 2:xtic(1) lc rgb "#61f2a2" with histograms title "Total travel distances"
EOF

        echo -e "\nelapsed time for -l: $SECONDS seconds" #la var SECONDS contient le temps écoulé (SECONDS="0" pour réinitialiser)
        ;;

    -t) #10 most crossed towns
        ;;

    -s) #min, max, averrage distances
        ;;

    *) echo "arg error : $i is not an option (-h for help)" ;;

    esac
done