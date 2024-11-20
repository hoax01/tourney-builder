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
void league (string teams[],float scores [], int num_teams,fstream &file);
void get_unique_teams(string teams[], int no_of_teams);
void score_sort(int score[],string str[],int num);
void score_sort(float score[],string str[],int num);
string header(string str,char ch,int iter,int spaces);
void allCaps(string &str);
void writeInFile(fstream &file,string str);
void fileStandingsTable(fstream &file,string teams[],int scores[],int num);
void fileStandingsTable(fstream &file,string teams[],float scores[],int num);
void leagueStanding(string teams[],float scores[],int num);



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
        cout<<"Knockout Compeition"<<endl;
        cout<<endl;
        writeInFile(file,header("Knockout Tournament",'*',2,20));
        writeInFile(file,"");
        knockout(teams,no_of_teams,file);

    }
    else if(format_type=='l'){
        cout<<"League Compeition"<<endl;
        cout<<endl;
        writeInFile(file,header("League Tournament",'*',2,20));
        writeInFile(file,"");
        league(teams,league_score,no_of_teams,file);
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
        else if(format_type=='k'){
                if(cin>>n && n%2==0 && n!=0){
                return n;
            }
            else{
                cin.clear();
                cin.ignore();
                cout<<"The no of teams entered do not match the knockout criteria, total teams entered should be multiples of 2"<<endl;
            }
        }
        else{
            cin>>n;
            return n;
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

void league (string teams[],float scores [], int num_teams,fstream &file)
{
    string display_standings;
    int league_size = 0;
    if (num_teams % 2 != 0){         //deals with odd number of players
        league_size = num_teams+1;
    }
    else{                   //deals with even number of players
        league_size = num_teams;
    }  

    string league_array[league_size];
    float league_scores[league_size] = {0};

    if (num_teams % 2 != 0){
        for(int z = 0; z<league_size;z++){
            if(z == league_size-1){
                league_array[z] =  "bye";
            }
            else{
                league_array[z] = teams[z];
        }   }
    }
    else{
        for(int z = 0; z<league_size;z++){
            league_array[z] = teams[z];
        }
    }
    

    
    int rounds = league_size - 1;
    int m_result = 0;

    for (int round = 0; round < rounds; round ++) {
        cout << "Round " << round + 1 <<endl;
        writeInFile(file,header("Round "+to_string(round+1),'.',1,10));
        writeInFile(file,"");
        for (int match = 0; match < league_size / 2; match++) {
            int p1 = (round + match) % (rounds);
            int p2 = (rounds - match + round) % (league_size - 1);
            if (match == 0){
                p2 = league_size - 1;
            }

            if(league_array[p1] == "bye" || league_array[p2] == "bye"){       // skips matchup if there is a bye
                continue;
            }

            cout<<league_array[p1]<<" vs "<<league_array[p2]<<endl;
            cout<<"Enter 0 for Team 1, 1 for Team 2 and 2 for draw"<<endl;
            writeInFile(file,league_array[p1]+" vs "+league_array[p2]);
            cin>>m_result;

            if (m_result == 0)
            {
                league_scores[p1] += 1;
                cout<<league_array[p1]<<" won the match"<<endl;
                writeInFile(file,string("( ")+league_array[p1]+" won the match )");
            }
            else if (m_result == 1)
            {
                league_scores[p2] += 1;
                cout<<league_array[p2]<<" won the match"<<endl;
                writeInFile(file,string("( ")+league_array[p2]+" won the match )");
            }
            else
            {
                league_scores[p1] += 0.5;
                league_scores[p2] += 0.5;
                cout<<"match was drawn"<<endl;
                writeInFile(file,"Match drawn!");
            }
            writeInFile(file,"");

        }
        cout<<"Do you want to display standings after "<<round+1<<" rounds?  (yes/no)"<<endl;
        cin>>display_standings;

        if(display_standings == "yes"){
            string temp_teams[league_size];
            float temp_scores[league_size] = {0};
            int temp_num = 0;
            if(num_teams % 2 == 0){
                for (int z =0;z<league_size;z++){
                    temp_teams[z] = league_array[z];
                    temp_scores[z] = league_scores[z];
                }
                temp_num = league_size;
            }
            else{
                for(int z = 0;z<league_size-1;z++){
                    temp_teams[z] = league_array[z];
                    temp_scores[z] = league_scores[z];
                }
                temp_num = league_size-1;
            }
            score_sort(temp_scores,temp_teams,temp_num);
            leagueStanding(temp_teams,temp_scores,temp_num);
        }
        cout << endl;
    }
    cout<<"Final Standings for League:"<<endl;
    if(num_teams%2 == 0){
        score_sort(league_scores,league_array,league_size);
        leagueStanding(league_array,league_scores,league_size);
        writeInFile(file,header("Final League Table",'-',2,10));
        fileStandingsTable(file,league_array,league_scores,league_size);
    }
    else{
        score_sort(league_scores,league_array,league_size-1);
        leagueStanding(league_array,league_scores,league_size-1);
        writeInFile(file,header("Final League Table",'-',2,10));
        fileStandingsTable(file,league_array,league_scores,league_size-1);
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

void fileStandingsTable(fstream &file,string teams[],float scores[],int num){
    file<<left<<setw(4)<<"Pos"<<setw(20)<<"Team"<<setw(10)<<"Wins"<<endl;
    file<<""<<endl;
    for (int x= 0;x<num;x++){
        file<<left<<setw(4)<<to_string(x+1)+"."<<setw(20)<<teams[x]<<setw(10)<<scores[x]<<endl;
    }
}

void leagueStanding(string teams[],float scores[],int num){
    cout<<left<<setw(4)<<"Pos"<<setw(20)<<"Team"<<setw(10)<<"Points"<<endl;
    cout<<endl;
    for (int x= 0;x<num;x++){
        cout<<left<<setw(4)<<to_string(x+1)+"."<<setw(20)<<teams[x]<<setw(10)<<scores[x]<<endl;
    }
}