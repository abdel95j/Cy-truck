#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct _avl
{
    int key;
    int max;
    int min;
    float D_summ;
    int N_steps;
    struct _avl* fd;
    struct _avl* fg;
}avl;

float ret_max(float a,float b)
{
    return (a>b) ? a:b;
}

float ret_min(float a,float b)
{
    return (a<b) ? a:b;
}

int main(int argc, char **argv) {
    assert(argc==2);
    //open file and check if it is okay
    FILE* file = fopen(argv[1], "r");
    assert(file!=NULL);
    
    int ID = -1;
    float distance = -1;
    
    while(fscanf(file,"%d;%f",&ID,&distance) != EOF){
        printf("%d ; %f\n",ID,distance);
    }

    fclose(file);

    return 0; 
}