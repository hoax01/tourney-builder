#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cmath>
#include <limits>
using namespace std;
int score_validation(int n);
void swiss(int rounds,string team_names[],int score[],int teams,string k_teams[],int half_teams);
void swap_teams(string str[],int j, int smallest);
void swap_score(int score[],int j, int smallest);
void swiss_sort_score(int score[], string str[],int teams);
int swiss_input_power_of_two_validation(int n);
int main(){
    int teams;
    int r=0; 
    do{
    cout<<"How many teams do you need {powers of 2}?";
    cin>>teams;
    r=swiss_input_power_of_two_validation(teams);
    }while(r==0);
    string str[teams];
    int score[teams]={0}; 
    string team_no;
    for(int i=0;i<teams;i++){
        team_no=to_string(i+1);
        str[i]="Team " + team_no;
    }
    int rounds=log2(teams);
    int half_teams=teams/2;
    string k_teams[half_teams];
    swiss(rounds,str,score,teams,k_teams,half_teams);
    return 0;
}


int score_validation(int n){
    while( n>2 || n<1){
        cout<<"Please only type 1 for team 1 or 2 for team 2, no other number or letter"<<endl;
        cin>>n;
    }
    return n;
}


void swiss(int rounds,string team_names[],int score[],int teams,string k_teams[],int half_teams){
    int a=1;
    for(int round=0;round<rounds;round++){
        int b=1;
        cout<<"Round "<<a<<endl;
        for(int i=0;i<teams;i+=2){
            cout<<"Match "<<b<<endl;
            cout<<team_names[i]<<" (team 1) vs "<<team_names[i+1]<<" (team 2)"<<endl;;
            cout<<"who is gonna win? (1 or 2)"<<endl;
            int victory=0;
            cin>>victory;
            victory=score_validation(victory);
            if(victory==1){
                score[i]+=1;
            }
            else{
                score[i+1]+=1;
            }
            b++;
        }
        swiss_sort_score(score,team_names,teams);
        cout<<"standings after Round "<<a<<":"<<endl;
        for(int i=0;i<teams;i++){
            cout<<setw(5)<<team_names[i]<<setw(5)<<"-score: "<<setw(5)<<score[i]<<endl;
        }
        if(round!=rounds-1)
        {cout<<"would you like to continue? (press any letter to resume)"<<endl;
        char x;
        cin>>x;}
        a++;
    }
    cout<<"Teams going to the knockout stage: "<<endl;
    k_teams[half_teams];
    for(int i=teams-1;i>=half_teams;i--){
        cout<<team_names[i]<<endl;
        k_teams[i];
    }

    
}


void swap_teams(string str[],int j, int smallest){  
    string temp;
    temp=str[j];
    str[j]=str[smallest];
    str[smallest]=temp;
}
void swap_score(int score[],int j, int smallest){ 
    int temp;
    temp=score[j];
    score[j]=score[smallest];
    score[smallest]=temp;
}


void swiss_sort_score(int score[], string str[],int teams){ 
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


int swiss_input_power_of_two_validation(int n){  //no of teams entered validation
    if(n!=1 && n>1&& (n&(n-1))==0){
        return 1;
    }
    else{
        return 0;
    }
}