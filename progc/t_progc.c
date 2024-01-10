#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//Defining struct to stock ID in avl
typedef struct _avlID{
    int ID;
    int balance;
    struct _avlID* rc;
    struct _avlID* lc;
}TreeID;

//Defining struct avl
typedef struct _avl
{
    char city[51];
    int sum;
    int first_sum;
    TreeID* roads;
    struct _avl * rc;
    struct _avl * lc;
    int balance;
}Tree;

//Defining aliases to use pointers easily
typedef TreeID* pTreeID;
typedef Tree* pTree;

//calling function prototypes to use them before
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

//copy an arr from source to destination
void copyarr(char source[], char destination[], int size) {
    for (int i = 0; i < size; i++) {
        destination[i] = source[i];
    }
}

int isemptyID(pTreeID t){
    return (t == NULL);
}

int hasleftchildID(pTreeID t){
    return (!isemptyID(t) && !isemptyID(t->lc));
}

int hasrightchildID(pTreeID t){
    return (!isemptyID(t) && !isemptyID(t->rc));
}

void deleterightchildID(pTreeID t){
    if(t != NULL && t->rc != NULL){
        if(t->rc->lc != NULL){
            deleteleftchildID(t->rc);
        }
        if(t->rc->rc != NULL){
            deleterightchildID(t->rc);
        }
        pTreeID tmp = t->rc;
        t->rc = NULL;
        free(tmp);
    }
}

void deleteleftchildID(pTreeID t){
    if(t != NULL && t->lc != NULL){
        if(t->lc->lc != NULL){
            deleteleftchildID(t->lc);
        }
        if(t->lc->rc != NULL){
            deleterightchildID(t->lc);
        }
        pTreeID tmp = t->lc;
        t->lc = NULL;
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
        deletetreeID(t->rc->roads);
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
        deletetreeID(t->lc->roads);
        t->lc = NULL;
        free(tmp);
    }
}

void deletetree(pTree t){
    deleteleftchild(t);
    deleterightchild(t);
    deletetreeID(t->roads);
    free(t);
}

pTree righturn(pTree t){
    pTree pivot;
    int eq_a,eq_p;
    pivot = t->lc;
    t->lc = pivot->rc;
    pivot->rc = t;
    eq_a = t->balance;
    eq_p = pivot->balance;
    t->balance = eq_a - min(eq_p,0)+1;
    pivot->balance = max(max(eq_a+2,eq_a+eq_p+2),eq_p+1);
    t = pivot;
    return t;
}

pTree leftturn(pTree t){
    pTree pivot;
    int eq_a,eq_p;
    pivot = t->rc;
    t->rc = pivot->lc;
    pivot->lc = t;
    eq_a = t->balance;
    eq_p = pivot->balance;
    t->balance = eq_a - max(eq_p,0)-1;
    pivot->balance = min(min(eq_a-2,eq_a+eq_p-2),eq_p-1);
    t = pivot;
    return t;
}

pTree right_left(pTree t){
    t->rc = righturn(t->rc);
    return leftturn(t);
}

pTree left_right(pTree t){
    t->lc = leftturn(t->lc);
    return righturn(t);
}

pTree balanceAVL(pTree t){
    if(t->balance >= 2){
        if(t->rc->balance >= 0){
            return leftturn(t);
        }
        else{
            return right_left(t);
        }
    }
    else if(t->balance <= -2){
        if(t->lc->balance <= 0){
            return righturn(t);
        }
        else{
            return left_right(t);
        }
    }
    return t;
}

pTreeID rightturnI(pTreeID t){
    pTreeID pivot;
    int eq_a,eq_p;
    pivot = t->lc;
    t->lc = pivot->rc;
    pivot->rc = t;
    eq_a = t->balance;
    eq_p = pivot->balance;
    t->balance = eq_a - min(eq_p,0)+1;
    pivot->balance = max(max(eq_a+2,eq_a+eq_p+2),eq_p+1);
    t = pivot;
    return t;
}

