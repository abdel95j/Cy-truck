#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct _avl
{
    int key;
    float max;
    float min;
    float D_summ;
    int N_steps;
    struct _avl * rc;
    struct _avl * lc;
    int eq;
}Tree;

typedef Tree* pTree;
void deleteleftchild(pTree t);
void deleterightchild(pTree t);

float max(float a,float b)
{
    return (a>b) ? a:b;
}

float min(float a,float b)
{
    return (a<b) ? a:b;
}

int isempty(pTree t){
    return (t == NULL);
}

int hasleftchild(pTree t){
    return (!isempty(t) && !isempty(t->lc));
}

int hasrightchild(pTree t){
    return (!isempty(t) && !isempty(t->rc));
}

void deleterightchild(pTree t){
    if(t != NULL && t->rc != NULL){
        if(t->rc->lc != NULL){
            deleteleftchild(t->rc);
        }
        if(t->rc->rc != NULL){
            deleterightchild(t->rc);
        }
        pTree tmp = t->rc;
        t->rc = NULL;
        free(tmp);
    }
}

void deleteleftchild(pTree t){
    if(t != NULL && t->lc != NULL){
        if(t->lc->lc != NULL){
            deleteleftchild(t->lc);
        }
        if(t->lc->rc != NULL){
            deleterightchild(t->lc);
        }
        pTree tmp = t->lc;
        t->lc = NULL;
        free(tmp);
    }
}

pTree createTree1(int ID,float distance){
    pTree p = malloc(sizeof(Tree));
    assert(p != NULL);
    p->key = ID;
    p->max = distance;
    p->min = distance;
    p->D_summ = distance;
    p->N_steps = 1;
    p->rc = NULL;
    p->lc = NULL;
    p->eq = 0;
    return p;
}

pTree createTree2(pTree t){
    assert(t != NULL);
    pTree p = malloc(sizeof(Tree));
    assert(p != NULL);
    p->key = t->key;
    p->max = t->max;
    p->min = t->min;
    p->D_summ = t->D_summ;
    p->N_steps = t->N_steps;
    p->rc = NULL;
    p->lc = NULL;
    p->eq = 0;
    return p;
}

pTree rigtturn(pTree t){
    pTree pivot;
    int eq_a,eq_p;
    pivot = t->lc;
    t->lc = pivot->rc;
    pivot->rc = t;
    eq_a = t->eq;
    eq_p = pivot->eq;
    t->eq = eq_a - min(eq_p,0)+1;
    pivot->eq = max(max(eq_a+2,eq_a+eq_p+2),eq_p+1);
    t = pivot;
    return t;
}

pTree leftturn(pTree t){
    pTree pivot;
    int eq_a,eq_p;
    pivot = t->rc;
    t->rc = pivot->lc;
    pivot->lc = t;
    eq_a = t->eq;
    eq_p = pivot->eq;
    t->eq = eq_a - max(eq_p,0)-1;
    pivot->eq = min(min(eq_a-2,eq_a+eq_p-2),eq_p-1);
    t = pivot;
    return t;
}

pTree right_left(pTree t){
    t->rc = rigtturn(t->rc);
    return leftturn(t);
}

pTree left_right(pTree t){
    t->lc = leftturn(t->lc);
    return rigtturn(t);
}

pTree balanceAVL(pTree t){
    if(t->eq >= 2){
        if(t->rc->eq >= 0){
            return leftturn(t);
        }
        else{
            return right_left(t);
        }
    }
    else if(t->eq <= -2){
        if(t->lc->eq <= 0){
            return rigtturn(t);
        }
        else{
            return left_right(t);
        }
    }
    return t;
}

// function insertTree for the avl1 sort (by id without duplicates and refresh max, min, averrage [D_summ/N_steps] distances)
pTree insertAVL1(pTree t,int ID,float distance,int* h){
    if(t == NULL){
        *h = 1;
        return createTree1(ID,distance);
    }
    if(ID > t->key){
        t->rc = insertAVL1(t->rc,ID,distance,h);
    }
    if(ID < t->key){
        t->lc = insertAVL1(t->lc,ID,distance,h);
        *h = -*h;
    }
    if(ID == t->key){
        t->D_summ = t->D_summ + distance;
        t->max = max(t->max, distance);
        t->min = min(t->min, distance);
        t->N_steps = t->N_steps + 1;
        *h = 0;
        return t;
        }
    if(*h != 0){
        t->eq = t->eq + *h;
        t = balanceAVL(t);
        if(t->eq == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return t;
}


//function insertTree for the avl2 with sort by (max distance - min distance)
pTree insertAVL2(pTree t1,pTree t2,int* h){
    if(t2 == NULL){
        *h = 1;
        return createTree2(t1);
    }
    if( (t1->max - t1->min) > (t2->max - t2->min) ){
        t2->rc = insertAVL2(t1,t2->rc,h);
    }
    if( (t1->max - t1->min) < (t2->max - t2->min) ){
        t2->lc = insertAVL2(t1,t2->lc,h);
        *h = -*h;
    }
    else{
        *h = 0;
        return t2;
        }
    if(*h != 0){
        t2->eq = t2->eq + *h;
        t2 = balanceAVL(t2);
        if(t2->eq == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return t2;
}

//functiuon convert avl1 (avl with sorted by id with max,min and averrage distances) to avl2 (same but sorted by (max - min))
void AVL1toAVL2(pTree t1,pTree* t2,int* h){
    if(t1 != NULL){
        *t2 = insertAVL2(t1,*t2,h);
    AVL1toAVL2(t1->rc,t2,h);
    AVL1toAVL2(t1->lc,t2,h);
    }
}

void deletetree(pTree t){
    deleteleftchild(t);
    deleterightchild(t);
    free(t);
}

void arrAVL(pTree t, int* putone){
    if(t == NULL){
        printf("empty tree\n");
    }
    else{
        if(hasrightchild(t)){
            arrAVL(t->rc,putone);
        }
        printf("%d;%d;%f;%f;%f;%f\n",*putone,t->key,t->min,t->D_summ/(t->N_steps *1.0),t->max,t->max-t->min);
        *putone=+1;
        if(hasleftchild(t)){
            arrAVL(t->lc,putone);
        }
    }
}

int main(int argc, char **argv) {
    assert(argc==2);
    //open file and check if it is okay
    FILE* file = fopen(argv[1], "r");
    assert(file!=NULL);
    
    int h = 10;
    int ID = -1;
    int putone = 1;
    float distance = -1;
    pTree AVL1 = NULL;
    pTree AVL2 = NULL;
    
    while(fscanf(file,"%d;%f",&ID,&distance) != EOF){ //reads file and insert in avl1
        AVL1 = insertAVL1(AVL1,ID,distance,&h);
    }
    AVL1toAVL2(AVL1,&AVL2,&h);
    arrAVL(AVL2, &putone);

    //free the memory 
    deletetree(AVL2);
    deletetree(AVL1);
    fclose(file);

    return 0; 
}