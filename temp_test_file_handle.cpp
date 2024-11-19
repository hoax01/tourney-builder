#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <fstream>

using namespace std;
void sport_type_validation(char &c);
void format_type_validation(char &c);
int swiss_score_validation(int n);
int league_score_validation(int n);
int knockout_score_validation(int n);
int no_of_teams_validation(int n, char format_type);
void swiss(string team_names[],int score[],int teams,string k_teams[],fstream &file);
void swap_teams(string str[],int j, int smallest);
void swap_score(int score[],int j, int smallest);
void knockout(string teams[],int size,fstream &flie);
void league (string teams[],int scores [], int num_teams,fstream &file);
void get_unique_teams(string teams[], int no_of_teams);
void score_sort(int score[],string str[],int num);
void score_sort(float score[],string str[],int num);
string header(string str,char ch,int iter,int spaces);
void allCaps(string &str);
void writeInFile(fstream &file,string str);
void fileStandingsTable(fstream &file,string teams[],int scores[],int num);



int main(){
    string line;
    string filename;
    string tournament_name;
    char sport_type;
    char format_type;
    int no_of_teams = 0;

    cout<<"Enter Tournament name"<<endl;
    getline(cin,tournament_name);
    filename = tournament_name + ".text";

    fstream file(filename,ios::in | ios:: out | ios::app);  //file opened
    allCaps(tournament_name);
    writeInFile(file,header(tournament_name,'|',3,50));
    writeInFile(file,"");   //empty line (for formatting purpose)

    cout<<"Which sport would you like to conduct a tournament of? (f for football, t for table tennis, v for Valorant, c for Cricket)"<<endl;
    cin>>sport_type;
    sport_type_validation(sport_type);
    cout<<"Which format would you like to use (s for swiss+knockout, l for league, k for knockout)"<<endl;
    cin>>format_type;
    format_type_validation(format_type);
    cout<<"Enter the no of teams"<<endl;
    no_of_teams=no_of_teams_validation(no_of_teams,format_type);


    string teams[no_of_teams];
    int score[no_of_teams] = {0};
    float league_score[no_of_teams] = {0};
    string swiss_qualifiers[no_of_teams/2];

    cin.ignore();

    get_unique_teams(teams,no_of_teams);


    writeInFile(file,"Starting Lineup of Teams:");
    // starting lineup of teams
    for(int i=0;i<no_of_teams;i++){
        allCaps(teams[i]);
        cout<<"Team "<<i+1<<":  "<<teams[i]<<endl;
        line = to_string(i+1)+".  "+teams[i];
        writeInFile(file,line);
    }
    writeInFile(file,"");
    cout<<endl;
    
    
    if(format_type=='s'){
        cout<<"Swiss Round"<<endl;
        cout<<endl;
        writeInFile(file,header("Swiss Round",'*',2,20));
        writeInFile(file,"");
        swiss(teams,score,no_of_teams,swiss_qualifiers,file);
        writeInFile(file,"");

        cout<<"Knockout Round"<<endl;
        cout<<endl;
        writeInFile(file,header("Knockout Round",'*',2,20));
        writeInFile(file,"");
        knockout(swiss_qualifiers,no_of_teams/2,file);
    }
    else if(format_type=='k'){
        knockout(teams,no_of_teams,file);

    }
    else if(format_type=='l'){
        league(teams,score,no_of_teams,file);
    }

    file.close(); // file closed
   
    return 0;
}