pTreeID leftturnID(pTreeID t){
    pTreeID pivot;
    int eq_a,eq_p;
    pivot = t->rc;
    t->rc = pivot->lc;
    pivot->lc = t;
    eq_a = t->balance;
    eq_p = pivot->balance;
    t->balance = eq_a - max(eq_p,0)-1;
    pivot->balance = min(min(eq_a-2,eq_a+eq_p-2),eq_p-1);
    t = pivot;
    return t;
}

pTreeID right_leftID(pTreeID t){
    t->rc = rightturnI(t->rc);
    return leftturnID(t);
}

pTreeID left_rightID(pTreeID t){
    t->lc = leftturnID(t->lc);
    return rightturnI(t);
}

pTreeID balanceAVLID(pTreeID t){
    if(t->balance >= 2){
        if(t->rc->balance >= 0){
            return leftturnID(t);
        }
        else{
            return right_leftID(t);
        }
    }
    else if(t->balance <= -2){
        if(t->lc->balance <= 0){
            return rightturnI(t);
        }
        else{
            return left_rightID(t);
        }
    }
    return t;
}

// function that check if a city is a starting town / returns a 1 if true else 0.
int researchABRID(pTreeID t,int ID){
    if(t == NULL){
        return 0;
    }
    if(ID > t->ID){
        return researchABRID(t->rc ,ID);
    }
    if(ID < t->ID){
        return researchABRID(t->lc ,ID);
    }
    return 1;
}

//function createTree for the id avl
pTreeID createTreeID(int ID){
    pTreeID p = malloc(sizeof(TreeID));
    assert(p != NULL);
    p->ID = ID;
    p->rc = NULL;
    p->lc = NULL;
    p->balance = 0;
    return p;
}

