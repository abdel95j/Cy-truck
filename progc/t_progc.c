#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct _avlID{
    int ID;
    int eq;
    struct _avlID* fd;
    struct _avlID* fg;
}TreeID;

typedef struct _avl
{
    char city[51];
    int sum;
    int first_sum;
    TreeID* routes;
    struct _avl * fd;
    struct _avl * fg;
    int eq;
}Tree;

typedef TreeID* pTreeID;
typedef Tree* pTree;

void deleteleftchildID(pTreeID t);
void deleterightchildID(pTreeID t);
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

void copyarr(char source[], char destination[], int taille) {
    for (int i = 0; i < taille; i++) {
        destination[i] = source[i];
    }
}

int isemptyID(pTreeID t){
    return (t == NULL);
}

int hasleftchildID(pTreeID t){
    return (!isemptyID(t) && !isemptyID(t->fg));
}

int hasrightchildID(pTreeID t){
    return (!isemptyID(t) && !isemptyID(t->fd));
}

void deleterightchildID(pTreeID t){
    if(t != NULL && t->fd != NULL){
        if(t->fd->fg != NULL){
            deleteleftchildID(t->fd);
        }
        if(t->fd->fd != NULL){
            deleterightchildID(t->fd);
        }
        pTreeID tmp = t->fd;
        t->fd = NULL;
        free(tmp);
    }
}

void deleteleftchildID(pTreeID t){
    if(t != NULL && t->fg != NULL){
        if(t->fg->fg != NULL){
            deleteleftchildID(t->fg);
        }
        if(t->fg->fd != NULL){
            deleterightchildID(t->fg);
        }
        pTreeID tmp = t->fg;
        t->fg = NULL;
        free(tmp);
    }
}

void deletetreeID(pTreeID t){
    deleteleftchildID(t);
    deleterightchildID(t);
    free(t);
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
        deletetreeID(t->fd->routes);
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
        deletetreeID(t->fg->routes);
        t->fg = NULL;
        free(tmp);
    }
}

