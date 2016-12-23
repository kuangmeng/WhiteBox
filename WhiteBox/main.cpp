#include<iostream>
#include<stdlib.h>
#include<sys/malloc.h>
#include<cstring>
#include <string.h>
#define MAX 100
using namespace std;
int Tail[MAX]={0};
int Head[MAX]={0};
char Judge[MAX]={'\0'};
int Array[MAX]={0};
int BasicPath[MAX];
int CC = 0;
bool EXTOrNot = false;
string Result = *new string();
//路径
typedef struct  Road{
    int tail,head;
    struct Road *hlink, *tlink;
    char judge;   //T/F/N
}Road;
//节点
typedef struct Node{
    int data;
    bool visited;
    Road *fin, *fout;
}Node;
//图（路径和顶点）
typedef struct{
    Node list[MAX];
    int vertexNum, pathNum;
}Graph;

bool EndOrNot(string str){
    if(str.compare("END") == 0 || str.compare("end") == 0 || str.compare("EXT") == 0 || str.compare("ext") == 0){
        return true;
    }
    return false;
}

int length(int num[]){
    int sum=0;
    for(int i=0; i<MAX; i++){
        if(num[i] != 0)
            sum++;
        else
            break;
    }
    return sum;
}

bool InArrayOrNot(int ch[],int in){
    for(int i=0; i<length(ch); i++){
        if(in == ch[i]){
            return true;
        }
    }
    return false;
}

void insert(string str){
    int a = length(Array);
    int t = length(Tail);
    string tail = str.substr(0,str.find('-'));
    string head = str.substr(str.find('>')+1,str.find(',')-str.find('>')-1);
    char judge = str[str.size()-1];
    if(!InArrayOrNot(Array, atoi(tail.c_str())))
        Array[a++] = atoi(tail.c_str());
    if(!InArrayOrNot(Array, atoi(head.c_str())))
        Array[a++] = atoi(head.c_str());
    Tail[t] = atoi(tail.c_str());
    Head[t] = atoi(head.c_str());
    Judge[t] = judge;
}
//节点在图内位置
int VertexLocate(Graph *G, int vertex){
    int j=0;
    for(int k=0; k<length(Array); k++)
        if(G->list[k].data == vertex){
            j = k;
            break;
        }
    return j;
}
//建图
void CreateGraph(Graph *G){
    int tail, head;
    Road *Temp;
    G->vertexNum = G->pathNum =0;
    for(int i=0; i<length(Array); i++){
        (*G).list[i].data = Array[i];
        G->list[i].fin = NULL;
        G->list[i].fout = NULL;
        G->vertexNum++;
        G->list[i].visited = false;
    }
    for(int i=0; i<length(Tail); i++){
        if(Judge[i] == 'T')
            CC ++;
        tail = VertexLocate(G,Tail[i]);
        head = VertexLocate(G,Head[i]);
        Temp = (Road*)malloc(sizeof(Road));
        Temp->tail = tail;
        Temp->head = head;
        Temp->tlink = (*G).list[tail].fout;
        (*G).list[tail].fout = Temp;
        Temp->hlink = (*G).list[head].fin;
        (*G).list[head].fin = Temp;
        Temp->judge = Judge[i];
        G->pathNum++;
    }
}
void Display(Graph *G,int arr[]){
    char ch[10];
    int Temp =0;
    bool NoOrNot = false;
    for(int i=0; i<length(arr)-1; i++){
        string Arc = "";
        Temp = arr[i];
        snprintf(ch,sizeof(ch),"%d",Temp);
        Arc += ch;
        Arc += ",";
        Temp = arr[i+1];
        snprintf(ch,sizeof(ch),"%d",Temp);
        Arc += ch;
        if(Result.find(Arc) != Result.npos)
            continue;
        else{
            NoOrNot = true;
            break;
        }
    }
    if(NoOrNot == true){
        memset(ch,0,10);
        for(int i=0; i<length(arr)-1; i++){
            Temp = arr[i];
            snprintf(ch,sizeof(ch),"%d",Temp);
            Result += ch;
            Result += ",";
        }
        memset(ch,0,10);
        snprintf(ch,sizeof(ch),"%d",arr[length(arr)-1]);
        Result += ch;
        Result += "|";
    }
}