//function insertTree for the id avl (with sort by ID)
pTreeID insertTreeID(pTreeID t,int ID,int* h){
    if(t == NULL){
        *h = 1;
        return createTreeID(ID);
    }
    if(ID > t->ID){
        t->rc = insertTreeID(t->rc,ID,h);
    }
    if(ID < t->ID){
        t->lc = insertTreeID(t->lc,ID,h);
        *h = -*h;
    }
    if(ID == t->ID){
        *h = 0;
        return t;
        }
    if(*h != 0){
        t->balance = t->balance + *h;
        t = balanceAVLID(t);
        if(t->balance == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return t;
}

// createTree function for the first avl 
pTree createTree1(char town[],int ID,int step,int pos,int* h){
    pTree p = malloc(sizeof(Tree));
    assert(p != NULL);
    assert(town != NULL);
    copyarr(town,p->city,51);
    p->roads = NULL;
    p->roads = insertTreeID(p->roads,ID,h);
    if(step == 1 && pos == 1){
        p->first_sum = 1;
    }
    else{
        p->first_sum = 0;
    }
    p->sum = 1;
    p->rc = NULL;
    p->lc = NULL;
    p->balance = 0;
    return p;
}

pTree createTree2(pTree t){
    assert(t != NULL);
    pTree p = malloc(sizeof(Tree));
    assert(p != NULL);
    copyarr(t->city,p->city,51);
    p->first_sum = t->first_sum;
    p->sum = t->sum;
    p->roads = NULL;
    p->rc = NULL;
    p->lc = NULL;
    p->balance = 0;
    return p;
}

//function insertTree for avl1 with alphabetical sort (by town)
pTree insertAVL1(pTree t,int ID,int step,char* town,int pos,int* h1,int* h2){
    if(t == NULL){
        *h1 = 1;
        return createTree1(town,ID,step,pos,h2);
    }
    int cmp = strcmp(town,t->city);
    if(cmp > 0){
        t->rc = insertAVL1(t->rc,ID,step,town,pos,h1,h2);
    }
    if(cmp < 0){
        t->lc = insertAVL1(t->lc,ID,step,town,pos,h1,h2);
        *h1 = -*h1;
    }
    if(cmp == 0){
        // if it's a starting town and it's a "(Town A) == 1"
        if(step == 1 && pos ==1){
            t->first_sum = t->first_sum + 1;
        }
        // if researchABRID == 0 it means that it's a starting town
        if(researchABRID(t->roads,ID) == 0){
            t->sum = t->sum + 1;
            t->roads = insertTreeID(t->roads,ID,h2);
        }
        *h1 = 0;
        return t;
        }
    if(*h1 != 0){
        t->balance = t->balance + *h1;
        t = balanceAVL(t);
        if(t->balance == 0){
            *h1 = 0;
        }
        else{
            *h1 = 1;
        }
    }
    return t;
}

//function insertTree for the avl2 with sort by crossed count
pTree insertAVL2(pTree t1,pTree t2,int* h){
    if(t2 == NULL){
        *h = 1;
        return createTree2(t1);
    }
    if(t1->sum > t2->sum){
        t2->rc = insertAVL2(t1,t2->rc,h);
    }
    if(t1->sum < t2->sum){
        t2->lc = insertAVL2(t1,t2->lc,h);
        *h = -*h;
    }
    else{
        *h = 0;
        return t2;
        }
    if(*h != 0){
        t2->balance = t2->balance + *h;
        t2 = balanceAVL(t2);
        if(t2->balance == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return t2;
}

//function insertTree for the avl3 sort by towns alphabetically
pTree insertAVL3(pTree t1,pTree t2,int* h){
    if(t2 == NULL){
        *h = 1;
        return createTree2(t1);
    }
    int cmp = strcmp(t1->city,t2->city);
    if(cmp > 0){
        t2->rc = insertAVL3(t1,t2->rc,h);
    }
    if(cmp < 0){
        t2->lc = insertAVL3(t1,t2->lc,h);
        *h = -*h;
    }
    else{
        *h = 0;
        return t2;
    }
    if(*h != 0){
        t2->balance = t2->balance + *h;
        t2 = balanceAVL(t2);
        if(t2->balance == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return t2;
}

//Convert AVL1 (avl with all the first datas) to AVL2 (avl with all the towns, their crossed count and their "first town of traject" count)
void AVL1toAVL2(pTree t1,pTree* t2,int* h){
    if(t1 != NULL){
        *t2 = insertAVL2(t1,*t2,h);
        AVL1toAVL2(t1->rc,t2,h);
        AVL1toAVL2(t1->lc,t2,h);
    }
}

void arrAVL(pTree t){
    if(t == NULL){
        printf("empty tree\n");
    }
    else{
        if(hasleftchild(t)){
            arrAVL(t->lc);
        }
        
        printf("%s;%d;%d\n",t->city,t->sum,t->first_sum);
        
        if(hasrightchild(t)){
            arrAVL(t->rc);
        }
    }
}

//Convert AVL2 (avl with all the towns, their crossed count and their "first town of traject" count) to AVL3 (AVL with the 10 most crossed towns sorted alphabetically) 
void AVL2toAVL3(pTree t2,pTree* t3,int* h,int* count){
    assert(t2 != NULL);
    if(hasrightchild(t2) && *count < 10){
        AVL2toAVL3(t2->rc,t3,h,count);
    }
    if(*count < 10){
        *t3 = insertAVL3(t2,*t3,h);
        *count = *count + 1;
    }
    if(hasleftchild(t2) && *count < 10){
        AVL2toAVL3(t2->lc,t3,h,count);
    }
}

int main(int argc, char **argv) {
    assert(argc==2);
    //open file and check if it did well
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
    arrAVL(AVL3);
    deletetree(AVL2);
    deletetree(AVL1);
    deletetree(AVL3);

    fclose(file);

    return 0; 
}