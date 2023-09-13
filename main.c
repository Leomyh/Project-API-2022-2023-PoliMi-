#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//test out-wrong
enum{V=20};
enum{N=4096};

struct dictionary { //tree for dictionary
    char*str;
    struct dictionary* R;
    struct dictionary* L;
    struct dictionary *link;
};

struct miniTree { //tree for dictionary
    char*str;
    struct miniTree* F;
    struct miniTree* R;
    struct miniTree* L;
};

struct nanoTree { //tree for character
    char c;
    struct nanoTree*R;
    struct nanoTree*L;
};

//global variable
struct dictionary* root = NULL;//principle tree
struct miniTree* curTree=NULL; //secondary tree
struct dictionary*Head_list=NULL; //pt list
int k; //max length k
int remain_space=N;
int ascII1_4[78];
int ascII5[78];
//function declaration
struct dictionary* create_Node(char s[], struct dictionary*new);
struct dictionary* inVocab(struct dictionary* curr, struct dictionary* new); //insert dictionary
char findVocab(struct dictionary *curr, char s[]); //findVocab in static tree
void stamp_Vocab(struct dictionary* curr); //print VocabTree

struct miniTree* reuse_Node(char s[]);
struct miniTree* inNode(struct miniTree* curr, struct miniTree* new);
struct miniTree *delete_Node(struct miniTree *curr); //delete dynamic tree
struct miniTree*successor_Node(struct miniTree*curr);
struct miniTree*min_Node(struct miniTree*curr);
void stamp_Tree(struct miniTree* curr);

char vinCheck(char v2[], struct nanoTree *v3[20], char present[], char word[]); //check vin
char search(struct nanoTree *l, char s);//search single char tree
struct nanoTree* inChar(struct nanoTree*curr, char c); //insert single char tree
void create_miniTree(char v2[], struct nanoTree *v3[20], char present[], int numNode);
void destroy_miniTree(struct miniTree*curr);
int quick_cmp( const char *s1, const char *s2);