int Memset(int arr[],int data){
    int Temp = 0;
    int len = length(arr);
    for(int i=0; i<len; i++){
        if(data == arr[i]){
            Temp = i;
            break;
        }
    }
    for(int i=Temp+1; i<len; i++){
        arr[i] = 0;
    }
    return Temp;
}

void ChangeToVisited(Graph *G,int arr[], int num){
    bool find = false;
    int temp =0;
    for(int i=0; i<length(arr); i++){
        if(arr[i] == num){
            find = true;
            i++;
        }
        if(arr[i] != 0){
            if(find == true){
                temp = VertexLocate(G,arr[i]);
                G->list[temp].visited = false;
            }
        }
    }
}

void DFSTraverse(Graph G, int pos){
    Road *p;
    Road *Queue[MAX*MAX];
    int Vex[MAX]={0};
    int _len_=0;
    int queue = -1;
    int pointer = -1;
    p = G.list[pos].fout;
    BasicPath[++pointer] = G.list[pos].data;
    if(!p){
        Display(&G,BasicPath);
        return;
    }
    while(p){
        if(!InArrayOrNot(BasicPath,G.list[p->tail].data))
            BasicPath[++pointer] = G.list[p->tail].data;
        if(p->judge != 'N'){
            if(InArrayOrNot(Vex,G.list[p->tail].data) && (!G.list[p->tail].visited)){
                if(p->judge == 'T')
                    p = p->tlink;
            }
            else{
                if((p->judge == 'T') && (!G.list[p->tail].visited)){
                    Queue[++queue] = p;
                    p = p->tlink;
                }else if(p->judge == 'F'){
                    Queue[++queue] = p->tlink;
                }
                G.list[p->tail].visited = true;
                if(!InArrayOrNot(Vex,G.list[p->tail].data))
                    Vex[_len_++] = G.list[p->tail].data;
            }
        }
        if(InArrayOrNot(BasicPath,G.list[p->head].data)){
            BasicPath[++pointer] = G.list[p->head].data;
            if(queue >= 0){
                Display(&G,BasicPath);
                p = Queue[queue];
                queue --;
                pointer = Memset(BasicPath,G.list[p->tail].data);
                ChangeToVisited(&G,Vex,G.list[p->tail].data);
            }else{
                Display(&G,BasicPath);
                break;
            }
            continue;
        }
        BasicPath[++pointer] = G.list[p->head].data;
        pos = VertexLocate(&G,G.list[p->head].data);
        p = G.list[pos].fout;
        if(!p){
            if(queue >= 0){
                Display(&G,BasicPath);
                p = Queue[queue];
                queue --;
                pointer = Memset(BasicPath,G.list[p->tail].data);
                ChangeToVisited(&G,Vex,G.list[p->tail].data);
            }else{
                Display(&G,BasicPath);
                break;
            }
        }
    }
}
int returnNum(string str,char ch){
    int sum =0;
    while(str.size()){
        int position = (int)str.find(ch);
        if(position != str.npos){
            str = str.substr(position+1);
            sum++;
        }else
            break;
    }
    return sum+1;
}

bool StringCompare(string str1,string str2){
    str1 += ",";
    str2 += ",";
    string temp1 = "";
    string temp2 = "";
    int pos1 = (int)str1.find(',');
    int pos2 = (int)str2.find(',');
    while(str1.size()){
        temp1 = str1.substr(0,pos1);
        temp2 = str2.substr(0,pos2);
        if(atoi(temp1.c_str()) > atoi(temp2.c_str()))
            return true;
        else if(atoi(temp1.c_str()) < atoi(temp2.c_str()))
            return false;
        str1 = str1.substr(pos1+1);
        pos1 = (int)str1.find(',');
        str2 = str2.substr(pos2+1);
        pos2 = (int)str2.find(',');
    }
    return false;
}