void get_unique_teams(string teams[], int no_of_teams){
        for (int i = 0; i < no_of_teams; i++) {
        while (true) {
            cout << "Enter team " << i + 1 << ": ";
            string team_name;
            getline(cin, team_name);
            if (team_name.empty()) {
                cout << "Team name cannot be empty. Please enter a valid name." << endl;
                continue; // Prompt the user again

                
            }            // Check if the team name already exists
            bool is_duplicate = false;
            for (int j = 0; j < i; j++) { // Check names entered so far
                if (teams[j] == team_name) {
                    is_duplicate = true;
                    break;
                }
            }

            if (is_duplicate) {
                cout << "Team name already entered. Please enter a unique name." << endl;
            }
            else {
                teams[i] = team_name; // Add the valid team name to the array
                break; // Exit the loop once a valid name is entered
            }
        }
    }
}
int knockout_score_validation(int n){
        while(true){
        if(cin>>n){
            if(n==1 || n==2){
            return n;
            }
            else{
            cout<<"Invalid Input, only 1, 2 are allowed"<<endl;
        }
    }
       else{
        cin.clear();
        cin.ignore();
        cout<<"Invalid Input, only 1 or 2 are allowed"<<endl;
    }
    
    }
}
int league_score_validation(int n){
        while(true){
        if(cin>>n){
            if(n==1 || n==2 || n==0){
            return n;
            }
            else{
            cout<<"Invalid Input, only 0, 1 or 2 are allowed"<<endl;
        }
    }
       else{
        cin.clear();
        cin.ignore();
        cout<<"Invalid Input, only 0, 1 or 2 are allowed"<<endl;
    }
    
    }
}
int swiss_score_validation(int n){
    while(true){
        if(cin>>n){
            if(n==1 || n==2){
            return n;
            }
            else{
            cout<<"Invalid Input, only 1 or 2 are allowed"<<endl;
        }
    }
       else{
        cin.clear();
        cin.ignore();
        cout<<"Invalid Input, only 1 or 2 are allowed"<<endl;
    }
    
    }
}


void swiss(string team_names[],int score[],int teams,string q_teams[],fstream &file){
    int rounds=log2(teams);
    for(int round=0;round<rounds;round++){
        int b=1;
        cout<<"Round "<<round+1<<endl;
        writeInFile(file,header("Round "+to_string(round+1),'.',1,10));
        writeInFile(file,"");

        for(int i=0;i<teams;i+=2){
            writeInFile(file,"Match "+to_string(b));
            cout<<"Match "<<b<<endl;
            cout<<team_names[i]<<" (team 1) vs "<<team_names[i+1]<<" (team 2)"<<endl;;
            writeInFile(file,team_names[i]+" vs "+team_names[i+1]);
            cout<<"Winner of the match? (1 or 2)"<<endl;
            int result=0;
            result=swiss_score_validation(result);
            if(result==1){
                score[i]+=1;
                writeInFile(file,string("( ")+team_names[i]+" won the match )");
            }
            else{
                score[i+1]+=1;
                writeInFile(file,string("( ")+team_names[i+1]+" won the match )");
            }
            writeInFile(file," ");
            b++;
        }

        score_sort(score,team_names,teams);
        cout<<"standings after Round "<<round+1<<":"<<endl;
        for(int i=0;i<teams;i++){
            cout<<setw(5)<<team_names[i]<<setw(5)<<"-score: "<<setw(5)<<score[i]<<endl;
        }

        if(round!=rounds-1)
        {cout<<"would you like to continue? (press any letter to resume)"<<endl;
        char x;
        cin>>x;}
    }

    writeInFile(file,"");

    writeInFile(file,header("Final Swiss Standings",'-',2,10));
    fileStandingsTable(file,team_names,score,teams);

    writeInFile(file,"");
    writeInFile(file,"Teams going to the knockout stage: ");
    writeInFile(file,"");

    cout<<"Teams going to the knockout stage: "<<endl;
    for(int i=0;i<teams/2;i++){
        cout<<team_names[i]<<endl;
        q_teams[i] = team_names[i];
        writeInFile(file,q_teams[i]);
    }

}


void format_type_validation(char &c){
    while(c!='s' && c!='v' && c!='k' && c!='l'){
        cout<<"please only type s for swiss+knockout, k for knockout, l for league"<<endl;
        cin>>c;
    }

}
void sport_type_validation(char &c){
    while(c!='f' && c!='v' && c!='t' && c!='c'){
        cout<<"please only type f for football, t for table tennis, v for Valorant, c for Cricket"<<endl;
        cin>>c;
    }


}


int no_of_teams_validation(int n, char format_type){  //no of teams entered validation
    while(true){
        if(format_type=='s'){
            if(cin>>n && n>1 && (n&(n-1))==0){
                return n;
            }
            else{
                cin.clear();
                cin.ignore();
                cout<<"The no of teams entered do not match the swiss criteria, total teams entered should be powers of 2"<<endl;
            }
        }
        else if(format_type=='k'|| format_type=='l'){
                if(cin>>n && n%2==0 && n!=0){
                return n;
            }
            else{
                cin.clear();
                cin.ignore();
                cout<<"The no of teams entered do not match the league/knockout criteria, total teams entered should be multiples of 2"<<endl;
            }
        }
    }
}