int main(){

    char *tmp=malloc(V*sizeof(char));
    if(fgets(tmp,V,stdin)==0) return 0;

    k=atoi(tmp);

    char voc[k+1]; //vector for receive input

    struct dictionary*new=malloc(N * (sizeof(struct dictionary) + sizeof(char) * (k + 2))); //batch allocation initial

    if(fgets(voc, V, stdin) == 0) return 0;

    while (voc[0] != '+') //dictionary initial
    {
        if(remain_space == 0)
        {
            remain_space=N;
            new=malloc(N*(sizeof(struct dictionary) + sizeof(char) * (k + 2)));
        }
        root= inVocab(root, create_Node(voc, new));
        remain_space--;

        if(fgets(voc, V, stdin) == 0) return 0;
    }
    char *v2 = malloc(k* sizeof(char)); // exact pos vin2 (vector pointer)
    struct nanoTree*v3[k]; //wrong pos vin3 (batch tree)
    char *present = malloc(k*sizeof(char)); //must present ascII1_4 (vector pointer)

    char *ris = malloc((k+1) * sizeof(char)); // dictionary ris[]
    int i,j,p,g; //(counter variable)
//begin program>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    do {

        //insert out of game
        if (voc[0]=='+'&&voc[1] == 'i')
        {
            if(fgets(voc, V, stdin) == 0) return 0;

            do {
                if(remain_space == 0)
                {
                    remain_space=N;
                    new=malloc(N*(sizeof(struct dictionary) + sizeof(char) * (k + 2)));
                }
                root= inVocab(root, create_Node(voc, new));
                remain_space--;
                if (fgets(voc, V, stdin) == 0) return 0;
            }while (voc[0] != '+');

            if(fgets(voc, V, stdin) == 0) return 0;

        }

        while (voc[0] != '+') //initial dictionary insert continue
        {
            if(remain_space == 0)
            {
                remain_space=N;
                new=malloc(N*(sizeof(struct dictionary) + sizeof(char) * (k + 2)));
            }
            root= inVocab(root, create_Node(voc, new));
            remain_space--;

            if(fgets(voc, V, stdin) == 0) return 0;
        }
//start new game >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        if (voc[0]=='+'&&voc[1] == 'n')
        {
            //reset all vin
            memset(v2,0,k*sizeof(char));
            memset(v3,0,k*sizeof(struct nanoTree*));
            memset(present,0,k*sizeof(char));
            memset(ascII1_4, 0, 78 * sizeof(int));
            memset(ascII5, 0, 78 * sizeof(int));

            int ToK = 0; //counter of '+'

            char ref[V]; //refer word: ref
            if(fgets(ref, V, stdin) == 0) return 0;

            if(fgets(tmp, V, stdin)==0) return 0;
            int chance=atoi(tmp); //chance possible
            //choice option: 1. +stamp;
            //               2. +insert;
            //               3. input : s

            while (chance != 0) {

                if (fgets(voc, V, stdin) == 0) return 0; //input the dictionary

                if(voc[0] != '+')
                {
//choice 1. input: s >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//present ascII1_4 ->> elaborate and begin to filter>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                    if (findVocab(root, voc) == 'v')
                    {
                        chance--;
                        ToK=0;
                        //elaborate for out ris
                        for (i = 0; i < k; i++) //search '+' and '/'
                        {
                            if (ref[i] == voc[i])
                            {
                                ris[i] = '+';
                                v2[i]=voc[i]; //fill in v2
                                ToK++; //num of '+' increment
                            } else
                                ris[i] = '/';
                        }

                        if (ToK != k) //if '+' is different by length dictionary
                        {

                            for (i = 0; i < k; i++)//search '|'
                            {
                                for (j = 0; j < k&&ris[i]!='+'; j++)
                                {
                                    if ((ref[i] == voc[j]) && (ris[j] == '/'))
                                    {
                                        ris[j] = '|';
                                        break;
                                    }
                                }
                            }
                            //output >> ris
                            ris[k]='\0';
                            printf("%s\n", ris);

                            //build  vinCheck
                            //case A: at least one '/'
                            for (i = 0; i < k; i++) //run and control ris[]
                            {
                                if(ris[i]=='/')
                                {
                                    ris[i]=0; //cancel
                                    g = 0; //prepare count number
                                    for(j=0;j<k;j++) //check array ris for first position
                                    {
                                        if(i==j||ris[j]==0)
                                            continue;
                                        if(voc[i] == voc[j]) //findVocab al least two same char
                                        {
                                            if (ris[j] =='|') //findVocab char which have '|' so begin count num in v5 and case particular in v3
                                            {
                                                if (v2[j] == 0) //if not occupation by '+'
                                                    v3[j]= inChar(v3[j], voc[j]);

                                                if (v2[i] == 0) //same thing to case particular in v3
                                                    v3[i]= inChar(v3[i], voc[j]);

                                                g++; //increase counter num exact
                                            } else if (ris[j] == '+')
                                                g++;//increase counter num exact

                                            ris[j] = 0; //cancel
                                        }
                                    }
                                    if(g==0)//insert v1
                                        ascII1_4[voc[i] - 45]=-1;
                                    else //if g!=0 action blocked v5
                                    {
                                        if(ascII5[voc[i] - 45] == 0) {
                                            ascII5[voc[i] - 45] = g;
                                            ascII1_4[voc[i] - 45] = -2;
                                            for(j=0;j<k;j++)
                                            {
                                                if(present[j]==voc[i])
                                                    break;
                                                else if(present[j]==0)
                                                {
                                                    present[j]=voc[i];
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            //case B: no one '/'
                            for (i = 0; i < k; i++) //run and control ris[]
                            {
                                if(ris[i]!=0) //if present '+' or '|'
                                {
                                    g=0;
                                    for(j=i;j<k;j++) //check array ris
                                    {
                                        if(ris[j]==0)
                                            continue;
                                        if (voc[i] == voc[j]) //findVocab al least two same char and differ by 0
                                        {
                                            if(ris[j]=='|') //if '|' fill v3
                                            {
                                                if (v2[j] == 0)//if not occupation by '+'
                                                    v3[j]= inChar(v3[j], voc[j]);
                                            }
                                            g++;
                                            ris[j]=0; //cancel
                                        }
                                    }
                                    //check and update v4
                                    if(ascII5[voc[i] - 45] == 0) {

                                        if (g > ascII1_4[voc[i] - 45]) {
                                            ascII1_4[voc[i] - 45] = g;
                                            for(j=0;j<k;j++)
                                            {
                                                if(present[j]==voc[i])
                                                    break;
                                                else if(present[j]==0)
                                                {
                                                    present[j]=voc[i];
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            p = 0; //counter accessible ascII1_4 for output
//update currently Tree
                            if(curTree == NULL)
                                create_miniTree(v2, v3, present, p);
                            else {
                                struct miniTree*tol= min_Node(curTree);
                                while(tol!=NULL){
                                    if (vinCheck(v2, v3, present, tol->str) == 'f')  //so modified
                                        tol= delete_Node(tol);
                                    else
                                    {
                                        tol= successor_Node(tol);
                                        p++;
                                    }
                                }
                                printf("%d\n", p); //out remain compatible ascII1_4
                            }
                        }
                        else {
                            printf("ok\n");
                            break;
                        }
                    }
                    else
                        printf("not_exists\n");
                }
//choice 2. insert >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                else if (voc[0]=='+'&&voc[1] == 'i')
                {
                    if(fgets(voc, V, stdin) == 0) return 0;
                    do {
                        if(remain_space == 0)
                        {
                            remain_space=N;
                            new=malloc(N*(sizeof(struct dictionary) + sizeof(char) * (k + 2)));
                        }
                        root= inVocab(root, create_Node(voc, new));
                        remain_space--;
                        if(curTree != NULL) {
                            if (vinCheck(v2, v3, present, voc) == 'v')
                                curTree = inNode(curTree, reuse_Node(voc));
                        }
                        if (fgets(voc, V, stdin) == 0) return 0;

                    } while (voc[0] != '+');

                }
//choice 3. stamp >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                else if(voc[0]=='+'&&voc[1]=='s'){
                    if(curTree == NULL)
                        stamp_Vocab(root);
                    else
                        stamp_Tree(curTree);
                }
            }
            destroy_miniTree(curTree);
            curTree=NULL; //clean curTree

            if (ToK != k)
                printf("ko\n");

            if(fgets(voc, V, stdin) == 0) return 0;
        }

    } while (!feof(stdin));

    return 0;
}

struct dictionary* create_Node(char s[], struct dictionary*new)
{
    struct dictionary*letter=(struct dictionary*)((char*)new + (N - remain_space) * (sizeof(*new) + (k + 2) * sizeof(char)));
    letter->str=(char*)letter+sizeof(*letter);
    strncpy(letter->str,s,k+2);
    letter->str[k+1]='\0';
    letter->link= letter->L= letter->R=NULL;

    return letter;
}

struct miniTree* reuse_Node(char s[])
{
    struct miniTree *node=malloc(sizeof(struct miniTree) + (k + 2) * sizeof(char));
    node->str=(char*)node+sizeof(*node);
    strncpy(node->str,s,k+1);
    node->F=node->L=node->R=NULL;
    return node;
}

struct dictionary* inVocab(struct dictionary* curr, struct dictionary* new)
{
    new->link=Head_list;
    Head_list=new;

    if(curr == NULL)
        return new;

    struct dictionary*y = NULL;
    struct dictionary*x=curr;

    while(x!=NULL)
    {
        y=x;
        if (quick_cmp(new->str, x->str) < 0)
            x=x->L;
        else
            x=x->R;
    }

    if(quick_cmp(new->str, y->str) < 0)
        y->L=new;
    else
        y->R=new;

    return curr;

}
struct miniTree* inNode(struct miniTree* curr, struct miniTree* new)
{
    if(curr == NULL)
        return new;

    struct miniTree*y = NULL;
    struct miniTree*x=curr;

    while(x!=NULL)
    {
        y=x;

        if (quick_cmp(new->str, x->str) < 0)
            x=x->L;
        else
            x=x->R;
    }
    new->F=y;

    if(quick_cmp(new->str, y->str) < 0)
        y->L=new;
    else
        y->R=new;


    return curr;

}

struct nanoTree*inChar(struct nanoTree*curr, char c)
{
    if(curr == NULL)
    {
        struct nanoTree *new= malloc(sizeof(struct nanoTree));
        new->c = c;
        new->L=new->R=NULL;
        return new;
    }

    struct nanoTree*y = NULL;
    struct nanoTree*x=curr;

    while(x!=NULL)
    {
        y=x;
        if (c< x->c)
            x=x->L;
        else if(c> x->c)
            x=x->R;
        else
            return curr;
    }
    struct nanoTree *new= malloc(sizeof(struct nanoTree));
    new->c = c;
    new->L=new->R=NULL;

    if(c<y->c)
        y->L=new;
    else
        y->R=new;

    return curr;
}

void stamp_Vocab(struct dictionary* curr)
{
    if (curr == NULL)
        return;
    stamp_Vocab(curr->L);
    printf("%s", curr->str);
    stamp_Vocab(curr->R);
}

void stamp_Tree(struct miniTree* curr)
{
    if (curr == NULL)
        return;
    stamp_Tree(curr->L);
    printf("%s", curr->str);
    stamp_Tree(curr->R);
}

char findVocab(struct dictionary *curr, char s[])
{
    while(curr != NULL)
    {
        if(quick_cmp(s, curr->str) == 0)
            return 'v';
        if(quick_cmp(s, curr->str) < 0)
            curr=curr->L;
        else
            curr=curr->R;
    }
    return 'f';

}

char vinCheck(char v2[], struct nanoTree *v3[20], char present[], char word[])
{
    int j;

    for(j=0;j<k;j++)
    {
        if (ascII1_4[word[j] - 45] == -1)//prohibit symbol v1
            return 'f';
        if (v2[j] != 0 && v2[j] != word[j])  //exact position v2
            return 'f';
        if (search(v3[j], word[j]) == 's') //wrong position v3
            return 'f';

    }
    int words[78];
    memset(words,0,78*sizeof(int));

    for(j=0;j<k;j++) //remain_space array temporary
        words[word[j] - 45]++;

    for(j=0;j<k;j++)
    {
        if(ascII5[word[j] - 45] > 0)// v5 compare
        {
            if(words[word[j] - 45] != ascII5[word[j] - 45])
                return 'f';
        }
        else if(ascII1_4[word[j] - 45] > 0)// v4 compare
        {
            if(words[word[j] - 45] < ascII1_4[word[j] - 45])
                return 'f';
        }
        if(words[present[j]-45]==0&&present[j]!=0)
            return 'f';
    }

    return 'v';
}

char search(struct nanoTree *l, char s)
{
    while(l!=NULL)
    {
        if(l->c==s)
            return 's';
        if (l->c <s )
            l = l->R;
        else
            l = l->L;
    }
    return 'n';
}

struct miniTree *delete_Node(struct miniTree *curr) {

    // no child case
    if (curr->L == NULL && curr->R == NULL) {
        if (curr->F == NULL) {
            curTree=NULL;
            free(curr);
            return NULL;
        }

        struct miniTree *x= successor_Node(curr);
        if (curr == curr->F->L)
            curr->F->L = NULL;
        else
            curr->F->R = NULL;
        free(curr);
        return x;

    }
    struct miniTree *y=NULL;
    struct miniTree *x=NULL;

    if(curr->L!=NULL&&curr->R!=NULL)
    {
        //2 children case
        y = successor_Node(curr);
        if(y->L!=NULL)
            x=y->L;
        else
            x=y->R;

        if(x!=NULL)
            x->F=y->F;

        if(y==y->F->L)
            y->F->L=x;
        else
            y->F->R=x;

        strncpy(curr->str,y->str,k+1);

        free(y);

        return curr;
    }
    else {
        //  1 child case
        y = curr;
        curr= successor_Node(curr);
        if(y->L!=NULL)
            x=y->L;
        else
            x=y->R;

        x->F=y->F;

        if(y->F==NULL)
        {
            curTree=x;
            free(y);
            return curr;
        }
        else if(y==y->F->L)
            y->F->L=x;
        else
            y->F->R=x;

        free(y);
        return curr;
    }


}

struct miniTree*min_Node(struct miniTree*curr)
{
    if(curr == NULL)
        return NULL;

    while(curr->L != NULL)
        curr=curr->L;
    return curr;
}

struct miniTree*successor_Node(struct miniTree*curr)
{
    if(curr->R != NULL)
        return min_Node(curr->R);

    struct miniTree*y=curr->F;
    while((y!=NULL)&&(curr == y->R))
    {
        curr=y;
        y=y->F;
    }
    return y;
}

int quick_cmp(const char *s1, const char *s2)
{
    while(*s1&&(*s1==*s2))
    {
        s1++;
        s2++;
    }

    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

void create_miniTree(char v2[], struct nanoTree *v3[20], char present[], int numNode)
{
    struct dictionary*temp=Head_list;
    while(temp!=NULL)
    {
        if (vinCheck(v2, v3, present, temp->str) == 'v')  //vincolo check and insert in temporary Tree
        {
            curTree = inNode(curTree, reuse_Node(temp->str));
            numNode++;
        }
        temp=temp->link;
    }
    printf("%d\n", numNode);
}

void destroy_miniTree(struct miniTree*curr)
{
    if(curr == NULL)
        return ;
    if(curr->L != NULL)
        destroy_miniTree(curr->L);
    if(curr->R != NULL)
        destroy_miniTree(curr->R);

    free(curr);
}