void orderDisplay(string str){
    string ordered = "";
    string Str = str;
    string small = str;
    int smallNum = 100;
    string temp = "";
    char EndOrNot = 'N';
    int pos = (int)str.find('|');
    while(str.size()){
        for(int i=0; i<Str.size(); i++){
            temp = Str.substr(0,pos);
            if(returnNum(temp,',') < smallNum){
                small = temp;
                smallNum = returnNum(temp,',');
            }
            else if(returnNum(temp,',') == smallNum)
                if(StringCompare(small,temp)){
                    small = temp;
                    smallNum = returnNum(temp,',');
                }
            if(EndOrNot == 'N'){
                Str = Str.substr(pos+1);
                pos = (int)Str.find('|');
                if(pos != Str.npos)
                    continue;
                else{
                    pos = (int)Str.size();
                    EndOrNot = 'Y';
                }
            }
            else
                break;
        }
        ordered += small;
        ordered += "|";
        int _pos = (int)str.find(small);
        str.replace(_pos,small.size()+1,"");
        Str = small = str;
        smallNum =100;
        pos = (int)str.find('|');
        EndOrNot = 'N';
    }
    Result = ordered;
}

void coutResult(string str){
    int pos = (int)str.find('|');
    string temp;
    while(str.size()){
        temp = str.substr(0,pos);
        cout << temp << endl;
        str = str.substr(pos+1);
        pos = (int)str.find('|');
    }
}

void changeArray(int num){
    for(int i=0; i<length(Array); i++)
        if(Array[i] == num){
            Array[i] = num*10 + 1;
            Array[length(Array)] = num*10 + 2;
            break;
        }
}

void CreatePath(string str){
    string ver = str.substr(0,str.find(','));
    string Con = str.substr(str.find(',')+1);
    int Num = atoi(ver.c_str());
    int T = 0,F = 0,in[MAX]={0};
    int l = length(Tail);
    int index =0;
    changeArray(Num);
    for(int i=0; i<l; i++)
        if(Head[i] == Num)
            in[index++] = i;
    for(int j=0; j<l; j++){
        if((Judge[j] == 'F') && (Tail[j] == Num))
            F = j;
        else if((Judge[j] == 'T') && (Tail[j] == Num))
            T = j;
    }
    for(int i=0; i<index; i++)
        Head[in[i]] = Num*10+1;
    if(Con == "AND"){
        int into = length(Tail);
        Tail[F] = Num*10+1;
        Tail[into] = Num*10+1;
        Head[into] = Num*10+2;
        Judge[into] = 'T';
        into ++;
        Tail[T] = Num*10+2;
        Tail[into] = Num*10+2;
        Head[into] = Head[F];
        Judge[into] = 'F';
    }
    else if(Con == "OR"){
        int into = length(Tail);
        Tail[T] = Num*10+1;
        Tail[into] = Num*10+1;
        Head[into] = Num*10+2;
        Judge[into] = 'F';
        into++;
        Tail[F] = Num*10+2;
        Tail[into] = Num*10+2;
        Head[into] = Head[T];
        Judge[into] = 'T';
    }
}
int main(){
    Graph G;
    int i=0;
    string temp ="";
    getline(cin,temp);
    if(temp.compare("END") == 0 || temp.compare("end") == 0) return 0;
    Array[i++] = atoi(temp.c_str());
    while(true){
        getline(cin,temp);
        if(temp.compare("EXT") == 0 || temp.compare("ext") == 0) EXTOrNot = true;
        if(EndOrNot(temp)) break;
        insert(temp);
    }
    if(EXTOrNot){
        while(true){
            getline(cin,temp);
            if(EndOrNot(temp)) break;
            CreatePath(temp);
        }
    }
    CreateGraph(&G);
    DFSTraverse(G,VertexLocate(&G,Array[0]));
    cout << "CC=" << returnNum(Result,'|')-1 << endl;
    for(int i=0;i<10;i++){
        orderDisplay(Result);
    }
    coutResult(Result);
    return 0;
}
