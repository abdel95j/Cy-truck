#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// cut route id et distance (cut -d';' -f1,5 data/data.csv |tail -n+2 > temp/s_data.temp)

typedef struct _arr
{
    int key;
    int max;
    int min;
    float D_summ;
    int N_steps;
    struct _arr* next;
}arr;

typedef struct _stock
{
    int id;
    float distance;
    struct _stock* next;
}stock;

stock* createStock(int road_ID, float dist)
{
    stock* temp = malloc(sizeof(stock));
    assert(temp!=NULL);

    temp->distance=dist;
    temp->id=road_ID;
    temp->next=NULL;

    return temp;
}

stock* addStock(stock* data, int road_ID, float dist)
{
    if (data == NULL)
    {
        data = createStock(road_ID,dist);
        return data;
    }

    data->next=createStock(road_ID,dist);
    return data;
}

int ret_max(int a,int b)
{
    return (a>b) ? a:b;
}

int ret_min(int a,int b)
{
    return (a<b) ? a:b;
}

int main(int argc, char **argv) {

    //open file and check if it is okay
    FILE* file = fopen(argv[1], "r");
    assert(file!=NULL);

    //create nodelist
    stock* data = NULL;
    
    //create recup vars 
    float dist=-1;
    int road_ID;

    //first fscanf to get the first line and put head on the stock created
    fscanf(file, "%d;%f", &road_ID, &dist);
    data = addStock(data, road_ID, dist);
    stock* head = data;

    //reads all lines and take the id and distance and put it in nodelist data
    while (fscanf(file, "%d;%f", &road_ID, &dist) != EOF)
    {
        data = addStock(data, road_ID, dist);
        data=data->next;
    }

    //reset data to its begining
    data = head;

    while (data!=NULL)
    {
        printf("%d / %f\n",data->id, data->distance);
        data=data->next;
    }
        
    fclose(file);

    return 0; 
}