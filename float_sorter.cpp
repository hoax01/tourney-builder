#include <iostream>
#include <iomanip>
using namespace std;
void swap_teams(string str[],int j, int smallest){  
    string temp;
    temp=str[j];
    str[j]=str[smallest];
    str[smallest]=temp;
}
void swap_score(float score[],int j, int smallest){ 
    int temp;
    temp=score[j];
    score[j]=score[smallest];
    score[smallest]=temp;
}
void swiss_sort_score(float score[], string str[],int teams){ 
    for(int j=0;j<teams-1;j++){
        int smallest=j;
        for(int i=j;i<teams;i++){
            if(score[smallest]>score[i]){
                smallest=i;
            }
        }
        swap_score(score,j,smallest);
        swap_teams(str,j,smallest);
    }
}