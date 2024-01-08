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
    struct _avl * fd;
    struct _avl * fg;
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
    return (!isempty(t) && !isempty(t->fg));
}

int hasrightchild(pTree t){
    return (!isempty(t) && !isempty(t->fd));
}

void deleterightchild(pTree t){
    if(t != NULL && t->fd != NULL){
        if(t->fd->fg != NULL){
            deleteleftchild(t->fd);
        }
        if(t->fd->fd != NULL){
            deleterightchild(t->fd);
        }
        pTree tmp = t->fd;
        t->fd = NULL;
        free(tmp);
    }
}

void deleteleftchild(pTree t){
    if(t != NULL && t->fg != NULL){
        if(t->fg->fg != NULL){
            deleteleftchild(t->fg);
        }
        if(t->fg->fd != NULL){
            deleterightchild(t->fg);
        }
        pTree tmp = t->fg;
        t->fg = NULL;
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
    p->fd = NULL;
    p->fg = NULL;
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
    p->fd = NULL;
    p->fg = NULL;
    p->eq = 0;
    return p;
}

pTree rotationdroite(pTree t){
    pTree pivot;
    int eq_a,eq_p;
    pivot = t->fg;
    t->fg = pivot->fd;
    pivot->fd = t;
    eq_a = t->eq;
    eq_p = pivot->eq;
    t->eq = eq_a - min(eq_p,0)+1;
    pivot->eq = max(max(eq_a+2,eq_a+eq_p+2),eq_p+1);
    t = pivot;
    return t;
}

pTree rotationgauche(pTree t){
    pTree pivot;
    int eq_a,eq_p;
    pivot = t->fd;
    t->fd = pivot->fg;
    pivot->fg = t;
    eq_a = t->eq;
    eq_p = pivot->eq;
    t->eq = eq_a - max(eq_p,0)-1;
    pivot->eq = min(min(eq_a-2,eq_a+eq_p-2),eq_p-1);
    t = pivot;
    return t;
}

pTree doublerotationgauche(pTree t){
    t->fd = rotationdroite(t->fd);
    return rotationgauche(t);
}

pTree doublerotationdroite(pTree t){
    t->fg = rotationgauche(t->fg);
    return rotationdroite(t);
}

pTree equilibrageAVL(pTree t){
    if(t->eq >= 2){
        if(t->fd->eq >= 0){
            return rotationgauche(t);
        }
        else{
            return doublerotationgauche(t);
        }
    }
    else if(t->eq <= -2){
        if(t->fg->eq <= 0){
            return rotationdroite(t);
        }
        else{
            return doublerotationdroite(t);
        }
    }
    return t;
}

pTree insertAVL1(pTree t,int ID,float distance,int* h){
    if(t == NULL){
        *h = 1;
        return createTree1(ID,distance);
    }
    if(ID > t->key){
        t->fd = insertAVL1(t->fd,ID,distance,h);
    }
    if(ID < t->key){
        t->fg = insertAVL1(t->fg,ID,distance,h);
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
        t = equilibrageAVL(t);
        if(t->eq == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return t;
}

pTree insertAVL2(pTree t1,pTree t2,int* h){
    if(t2 == NULL){
        *h = 1;
        return createTree2(t1);
    }
    if( (t1->max - t1->min) > (t2->max - t2->min) ){
        t2->fd = insertAVL2(t1,t2->fd,h);
    }
    if( (t1->max - t1->min) < (t2->max - t2->min) ){
        t2->fg = insertAVL2(t1,t2->fg,h);
        *h = -*h;
    }
    else{
        *h = 0;
        return t2;
        }
    if(*h != 0){
        t2->eq = t2->eq + *h;
        t2 = equilibrageAVL(t2);
        if(t2->eq == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return t2;
}

void AVL1toAVL2(pTree t1,pTree* t2,int* h){
    if(t1 != NULL){
        *t2 = insertAVL2(t1,*t2,h);
    AVL1toAVL2(t1->fd,t2,h);
    AVL1toAVL2(t1->fg,t2,h);
    }
}

void deletetree(pTree t){
    deleteleftchild(t);
    deleterightchild(t);
    free(t);
}

void ecriture(pTree t){
    if(t == NULL){
        printf("empty tree\n");
    }
    else{
        if(hasrightchild(t)){
            ecriture(t->fd);
        }
        printf("%d;%f;%f;%f\n",t->key,t->min,t->D_summ/(t->N_steps *1.0),t->max);
        if(hasleftchild(t)){
            ecriture(t->fg);
        }
    }
}

int main(int argc, char **argv) {
    //assert(argc==2);
    //open file and check if it is okay
    FILE* file = fopen(argv[1], "r");
    assert(file!=NULL);
    
    int h = 10;
    int ID = -1;
    float distance = -1;
    pTree AVL1 = NULL;
    pTree AVL2 = NULL;
    
    while(fscanf(file,"%d;%f",&ID,&distance) != EOF){
        AVL1 = insertAVL1(AVL1,ID,distance,&h);
    }
    AVL1toAVL2(AVL1,&AVL2,&h);
    ecriture(AVL2);
    deletetree(AVL2);
    deletetree(AVL1);

    fclose(file);


    return 0; 
}