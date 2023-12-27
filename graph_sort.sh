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
        start_time=$(date +%s.%N) #start the timer
        
        #awk part

        cut -d';' -f1,6 "$1" |awk -F';' '!arr[$1]++ {arr2[$2]++} END {for (i in arr2) printf "%s;%d\n",i,arr2[i]}' | sort -t';' -k2nr | head > temp/d1.temp
        
        # cut the data.csv file to just keep column 1 and 6
        # takes the duplicates of rides ID away using an arr to see if the ID were encountred already
        # next, counts the occurences of names using arr2 and separate them by ';' in the output
        # after all this, sort the output of names numerically by occurences and from the highest to lowest
        # using ; as delimiter. To finish, take the 10 first lines of the output
        # finally it sends all in the d1.temp file in temp/

gnuplot <<-EOF
    set terminal pngcairo enhanced font "arial,10"
    set output 'pictures/d1_output.png'

    # Données d'exemple (remplacez cela par le chemin de votre fichier de données)


    # Paramètres du graphique
    set datafile separator ";"
    set style fill solid
    set boxwidth 0.8
    set yrange [0:*]
    set xrange [0:250] # Définir une plage x appropriée
    set xlabel 'Number of Rides'
    set ylabel 'Driver'
    set title 'Horizontal Histogram'

    # Tracer l'histogramme horizontal
    plot 'temp/d1.temp' using 0:2:ytic(1) with boxes lc rgb "#61f2a2" notitle
EOF



        end_time=$(date +%s.%N) #end the timer
        elapsed_time=$(echo "$end_time - $start_time" | bc) #calculate the difference

        echo -e "\nelapsed time for -d1: $elapsed_time seconds"
        eog pictures/d1_output.png 
        ;;

    -d2) #10 drivers with longest distances
        start_time=$(date +%s.%N) #start the timer

        LC_NUMERIC=en_US.UTF-8 awk -F';' 'NR>1 {sum[$6] += $5} END {for (key in sum) printf "%s;%.2f\n", key, sum[key]}' data.csv| sort -t';' -k2nr | head
        
        end_time=$(date +%s.%N) #end the timer
        elapsed_time=$(echo "$end_time - $start_time" | bc) #calculate the difference
        
        echo -e "\nelapsed time for -d1: $elapsed_time seconds"
        ;;

    -l) #10 longest rides
        start_time=$(date +%s.%N) #start the timer
       
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
        end_time=$(date +%s.%N) #end the timer

        echo -e "\nelapsed time for -l: $end_time seconds" #la var SECONDS contient le temps écoulé (SECONDS="0" pour réinitialiser)
        eog pictures/l_output.png 
        ;;

    -t) #10 most crossed towns
        ;;

    -s) #min, max, averrage distances
        ;;

    *) echo "arg error : $i is not an option (-h for help)" ;;

    esac
done