void deletetree(pTree t){
    deleteleftchild(t);
    deleterightchild(t);
    free(t);
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

pTreeID rotationdroiteID(pTreeID t){
    pTreeID pivot;
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

pTreeID rotationgaucheID(pTreeID t){
    pTreeID pivot;
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

pTreeID doublerotationgaucheID(pTreeID t){
    t->fd = rotationdroiteID(t->fd);
    return rotationgaucheID(t);
}

pTreeID doublerotationdroiteID(pTreeID t){
    t->fg = rotationgaucheID(t->fg);
    return rotationdroiteID(t);
}

pTreeID equilibrageAVLID(pTreeID t){
    if(t->eq >= 2){
        if(t->fd->eq >= 0){
            return rotationgaucheID(t);
        }
        else{
            return doublerotationgaucheID(t);
        }
    }
    else if(t->eq <= -2){
        if(t->fg->eq <= 0){
            return rotationdroiteID(t);
        }
        else{
            return doublerotationdroiteID(t);
        }
    }
    return t;
}

int researchABRID(pTreeID t,int ID){
    if(t == NULL){
        return 0;
    }
    if(ID > t->ID){
        return researchABRID(t->fd ,ID);
    }
    if(ID < t->ID){
        return researchABRID(t->fg ,ID);
    }
    return 1;
}

pTreeID createTreeID(int ID){
    pTreeID p = malloc(sizeof(TreeID));
    assert(p != NULL);
    p->ID = ID;
    p->fd = NULL;
    p->fg = NULL;
    p->eq = 0;
    return p;
}

pTreeID insertTreeID(pTreeID t,int ID,int* h){
    if(t == NULL){
        *h = 1;
        return createTreeID(ID);
    }
    if(ID > t->ID){
        t->fd = insertTreeID(t->fd,ID,h);
    }
    if(ID < t->ID){
        t->fg = insertTreeID(t->fg,ID,h);
        *h = -*h;
    }
    if(ID == t->ID){
        *h = 0;
        return t;
        }
    if(*h != 0){
        t->eq = t->eq + *h;
        t = equilibrageAVLID(t);
        if(t->eq == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return t;
}

pTree createTree1(char tab[],int ID,int step,int pos,int* h){
    pTree p = malloc(sizeof(Tree));
    assert(p != NULL);
    assert(tab != NULL);
    copyarr(tab,p->city,51);
    p->routes = NULL;
    p->routes = insertTreeID(p->routes,ID,h);
    if(step == 1 && pos == 1){
        p->first_sum = 1;
    }
    else{
        p->first_sum = 0;
    }
    p->sum = 1;
    p->fd = NULL;
    p->fg = NULL;
    p->eq = 0;
    return p;
}

pTree createTree2(pTree t){
    assert(t != NULL);
    pTree p = malloc(sizeof(Tree));
    assert(p != NULL);
    copyarr(t->city,p->city,51);
    p->first_sum = t->first_sum;
    p->sum = t->sum;
    p->routes = NULL;
    p->fd = NULL;
    p->fg = NULL;
    p->eq = 0;
    return p;
}

pTree insertAVL1(pTree t,int ID,int step,char* town,int pos,int* h1,int* h2){
    if(t == NULL){
        *h1 = 1;
        return createTree1(town,ID,step,pos,h2);
    }
    int cmp = strcmp(town,t->city);
    if(cmp > 0){
        t->fd = insertAVL1(t->fd,ID,step,town,pos,h1,h2);
    }
    if(cmp < 0){
        t->fg = insertAVL1(t->fg,ID,step,town,pos,h1,h2);
        *h1 = -*h1;
    }
    if(cmp == 0){
        if(step == 1 && pos ==1){
            t->first_sum = t->first_sum + 1;
        }
        if(researchABRID(t->routes,ID) == 0){
            t->sum = t->sum + 1;
            t->routes = insertTreeID(t->routes,ID,h2);
        }
        *h1 = 0;
        return t;
        }
    if(*h1 != 0){
        t->eq = t->eq + *h1;
        t = equilibrageAVL(t);
        if(t->eq == 0){
            *h1 = 0;
        }
        else{
            *h1 = 1;
        }
    }
    return t;
}

pTree insertAVL2(pTree t1,pTree t2,int* h){
    if(t2 == NULL){
        *h = 1;
        return createTree2(t1);
    }
    if(t1->sum > t2->sum){
        t2->fd = insertAVL2(t1,t2->fd,h);
    }
    if(t1->sum < t2->sum){
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

pTree insertAVL3(pTree t1,pTree t2,int* h){
    if(t2 == NULL){
        *h = 1;
        return createTree2(t1);
    }
    int cmp = strcmp(t1->city,t2->city);
    if(cmp > 0){
        t2->fd = insertAVL3(t1,t2->fd,h);
    }
    if(cmp < 0){
        t2->fg = insertAVL3(t1,t2->fg,h);
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

void ecritureID(pTreeID t){
    if(t == NULL){
        printf("empty tree\n");
    }
    else{
        if(hasrightchildID(t)){
            ecritureID(t->fd);
        }
        printf("%d-",t->ID);
        if(hasleftchildID(t)){
            ecritureID(t->fg);
        }
    }
}

void ecriture(pTree t){
    if(t == NULL){
        printf("empty tree\n");
    }
    else{
        if(hasleftchild(t)){
            ecriture(t->fg);
        }
        
        printf("%s;%d;%d\n",t->city,t->sum,t->first_sum);
        
        if(hasrightchild(t)){
            ecriture(t->fd);
        }
    }
}

void AVL2toAVL3(pTree t2,pTree* t3,int* h,int* count){
    assert(t2 != NULL);
    if(hasrightchild(t2) && *count < 10){
        AVL2toAVL3(t2->fd,t3,h,count);
    }
    if(*count < 10){
        *t3 = insertAVL3(t2,*t3,h);
        *count = *count + 1;
    }
    if(hasleftchild(t2) && *count < 10){
        AVL2toAVL3(t2->fg,t3,h,count);
    }
}

int main(int argc, char **argv) {
    assert(argc==2);
    //open file and check if it is okay
    FILE* file = fopen(argv[1], "r");
    assert(file!=NULL);
    
    int h1 = 10;
    int h2 = 10;
    int ID = -1;
    int step = -1;
    int count = 0;
    char townA[51];
    char townB[51];
    pTree AVL1 = NULL;
    pTree AVL2 = NULL;
    pTree AVL3 = NULL;
    
    while(fscanf(file,"%d;%d;%50[^;];%50[^;\n]",&ID,&step,townA,townB) != EOF){
        AVL1 = insertAVL1(AVL1,ID,step,townA,1,&h1,&h2);
        AVL1 = insertAVL1(AVL1,ID,step,townB,2,&h1,&h2);
    }
    AVL1toAVL2(AVL1,&AVL2,&h1);
    AVL2toAVL3(AVL2,&AVL3,&h2,&count);
    ecriture(AVL3);
    deletetree(AVL2);
    deletetree(AVL1);
    deletetree(AVL3);

    fclose(file);

    return 0; 
}