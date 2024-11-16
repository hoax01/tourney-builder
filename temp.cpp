#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cmath>
#include <limits>
using namespace std;
int score_validation(int n);
void swiss(string team_names[],int score[],int teams,string k_teams[]);
void swap_teams(string str[],int j, int smallest);
void swap_score(int score[],int j, int smallest);
void swiss_sort_score(int score[], string str[],int teams);
int swiss_input_power_of_two_validation(int n);
void knockout(string teams[],int size);
void league (string teams[],int scores [], int size);
void get_unique_teams(string teams[], int no_of_teams);
int main(){
    string tournament_name;
    char sport_type;
    char format_type;
    int no_of_teams;

    cout<<"Enter Tournament name"<<endl;
    getline(cin,tournament_name);
    //cin.ignore();
    cout<<"Which sport would you like to conduct a tournament of? (f for football, t for table tennis, v for Valorant, c for Cricket)"<<endl;
    cin>>sport_type;
    cout<<"Which format would you like to use (s for swiss+knockout, l for league, k for knockout)"<<endl;
    cin>>format_type;
    cout<<"enter the no of teams"<<endl;
    cin>>no_of_teams;


    string teams[no_of_teams];
    int score[no_of_teams]={0};
    string swiss_qualifiers[no_of_teams/2];

    cin.ignore();

    get_unique_teams(teams,no_of_teams);
    for(int i=0;i<no_of_teams;i++){
            cout<<teams[i]<<endl;
        
    }
    

    
    if(format_type=='s'){
        swiss(teams,score,no_of_teams,swiss_qualifiers);
    }

}
/*int main(){
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
    //swiss(rounds,str,score,teams,k_teams,half_teams);

    char sport = ' ';
    // char tourn_type = ' ';
    // int team_num = 0;

    // cout<<"Enter number of Teams:";
    // cin>>team_num;

    // string teams[team_num];
    // float scores[team_num];

    // for (int x = 0; x<team_num ; x++)
    // {
    //     cout<<"Team name?";
    //     cin>>teams[x];
    //     scores[x] = 0;
    // }
    return 0;
}*/
void get_unique_teams(string teams[], int no_of_teams){
        for (int i = 0; i < no_of_teams; i++) {
        while (true) {
            cout << "Enter team " << i + 1 << ": ";
            string team_name;
            getline(cin, team_name);

            // Check if the team name already exists
            bool is_duplicate = false;
            for (int j = 0; j < i; j++) { // Check names entered so far
                if (teams[j] == team_name) {
                    is_duplicate = true;
                    break;
                }
            }

            if (is_duplicate) {
                cout << "Team name already entered. Please enter a unique name." << endl;
            } else {
                teams[i] = team_name; // Add the valid team name to the array
                break; // Exit the loop once a valid name is entered
            }
        }
    }
}

int score_validation(int n){
    while( n>2 || n<1){
        cout<<"Please only type 1 for team 1 or 2 for team 2, no other number or letter"<<endl;
        cin>>n;
    }
    return n;
}


void swiss(string team_names[],int score[],int teams,string k_teams[]){
    int rounds=log2(teams);
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
    for(int i=teams-1;i>=teams/2;i--){
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
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/*void swap_teams(string str[],int j, int smallest){  
    string temp;
    temp=str[j];
    str[j]=str[smallest];
    str[smallest]=temp;
}*/
/*void swap_score(float score[],int j, int smallest){ 
    int temp;
    temp=score[j];
    score[j]=score[smallest];
    score[smallest]=temp;
}*/
/*void swiss_sort_score(float score[], string str[],int teams){ 
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
}*/

void knockout(string teams[],int size)
{
    int num_rounds = size / 2;
    bool result = 0;

    do
    {
        num_rounds = size / 2;
        string pl[num_rounds];
        cout<<"Round of "<<size<<endl;

        for (int y = 0;y<size;y = y+2)
        {
            cout<<"Match "<<y+1<<endl;
            cout<<teams[y]<<" vs "<<teams[y+1]<<endl;
            cout<<"whats the result of the match  (Enter 0 for Team 1 and 1 for Team 2)"<<endl;
            cin>>result;

            if (result == 0)
            {
                pl[y/2] = teams[y];
            }
            else{
                pl[y/2] = teams[y+1];
            }

        }

        size = size / 2;
        cout<<endl;

        if (size < 1)
        {
            cout<<"Winner of the Knockout Tournament is: "<<endl;
            cout<<pl[0];
        }

    }while(size>1);

}

void league (string teams[],int scores [], int size)
{
    int rounds = size - 1;
    int m_result;

    for (int round = 0; round < rounds; round ++) {
        cout << "Round " << round + 1 <<endl;
        for (int match = 0; match < size / 2; match++) {
            int player1 = (round + match) % (rounds);
            int player2 = (rounds - match + round) % (size - 1);
            if (match == 0) player2 = size - 1;

            cout << "Match " << match + 1 << ": " << teams[player1] << " vs " << teams[player2]<<endl;
            cout<<"Enter 0 for Team 1, 1 for Team 2 and 2 for draw"<<endl;
            cin>>m_result;

            if (m_result == 0)
            {
                scores[player1] += 1;
                cout<<teams[player1]<<" won the match"<<endl;
            }
            else if (m_result == 1)
            {
                scores[player2] += 1;
                cout<<teams[player2]<<" won the match"<<endl;
            }
            else
            {
                scores[player1] += 0.5;
                scores[player2] += 0.5;
                cout<<"match was drawn"<<endl;
            }

        }
        cout << endl;
    }
}