void knockout(string teams[],int size,fstream &file)
{
    int num_rounds = 0;
    int result = 0;

    do
    {
        num_rounds = size / 2;
        string pl[num_rounds];
        if(size != 2){
            cout<<"Round of "<<size<<endl;
            writeInFile(file,header("Round of "+to_string(size),'.',1,10));
        }
        else{
            cout<<"Grand Final"<<endl;
            writeInFile(file,header("Grand Final",'.',1,10));
        }

        writeInFile(file,"");

        for (int y = 0;y<num_rounds;y++)
        {
            cout<<"Match "<<y+1<<endl;
            cout<<teams[y]<<" vs "<<teams[size-1-y]<<endl;
            writeInFile(file,"Match "+to_string(y+1));
            writeInFile(file,teams[y]+" vs "+teams[size-1-y]);
            cout<<"whats the result of the match? (Enter 1 for Team 1 and 2 for Team 2)"<<endl;
            result=knockout_score_validation(result);

            if (result == 1)
            {
                pl[y] = teams[y];
                writeInFile(file,string("( ")+teams[y]+" won the match )");
            }
            else{
                pl[y] = teams[size-1-y];
                writeInFile(file,string("( ")+teams[size-1-y]+" won the match )");
            }
            writeInFile(file,"");

        }

        // updating teams array
        for (int z = 0; z<num_rounds ; z++){
            teams[z] = pl[z];
        }

        size = size / 2;
        cout<<endl;

        if (size <= 1)
        {
            cout<<"Winner of the Knockout Tournament is: "<<endl;
            cout<<teams[0]<<" !!!!";

            writeInFile(file,header("Winner of the Knockout Tournament is: "+teams[0],'!',3,10));
        }

    }while(size>1);

}

void league (string teams[],int scores [], int num_teams,fstream &file)
{
    int rounds = num_teams - 1;
    int m_result;

    for (int round = 0; round < rounds; round ++) {
        cout << "Round " << round + 1 <<endl;
        for (int match = 0; match < num_teams / 2; match++) {
            int player1 = (round + match) % (rounds);
            int player2 = (rounds - match + round) % (num_teams - 1);
            if (match == 0) player2 = num_teams - 1;

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

void score_sort(int score[],string str[],int num){
    int temp_num = 0;
    string temp_str;
    for (int x = 0; x<num;x++){
        for (int y = 0; y<num-1;y++){
            if (score[y] <= score[y+1]){
                    temp_num = score[y];
                    temp_str = str[y];

                    score[y] = score[y+1];
                    str[y] = str[y+1];

                    score[y+1] = temp_num;
                    str[y+1] = temp_str;
                }
        }
    }
}

// overloaded functions for float scores

void swap_score(float score[],int j, int smallest){ 
    int temp;
    temp=score[j];
    score[j]=score[smallest];
    score[smallest]=temp;
}

void score_sort(float score[],string str[],int num){
    float temp_num = 0;
    string temp_str;
    for (int x = 0; x<num;x++){
        for (int y = 0; y<num-1;y++){
            if (score[y] <= score[y+1]){
                    temp_num = score[y];
                    temp_str = str[y];

                    score[y] = score[y+1];
                    str[y] = str[y+1];

                    score[y+1] = temp_num;
                    str[y+1] = temp_str;
                }
        }
    }
}

// new function file handling etc

string header(string str,char ch,int iter,int spaces){
    string header;
    header = string(iter,ch)+header;
    header = header + string(spaces,' ');
    header = header+str;
    header = header + string(spaces,' ');
    header = header + string(iter,ch);
    return header;
}

void allCaps(string &str){

    int len = str.length();
    for(int z = 0;z<len;z++){
        if (str[z]>=97 && str[z]<=122){
            str[z] = str[z] - 32;
        }
    }
}

void writeInFile(fstream &file,string str){
    file<<str<<endl;
}

void fileStandingsTable(fstream &file,string teams[],int scores[],int num){
    file<<left<<setw(4)<<"Pos"<<setw(20)<<"Team"<<setw(10)<<"Wins"<<endl;
    file<<""<<endl;
    for (int x= 0;x<num;x++){
        file<<left<<setw(4)<<to_string(x+1)+"."<<setw(20)<<teams[x]<<setw(10)<<scores[x]<<endl;
    }
}