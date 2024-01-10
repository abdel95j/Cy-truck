# Cy-Truck

To array the manual you can use -h or --help options (./graph_sort -h | --help)

----------------------------------------------------
NEEDINGS

To use this programm you'll need linux or macos system.

You'll need 3 additional programs to run correctly graph_sort.sh

1 : imagemagick   2 : vlc     3 : gnuplot

      Mac : brew install imagemagick | Ubuntu : sudo apt install imagemagick

            brew install --cask vlc  |          sudo apt install vlc
      
            brew install gnuplot     |          sudo apt-get install gnuplot

----------------------------------------------------
USAGE

To execute the program you just have to type "./graph_sort.sh [FILE.csv] [OPTION]" for more precisions use -h or --help

The script will check if the temp and pictures files exist, if so temp's content will be removed. But if temp/ or pictures/ does not exist
the'll be created and given all rights to the user 

The script will cut the data.csv file in order to treat it faster. The cut result is stocked in temp/ with "?.temp" or "?_data.temp" name

In the progc directory you can use "make" command to compilate s_progc.c by using s_progc target, t_progc.c by using t_progc target
and you can also rm the executables using the clean target. (You can also use -c option for a more complete clean)
