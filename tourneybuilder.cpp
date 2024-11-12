#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
void swap_teams(string str[],int j, int smallest){  //swap function, miss kra
    string temp;
    temp=str[j];
    str[j]=str[smallest];
    str[smallest]=temp;
}
void swap_score(int score[],int j, int smallest){ //swap func again
    int temp;
    temp=score[j];
    score[j]=score[smallest];
    score[smallest]=temp;
}
void sort_score(int score[], string str[],int teams){ //sort func, abhi tak mainly yehi likha
    for(int j=0;j<teams-1;j++){
        int smallest=score[j];
        for(int i=j;i<teams;i++){
            if(score[smallest]>score[i]){
                smallest=i;
            }
        }
        swap_score(score,j,smallest);
        swap_teams(str,j,smallest);
    }
}
int power_of_two(int n){  //input validation
    if((n!=0 && n!=1 && n&(n-1))==0){
        return 1;
    }
    else{
        return 0;
    }
}

int main(){
    int teams;
    int r=0; // do while loop chlanay ke liye
    do{
    cout<<"How many teams do you need {powers of 2}?";
    cin>>teams;
    r=power_of_two(teams);
    }while(r==0);
    const int CAPACITY=100; //just a rlly big num, I hope there arent 128 teams loll
    string str[CAPACITY];
    int score[teams]={0}; //start me score 0 rakh rha
    for(int i=0;i<teams;i++){
        string skibidi=to_string(i+1);
        str[i]="Team " + skibidi;  //teams ko naam de rha hu
    }
    for(int i=0;i<teams;i++){ //the following lines just check the code for now
    // random score daal kar dekh raha hu if sorting is working
    //will do swiss tmrw or thursday
        cout<<"score pls"<<endl;
        cin>>score[i];
    }
    sort_score(score,str,teams);
    for(int i=0;i<teams;i++){
        cout<<str[i]<<" "<<score[i]<<endl;
    }


    






    return 0;
}