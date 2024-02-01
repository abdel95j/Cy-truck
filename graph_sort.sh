#!/bin/bash

if [ ! -d "temp" ]; then #Check if temp/ exist, else create it and give all access
    mkdir temp
    chmod u+r+w+x temp

elif [ -s "temp" ]; then #Check if temp/ is empty, else rm its content
    rm -rf temp/*.temp
fi

if [ ! -d "pictures" ]; then #Check if pictures/ exist, else create it and give all access
    mkdir pictures
    chmod u+r+w+x pictures
fi

tmp=$*

for i in ${tmp#*" "}; do    #special options (will read the last you put)
    case $i in
        -h | --help)    #if arg? == -h || --help, arrays help and exit 0
            echo -e "Usage : ./graph_sort.sh [FILE.csv] [OPTION]\n"
            echo -e "FILE has to be a .csv file using the right format. \n Format : Route ID;Step ID;Town A;Town B;Distance;Driver name \n"
            echo -e "Available options:\n"
            echo -e " -d1 :        Gets the 10 drivers with the most amount of drives and arrays a horizontal\n              histogram graph\n\n"
            echo -e " -d2 :        Gets the 10 drivers with the longest total distances crossed and arrays a\n              horizontal histogram graph\n\n"
            echo -e " -l  :        Gets the 10 longest roads and arrays a vertical histogram graph\n\n\n"
            echo -e " -t  :        Gets the 10 most crossed towns and arrays vertical histogram graph with the\n              amount of crossing and the number of time when the town is a departure city\n              of a road\n\n"
            echo -e " -s  :        Gets the min, max and average distance of every road. Keeps the 50 highest\n              sorted by (max - min) and arrays a min-max-av graph\n\n"
            echo -e " -c | --clean :    Cleans the directory by removing temp files, pictures files and c\n                   executables in progc and exit\n\n"
            echo -e " -h | --help  :    Arrays help and exit\n"
            
            exit 0 ;;
        
        -c | --clean)   #if arg? == -c || --clean, cleans directories
            cd temp || exit ; rm -rf *.temp ; cd ..
            cd progc || exit ; rm -rf *_progc ; cd ..
            cd pictures || exit ; rm -rf *.png ; cd ..
            echo clean succeded
            exit 0;;
    esac 
done

if (($# <= 1)); then #Check if at least 2 args
    echo "arg error : no argument detected"
    exit 1
fi

if [ ! -f "$1" ]; then #Check if arg1 is a file
    echo "arg 1 error : not a file"
    exit 2
fi

if [ $(echo "${1##*.}") != "csv" ]; then #Check if arg1 is a .csv file
    echo "arg 1 error : not a .csv file"
    exit 3
fi

echo "starting epîc music ..."
sleep 1
vlc -Idummy data/c\'est_pas_cy.mp3 &> temp/vlc.log &

for i in ${tmp#*" "}; do

    case $i in

    -d1) #10 drivers with most Nrides
        start_time=$(date +%s.%N) #start the timer
        
        echo "getting the 10 drivers with the most rides ..."

        #awk part

        cut -d';' -f1,6 "$1" |awk -F';' '!arr[$1]++ {arr2[$2]++} END {for (i in arr2) printf "%s;%d\n",i,arr2[i]}' | sort -t';' -k2nr | head > temp/d1.temp
        
        # cut the data.csv file to just keep column 1 and 6
        # takes the duplicates of rides ID away using an arr to see if the ID were encountered already
        # next, counts the occurrences of names using arr2 and separate them by ';' in the output
        # after all this, sort the output of names numerically by occurrences and from the highest to lowest
        # using ; as delimiter. To finish, take the 10 first lines of the output
        # finally it sends all in the d1.temp file in temp/

        # Gnuplot
gnuplot << EOF
        set terminal pngcairo size 800,1200 enhanced font "arial,12"
        set output 'pictures/d1_output.png'
        set ylabel "Option -d1 : Nb routes = f(Drivers)"
        set xlabel 'DRIVER NAMES' rotate by 180
        set y2label 'NB ROUTES'
        unset key
        set style data histograms
        set style histogram cluster gap 1
        set style fill solid border 
        set boxwidth 1.6
        set xtics rotate
        set y2tics rotate
        set y2range [0:250]
        unset ytics;set y2tics mirror
        set datafile separator ";"
        plot 'temp/d1.temp' using 2:xtic(1) axes x1y2 lc rgb "#78E5AE"
EOF

        convert -rotate 90 pictures/d1_output.png pictures/d1_output.png  

        end_time=$(date +%s.%N) #end the timer
        elapsed_time=$(echo "$end_time - $start_time" | bc) #calculate the difference

        echo -e "\nelapsed time for -d1: $elapsed_time seconds"
        display "pictures/d1_output.png" &> temp/display.log 
        ;;

    -d2) #10 drivers with longest distances
        start_time=$(date +%s.%N) #start the timer

        echo "getting the 10 drivers with the longest distances ..."

        #awk part

        LC_NUMERIC=en_US.UTF-8 awk -F';' 'NR>1 {sum[$6] += $5} END {for (key in sum) printf "%s;%.2f\n", key, sum[key]}' "$1" | sort -t';' -k2nr | head > temp/d2.temp

        # Gnuplot
gnuplot << EOF
        set terminal pngcairo size 800,1200 enhanced font "arial,12"
        set output 'pictures/d2_output.png'
        set ylabel "Option -d2 : Distances = f(Drivers)"
        set xlabel 'DRIVER NAMES' rotate by 180
        set y2label 'DISTANCES (x1000 km)'
        unset key
        set style data histograms
        set style histogram cluster gap 1
        set style fill solid border 
        set boxwidth 1.6
        set xtics rotate
        set y2tics rotate
        set y2range [0:160]
        unset ytics;set y2tics mirror
        set datafile separator ";"
        plot 'temp/d2.temp' using (\$2/1000):xtic(1) axes x1y2 lc rgb "#78E5AE"
EOF
        convert -rotate 90 pictures/d2_output.png pictures/d2_output.png  

        end_time=$(date +%s.%N) #end the timer
        elapsed_time=$(echo "$end_time - $start_time" | bc) #calculate the difference
        
        echo -e "\nelapsed time for -d2: $elapsed_time seconds"
        display "pictures/d2_output.png" &> temp/display.log 
        ;;

    -l) #10 longest roads
        start_time=$(date +%s.%N) #start the timer
       
        echo "getting the 10 longest rides ..."

        #awk part

        cut -d';' -f1,5 "$1" |LC_NUMERIC=en_US.UTF-8 awk -F';' '{arr[$1]+=$2} END {for (i in arr) printf "%s;%f\n", i, arr[i]}' |sort -t';' -k2,2nr |head -n 10 | sort -t';' -k1,2nr > temp/l.temp
        
        # Gnuplot
gnuplot << EOF
        set terminal pngcairo size 800,800 enhanced font "arial,12"
        set output 'pictures/l_output.png'
        set title "Option -l : Distance = f(Route)"
        set xlabel "ROUTE ID"
        set ylabel "DISTANCE (Km)"
        unset key
        set style data histograms
        set style fill solid
        set boxwidth 2.0
        set yrange [0:3000]
        set datafile separator ";"
        plot "temp/l.temp" using 2:xtic(1) lc rgb "#78E5AE" 
EOF
        end_time=$(date +%s.%N) #end the timer

        elapsed_time=$(echo "$end_time - $start_time" | bc) #calculate the difference
        echo -e "\nelapsed time for -l: $elapsed_time seconds" 
        display "pictures/l_output.png" &> temp/display.log 

        ;;

    -t) #10 most crossed towns
        start_time=$(date +%s.%N) #start the timer

        echo "getting the 10 most crossed towns ..."

        #cut part

        cut -d';' -f1,2,3,4 data/data.csv |tail -n+2 > temp/t_data.temp

        end_time=$(date +%s.%N) #end the timer
        elapsed_time1=$(echo "$end_time - $start_time" | bc) #take the first time

        #c part

        cd progc/ || exit

        make t_progc -s
        
        if [ ! -e "t_progc" ];then
        echo "C error : Error while compiling t_progc"
        exit 4
        fi

        start_time=$(date +%s.%N) #restart the timer

        ./t_progc ../temp/t_data.temp > ../temp/t.temp
        cd ..
        
        # Gnuplot
gnuplot << EOF
    set terminal pngcairo enhanced font "arial,10" size 700,700
    set output 'pictures/t_output.png'
    set title "Option -t : Nb routes = f(Towns)"
    set xlabel "TOWN NAMES"
    set ylabel "NB ROUTES"
    set xtics rotate by 45 right
    set yrange [0:4000]
    set ytics 0,500,4000
    set style data histograms
    set style histogram cluster gap 1
    set style fill solid
    set datafile separator ";"

    plot 'temp/t.temp' using 2:xtic(1) title "Total routes" lc rgb "#78E5AE", \
     '' using 3 title "First town" lc rgb "#5DCA93"
EOF
        end_time=$(date +%s.%N) #reend the timer
        elapsed_time2=$(echo "$end_time - $start_time" | bc) #take the second time

        elapsed_time=$(echo "$elapsed_time1 + $elapsed_time2" | bc) #calculate final time
        echo -e "\nelapsed time for -t: $elapsed_time seconds" 
        display "pictures/t_output.png" &> temp/display.log 

       ;;

    -s) #min, max, averrage distances
        start_time=$(date +%s.%N) #start the timer

        echo "getting the 50 longest max-min rides ..."

        #cut part

        cut -d';' -f1,5 data/data.csv |tail -n+2 > temp/s_data.temp

        end_time=$(date +%s.%N) #end the timer
        elapsed_time1=$(echo "$end_time - $start_time" | bc) #take the first time

        #c part

        cd progc/ || exit

        make s_progc -s
        
        if [ ! -e "s_progc" ];then
        echo "C error : Error while compiling s_progc"
        exit 4
        fi

        start_time=$(date +%s.%N) #restart the timer

        ./s_progc ../temp/s_data.temp | head -50 > ../temp/s.temp
        cd ..
        
        # Gnuplot
gnuplot << EOF
    set terminal pngcairo enhanced font "arial,10" size 1100,800
    set output 'pictures/s_output.png'
    set title "Option -s : Distance = f(Route)"
    set xlabel "ROUTE ID"
    set ylabel "DISTANCE (Km)"
    set xtics rotate by 45 right
    set ytics 100
    set yrange [0:1000]
    set datafile separator ";"

    plot 'temp/s.temp' using 0:3:5:xticlabels(2) with filledcurves lc rgb "#78E5AE" lt 1 title 'Distances Max (Km))', \
     '' using 0:4 with lines lc rgb "#0D9F75" title 'Distance average (Km)'
EOF
        end_time=$(date +%s.%N) #reend the timer
        elapsed_time2=$(echo "$end_time - $start_time" | bc) #take the second time

        elapsed_time=$(echo "$elapsed_time1 + $elapsed_time2" | bc) #calculate final time

        echo -e "\nelapsed time for -s: $elapsed_time seconds" 
        display "pictures/s_output.png" &> temp/display.log 
        ;;

    *) echo "arg error : $i is not an option (-h for help)" ;;

    esac
done

killall vlc &> temp/vlc.log
rm -rf temp/vlc.log
rm -rf temp/display.log