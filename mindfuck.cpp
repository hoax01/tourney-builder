#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
using namespace sf;
using namespace std;

using namespace std;
// validation functions
void sport_type_validation(char &sport_type, bool &valid) {
    sport_type = toupper(sport_type);
    valid = (sport_type == 'F' || sport_type == 'T' || sport_type == 'V' || sport_type == 'C');
}

// Validation for format type
void format_type_validation(char &format_type, bool &valid) {
    format_type = toupper(format_type);
    valid = (format_type == 'S' || format_type == 'L' || format_type == 'K');
}
int input_validation(int n);
void sport_type_validation(char &c);
void format_type_validation(char &c);
int swiss_score_validation(int n);
int league_score_validation(int n);
int score_validation(int n);
int no_of_teams_validation(int n, char format_type);
int maxScoreValidation(int n,int max);
int noDraws(string a, string b, int &num1,int &num2);
//-------
void swiss(string team_names[],int score[],int teams,string k_teams[],fstream &file);
void knockout(string teams[],int size,fstream &flie);
void league (string teams[],float scores [], int num_teams,fstream &file);
void leagueStanding(string teams[],float scores[],int num);
//misc functions
string header(string str,char ch,int iter,int spaces);
void allCaps(string &str);
void get_unique_teams(string teams[], int no_of_teams);
void score_sort(int score[],string str[],int num);
void score_sort(float score[],string str[],int num);
void swap_teams(string str[],int j, int smallest);
void swap_score(int score[],int j, int smallest);
//file handling functions
void writeInFile(fstream &file,string str);
void fileStandingsTable(fstream &file,string teams[],int scores[],int num);
void fileStandingsTable(fstream &file,string teams[],float scores[],int num);
void fileFootballStandingsTable(fstream &file,string teams[],float scores[],int w[],int d[],int l[],int gf[],int ga[],int gdiff[],int num);
void fileTtValStandings(fstream &file,string teams[],float scores[],int w[],int l[],int sw[],int sl[],int num);
//football
void footballLeague (string teams[],float scores [], int num_teams,fstream &file);
void footballStanding(string teams[],float scores[],int w[],int d[],int l[],int gf[],int ga[],int gdiff[],int num);
void football_sort(float score[],string str[],int w[],int d[],int l[],int gf[],int ga[],int gdiff[],int num);
string penalties(string a,string b);
void footballKnockout(string teams[],int size,fstream &file);
//tt and val
void ttAndValLeague (string teams[],float scores [], int num_teams,fstream &file,char sport);
void ttVal_sort(float score[],string str[],int w[],int l[],int sw[],int sl[],int num);
void ttValStanding(string teams[],float scores[],int w[],int l[],int sw[],int sl[],int num);
void ttValKnockout(string teams[],int size,fstream &file,char sport);


int main(){
   // Variables for input
    string line, filename, tournament_name;
    char sport_type = '\0', format_type = '\0';
    int no_of_teams = 0;

    // Create SFML window
    RenderWindow window(VideoMode(800, 600), "Tournament Manager");

    // Fonts and text
    Font font;
    if (!font.loadFromFile("Roman_New_Times.otf")) { // Ensure a valid font file is provided
        cerr << "Error loading font!" << endl;
        return -1;
    }

    // Text elements
    Text title("Enter Tournament Name:", font, 24);
    title.setPosition(20, 20);
    Text tournamentInput("", font, 24);
    tournamentInput.setPosition(20, 60);

    Text sportPrompt("Select Sport Type (F, T, V, C):", font, 24);
    sportPrompt.setPosition(20, 120);
    Text sportInput("", font, 24);
    sportInput.setPosition(20, 160);

    Text formatPrompt("Select Format Type (S, L, K):", font, 24);
    formatPrompt.setPosition(20, 220);
    Text formatInput("", font, 24);
    formatInput.setPosition(20, 260);

    Text errorText("", font, 24);
    errorText.setFillColor(Color::Red);
    errorText.setPosition(20, 300);

    // Input tracking
    string currentInput;
    int step = 0; // Tracks the current stage (0: tournament name, 1: sport type, 2: format type)
    bool proceedToNext = false;
    bool inputValid = true; // Tracks validity of the current input

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::TextEntered) {
                if (event.text.unicode == '\b' && !currentInput.empty()) {
                    currentInput.pop_back();
                } else if (event.text.unicode < 128 && event.text.unicode != '\b' && event.text.unicode != '\r') {
                    currentInput += static_cast<char>(event.text.unicode);
                } else if (event.text.unicode == '\r') {
                    proceedToNext = true;
                }
            }
        }

        if (proceedToNext) {
            inputValid = true; // Reset validity flag
            errorText.setString(""); // Clear error message

            if (step == 0) { // Tournament name
                tournament_name = currentInput;
                filename = tournament_name + ".txt";
                
                allCaps(tournament_name);
                step++;
            } else if (step == 1) { // Sport type
                if (currentInput.size() == 1) {
                    sport_type = toupper(currentInput[0]);
                    sport_type_validation(sport_type,inputValid);
                    if (inputValid) {
                        step++;
                    } else {
                        errorText.setString("Invalid sport type! Enter F, T, V, or C.");
                    }
                }
            } else if (step == 2) { // Format type
                if (currentInput.size() == 1) {
                    format_type = toupper(currentInput[0]);
                    format_type_validation(format_type,inputValid);
                    if (inputValid) {
                        step++;
                        window.close(); // Close the window after taking the format type
                    } else {
                        errorText.setString("Invalid format type! Enter S, L, or K.");
                    }
                }
            }
            proceedToNext = false;
            currentInput.clear();
        }

        window.clear();
        window.draw(title);

        if (step == 0) {
            tournamentInput.setString(currentInput);
            window.draw(tournamentInput);
        } else if (step == 1) {
            window.draw(sportPrompt);
            sportInput.setString(currentInput);
            window.draw(sportInput);
        } else if (step == 2) {
            window.draw(formatPrompt);
            formatInput.setString(currentInput);
            window.draw(formatInput);
        }

        window.draw(errorText); // Draw error message if any
        window.display();
    }

    fstream file(filename, ios::in | ios::out | ios::app);
    writeInFile(file, header(tournament_name, '|', 3, 50));
    writeInFile(file, ""); // Empty line for formatting

    // Output collected data
    cout << "Tournament Name: " << tournament_name << endl;
    cout << "Sport Type: " << sport_type << endl;
    cout << "Format Type: " << format_type << endl;
    cout<<"Enter the no of teams"<<endl;
    no_of_teams=no_of_teams_validation(no_of_teams,format_type);


    string teams[no_of_teams];
    int score[no_of_teams] = {0};
    float league_score[no_of_teams] = {0};
    string swiss_qualifiers[no_of_teams/2];

    cin.ignore();

    get_unique_teams(teams,no_of_teams);
    cout<<endl;


    writeInFile(file,"Starting Lineup of Teams:");
    cout<<"Starting Lineup"<<endl;
    // starting lineup of teams
    for(int i=0;i<no_of_teams;i++){
        allCaps(teams[i]);
        cout<<"Team "<<i+1<<":  "<<teams[i]<<endl;
        line = to_string(i+1)+".  "+teams[i];
        writeInFile(file,line);
    }
    writeInFile(file,"");
    cout<<endl;


    if(sport_type == 'f'){          // for football
        if(format_type == 's'){         // football + swiss
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
            footballKnockout(swiss_qualifiers,no_of_teams/2,file);
        }
        else if (format_type == 'l' ){         // football + league
            cout<<"Football League Compeition"<<endl;
            cout<<endl;
            writeInFile(file,header("Football League Tournament",'*',2,20));
            writeInFile(file,"");
            footballLeague(teams,league_score,no_of_teams,file);
        }
        else if (format_type == 'k'){       //football + knockout
            cout<<"Football Knockout Compeition"<<endl;
            cout<<endl;
            writeInFile(file,header("Football Knockout Tournament",'*',2,20));
            writeInFile(file,"");
            footballKnockout(teams,no_of_teams,file);
        }
    }
    else if (sport_type == 't' || sport_type == 'v'){           //for table tennis and valorant
        if(format_type == 's'){         // table tennis and valorant + swiss
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
            ttValKnockout(teams,no_of_teams/2,file,sport_type);
        }
        else if (format_type == 'l' ){         // table tennis and valorant + league
            cout<<"League Compeition"<<endl;
            cout<<endl;
            writeInFile(file,header("League Tournament",'*',2,20));
            writeInFile(file,"");
            ttAndValLeague(teams,league_score,no_of_teams,file,sport_type);
        }
        else if (format_type == 'k'){       //table tennis and valorant + knockout
            cout<<"Knockout Compeition"<<endl;
            cout<<endl;
            writeInFile(file,header("Knockout Tournament",'*',2,20));
            writeInFile(file,"");
            ttValKnockout(teams,no_of_teams,file,sport_type);
        }
    }
    else if (sport_type == 'c'){                // for cricket
        if(format_type == 's'){         // cricket + swiss
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
            knockout(teams,no_of_teams/2,file);
        }
        else if (format_type == 'l' ){         // cricket + league
            cout<<"Cricket League Compeition"<<endl;
            cout<<endl;
            writeInFile(file,header("Cricket League Tournament",'*',2,20));
            writeInFile(file,"");
            league(teams,league_score,no_of_teams,file);
        }
        else if (format_type == 'k'){       //cricket + knockout
            cout<<"Knockout Compeition"<<endl;
            cout<<endl;
            writeInFile(file,header("Knockout Tournament",'*',2,20));
            writeInFile(file,"");
            knockout(teams,no_of_teams,file);
        }
    }
    
    file.close(); // file closed
    cout<<"Congratulations! Tournament conducted successfully"<<endl;
    cout<<"Tournament summary has been saved in the file: "<<filename<<endl;
   
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
int score_validation(int n){
        while(true){
        if(cin>>n){
            if(n>=0){
            return n;
            }
            else{
            cout<<"Invalid Input, only positive numbers are allowed"<<endl;
        }
    }
       else{
        cin.clear();
        cin.ignore();
        cout<<"Invalid Input, no letters allowed"<<endl;
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
            cout<<setw(30)<<team_names[i]<<setw(5)<<"-score: "<<setw(5)<<score[i]<<endl;
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
        if(format_type=='S' || format_type=='K'){
            if(cin>>n && n>1 && (n&(n-1))==0){
                return n;
            }
            else{
                cin.clear();
                cin.ignore();
                cout<<"The no of teams entered do not match the swiss/knockout criteria, total teams entered should be powers of 2"<<endl;
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
            result=input_validation(result);

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
            cout<<teams[0]<<" !!!!"<<endl;

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
    file<<left<<setw(4)<<"Pos"<<setw(30)<<"Team"<<setw(10)<<"Wins"<<endl;
    file<<""<<endl;
    for (int x= 0;x<num;x++){
        file<<left<<setw(4)<<to_string(x+1)+"."<<setw(30)<<teams[x]<<setw(10)<<scores[x]<<endl;
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


void footballLeague (string teams[],float scores [], int num_teams,fstream &file)
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
    int goals_for[league_size] = {0};
    int goals_against[league_size] = {0};
    int goal_diff[league_size] = {0};
    int wins[league_size] = {0};
    int loses[league_size] = {0};
    int draws[league_size] = {0};
    int p1goal;
    int p2goal;

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
            writeInFile(file,league_array[p1]+" vs "+league_array[p2]);
            cout<<"Enter goals scored by "<<league_array[p1]<<" ";
            p1goal=score_validation(p1goal);
            cout<<"Enter goals scored by "<<league_array[p2]<<" ";
            p2goal=score_validation(p2goal);

            goals_for[p1] = goals_for[p1] + p1goal;
            goals_for[p2] = goals_for[p2] + p2goal;
            goals_against[p1] = goals_against[p1] + p2goal;
            goals_against[p2] = goals_against[p2] + p1goal;
            goal_diff[p1] = goal_diff[p1] + p1goal - p2goal;
            goal_diff[p2] = goal_diff[p2] + p2goal - p1goal;


            if (p1goal > p2goal){
                m_result = 0;
            }
            else if(p1goal < p2goal){
                m_result = 1;
            }
            else{
                m_result = 2;
            }

            if (m_result == 0)
            {
                league_scores[p1] += 1;
                wins[p1] +=1;
                loses[p2] +=1;
                cout<<league_array[p1]<<" won the match "<<p1goal<<" - "<<p2goal<<endl;
                writeInFile(file,string("( ")+league_array[p1]+" won the match ) -- | "+to_string(p1goal)+" - "+to_string(p2goal)+" |");
            }
            else if (m_result == 1)
            {
                league_scores[p2] += 1;
                wins[p2] +=1;
                loses[p1] +=1;
                cout<<league_array[p2]<<" won the match "<<p2goal<<" - "<<p1goal<<endl;
                writeInFile(file,string("( ")+league_array[p2]+" won the match ) -- | "+to_string(p2goal)+" - "+to_string(p1goal)+" |");
            }
            else
            {
                league_scores[p1] += 0.5;
                league_scores[p2] += 0.5;
                draws[p1] += 1;
                draws[p2] += 1;
                cout<<"match was drawn"<<endl;
                writeInFile(file,"Match drawn! -- | "+to_string(p2goal)+" - "+to_string(p1goal)+" |");
            }
            writeInFile(file,"");

        }
        cout<<"Do you want to display standings after "<<round+1<<" rounds?  (yes/no)"<<endl;
        cin>>display_standings;

        if(display_standings == "yes"){
            string temp_teams[league_size];
            float temp_scores[league_size] = {0};
            int tempgf[league_size] = {0};
            int tempga[league_size] = {0};
            int tempdiff[league_size] = {0};
            int tempw[league_size] = {0};
            int templ[league_size] = {0};
            int tempd[league_size] = {0};
            int temp_num = 0;
            if(num_teams % 2 == 0){
                for (int z =0;z<league_size;z++){
                    temp_teams[z] = league_array[z];
                    temp_scores[z] = league_scores[z];
                    tempgf[z] = goals_for[z];
                    tempga[z] = goals_against[z];
                    tempw[z] = wins[z];
                    templ[z] = loses[z];
                    tempd[z] = draws[z];
                    tempdiff[z] = goal_diff[z];
                }
                temp_num = league_size;
            }
            else{
                for(int z = 0;z<league_size-1;z++){
                    temp_teams[z] = league_array[z];
                    temp_scores[z] = league_scores[z];
                    tempgf[z] = goals_for[z];
                    tempga[z] = goals_against[z];
                    tempw[z] = wins[z];
                    templ[z] = loses[z];
                    tempd[z] = draws[z];
                    tempdiff[z] = goal_diff[z];
                }
                temp_num = league_size-1;
            }
            football_sort(temp_scores,temp_teams,tempw,tempd,templ,tempgf,tempga,tempdiff,temp_num);
            footballStanding(temp_teams,temp_scores,tempw,tempd,templ,tempgf,tempga,tempdiff,temp_num);
        }
        cout << endl;
    }
    cout<<"Final Standings for League:"<<endl;
    if(num_teams%2 == 0){
        football_sort(league_scores,league_array,wins,draws,loses,goals_for,goals_against,goal_diff,league_size);
        footballStanding(league_array,league_scores,wins,draws,loses,goals_for,goals_against,goal_diff,league_size);
        writeInFile(file,header("Final League Table",'-',2,10));
        fileFootballStandingsTable(file,league_array,league_scores,wins,draws,loses,goals_for,goals_against,goal_diff,league_size);
    }
    else{
        football_sort(league_scores,league_array,wins,draws,loses,goals_for,goals_against,goal_diff,league_size-1);
        footballStanding(league_array,league_scores,wins,draws,loses,goals_for,goals_against,goal_diff,league_size-1);
        writeInFile(file,header("Final League Table",'-',2,10));
        fileFootballStandingsTable(file,league_array,league_scores,wins,draws,loses,goals_for,goals_against,goal_diff,league_size-1);
    }

}

void footballStanding(string teams[],float scores[],int w[],int d[],int l[],int gf[],int ga[],int gdiff[],int num){
    cout<<left<<setw(4)<<"Pos"<<setw(20)<<"Team"<<setw(10)<<"Wins"<<setw(10)<<"Draws"<<setw(10)<<"Losses"<<setw(10)<<"Points"<<setw(15)<<"Goals For"<<setw(15)<<"Goals Aga"<<setw(15)<<"Goal Diff"<<endl;
    cout<<endl;
    for (int x= 0;x<num;x++){
        cout<<left<<setw(4)<<to_string(x+1)+"."<<setw(20)<<teams[x]<<setw(10)<<w[x]<<setw(10)<<d[x]<<setw(10)<<l[x]<<setw(10)<<scores[x]<<setw(15)<<gf[x]<<setw(15)<<ga[x]<<setw(15)<<gdiff[x]<<endl;
    }
}


void football_sort(float score[],string str[],int w[],int d[],int l[],int gf[],int ga[],int gdiff[],int num){
    float temp_num = 0;
    string temp_str;
    int tempgf;
    int tempga;
    int tempdiff;
    int tempw;
    int templ;
    int tempd;
    for (int x = 0; x<num;x++){
        for (int y = 0; y<num-1;y++){
            if (score[y] < score[y+1]){
                    temp_num = score[y];
                    temp_str = str[y];
                    tempgf = gf[y];
                    tempga = ga[y];
                    tempdiff = gdiff[y];
                    tempw = w[y];
                    templ = l[y];
                    tempd = d[y];

                    score[y] = score[y+1];
                    str[y] = str[y+1];
                    w[y] = w[y+1];
                    l[y] = l[y+1];
                    d[y] = d[y+1];
                    gf[y] = gf[y+1];
                    ga[y] = ga[y+1];
                    gdiff[y] = gdiff[y+1];


                    score[y+1] = temp_num;
                    str[y+1] = temp_str;
                    w[y+1] = tempw;
                    l[y+1] = templ;
                    d[y+1] = tempd;
                    gf[y+1] = tempgf;
                    ga[y+1] = tempga;
                    gdiff[y+1] = tempdiff;
                }
                else if (score[y] == score[y+1]){
                    if(gdiff[y]<=gdiff[y+1]){
                        temp_num = score[y];
                        temp_str = str[y];
                        tempgf = gf[y];
                        tempga = ga[y];
                        tempdiff = gdiff[y];
                        tempw = w[y];
                        templ = l[y];
                        tempd = d[y];

                        score[y] = score[y+1];
                        str[y] = str[y+1];
                        w[y] = w[y+1];
                        l[y] = l[y+1];
                        d[y] = d[y+1];
                        gf[y] = gf[y+1];
                        ga[y] = ga[y+1];
                        gdiff[y] = gdiff[y+1];


                        score[y+1] = temp_num;
                        str[y+1] = temp_str;
                        w[y+1] = tempw;
                        l[y+1] = templ;
                        d[y+1] = tempd;
                        gf[y+1] = tempgf;
                        ga[y+1] = tempga;
                        gdiff[y+1] = tempdiff;

                    }
                }
        }
    }
}

void fileFootballStandingsTable(fstream &file,string teams[],float scores[],int w[],int d[],int l[],int gf[],int ga[],int gdiff[],int num){
    file<<left<<setw(4)<<"Pos"<<setw(30)<<"Team"<<setw(10)<<"Wins"<<setw(10)<<"Draws"<<setw(10)<<"Losses"<<setw(10)<<"Points"<<setw(15)<<"Goals For"<<setw(15)<<"Goals Aga"<<setw(15)<<"Goal Diff"<<endl;
    file<<""<<endl;
    for (int x= 0;x<num;x++){
        file<<left<<setw(4)<<to_string(x+1)+"."<<setw(30)<<teams[x]<<setw(10)<<w[x]<<setw(10)<<d[x]<<setw(10)<<l[x]<<setw(10)<<scores[x]<<setw(15)<<gf[x]<<setw(15)<<ga[x]<<setw(15)<<gdiff[x]<<endl;
    }
}


void footballKnockout(string teams[],int size,fstream &file)
{
    int num_rounds = 0;
    int result = 0;
    int p1goal = 0;
    int p2goal = 0;
    string penalty_winner;

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
            cout<<"Enter goals scored by "<<teams[y]<<" ";
            p1goal=score_validation(p1goal);
            cout<<"Enter goals scored by "<<teams[size-1-y]<<" ";
            p2goal=score_validation(p2goal);

            string t1 = teams[y];
            string t2 = teams[size-1-y];

            if (p1goal > p2goal){
                result = 1;
            }
            else if(p1goal < p2goal){
                result = 2;
            }
            else{
                result = 3;
                penalty_winner = penalties(t1,t2);
            }

            if (result == 1)
            {
                pl[y] = teams[y];
                writeInFile(file,string("( ")+teams[y]+" won the match ) -- | "+to_string(p1goal)+" - "+to_string(p2goal)+" |");
            }
            else if (result == 2)
            {
                pl[y] = teams[size-1-y];
                 writeInFile(file,string("( ")+teams[size-1-y]+" won the match ) -- | "+to_string(p2goal)+" - "+to_string(p1goal)+" |");
            }
            else{
                pl[y] = penalty_winner;
                writeInFile(file,"Match drawn! -- | "+to_string(p2goal)+" - "+to_string(p1goal)+" |");
                writeInFile(file,penalty_winner+" won on penalties");
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
            cout<<teams[0]<<" !!!!"<<endl;

            writeInFile(file,header("Winner of the Knockout Tournament is: "+teams[0],'!',3,10));
        }

    }while(size>1);

}
int input_validation(int n){
    while(true){
        if(cin>>n){
            if(n==1 || n==2){
                return n;
            }
            else{
                cout<<"Only enter 1 or 2"<<endl;
            }
        }
        else{
            cin.clear();
            cin.ignore();
            cout<<"Please do not type a letter"<<endl;
        }
    }

}
string penalties(string a,string b){
    int r = 0;
    cout<<"who wins on penalties?  1 for Team1 / 2 for Team2"<<endl;
    r = input_validation(r);

    if (r == 1){
        return a;
    }
    else{
        return b;
    }

}

void ttAndValLeague (string teams[],float scores [], int num_teams,fstream &file,char sport)
{
    string display_standings;
    int max = 0;
    int points =0;
    if (sport == 't'){
        max = 11;
    }
    else{
        max = 13;
    }
    
    int league_size = 0;
    if (num_teams % 2 != 0){         //deals with odd number of players
        league_size = num_teams+1;
    }
    else{                   //deals with even number of players
        league_size = num_teams;
    }  

    string league_array[league_size];
    float league_scores[league_size] = {0};
    int sets_won[league_size] = {0};
    int sets_lost[league_size] = {0};
    int wins[league_size] = {0};
    int loses[league_size] = {0};
    int p1set;
    int p2set;

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
            writeInFile(file,league_array[p1]+" vs "+league_array[p2]);
            cout<<"Enter sets won by "<<league_array[p1]<<" ";
            p1set = maxScoreValidation(p1set,4);
            cout<<"Enter sets won by "<<league_array[p2]<<" ";
            p2set = maxScoreValidation(p2set,4);

            sets_won[p1] = sets_won[p1] + p1set;
            sets_won[p2] = sets_won[p2] + p2set;
            sets_lost[p1] = sets_lost[p1] + p2set;
            sets_lost[p2] = sets_lost[p2] + p1set;

            string line;


            if (p1set > p2set){
                m_result = 1;
            }
            else if(p1set < p2set){
                m_result = 2;
            }
            else{
                m_result = noDraws(league_array[p1],league_array[p2],p1set,p2set);
            }

            for (int x = 0;x<p1set+p2set;x++){
                cout<<"Set "<<x+1<<endl;
                cout<<"Points scored by "<<league_array[p1]<<" ";
                points = maxScoreValidation(points,max);
                line = line + to_string(points) +" - ";
                cout<<"Points scored by "<<league_array[p2]<<" ";
                points = maxScoreValidation(points,max);
                line = line + to_string(points) +" |  ";

            }

            if (m_result == 1)
            {
                league_scores[p1] += 1;
                wins[p1] +=1;
                loses[p2] +=1;
                cout<<league_array[p1]<<" won the match "<<p1set<<" - "<<p2set<<endl;
                writeInFile(file,string("( ")+league_array[p1]+" won the match ) -- | "+to_string(p1set)+" - "+to_string(p2set)+" |");
            }
            else if (m_result == 2)
            {
                league_scores[p2] += 1;
                wins[p2] +=1;
                loses[p1] +=1;
                cout<<league_array[p2]<<" won the match "<<p2set<<" - "<<p1set<<endl;
                writeInFile(file,string("( ")+league_array[p2]+" won the match ) -- | "+to_string(p2set)+" - "+to_string(p1set)+" |");
            }
            writeInFile(file,line);
            writeInFile(file,"");

        }
        cout<<"Do you want to display standings after "<<round+1<<" rounds?  (yes/no)"<<endl;
        cin>>display_standings;

        if(display_standings == "yes"){
            string temp_teams[league_size];
            float temp_scores[league_size] = {0};
            int tempsw[league_size] = {0};
            int tempsl[league_size] = {0};
            int tempw[league_size] = {0};
            int templ[league_size] = {0};
            int temp_num = 0;
            if(num_teams % 2 == 0){
                for (int z =0;z<league_size;z++){
                    temp_teams[z] = league_array[z];
                    temp_scores[z] = league_scores[z];
                    tempsw[z] = sets_won[z];
                    tempsl[z] = sets_lost[z];
                    tempw[z] = wins[z];
                    templ[z] = loses[z];
                }
                temp_num = league_size;
            }
            else{
                for(int z = 0;z<league_size-1;z++){
                    temp_teams[z] = league_array[z];
                    temp_scores[z] = league_scores[z];
                    tempsw[z] = sets_won[z];
                    tempsl[z] = sets_lost[z];
                    tempw[z] = wins[z];
                    templ[z] = loses[z];
                }
                temp_num = league_size-1;
            }
            ttVal_sort(temp_scores,temp_teams,tempw,templ,tempsw,tempsl,temp_num);
            ttValStanding(temp_teams,temp_scores,tempw,templ,tempsw,tempsl,temp_num);
        }
        cout << endl;
    }
    cout<<"Final Standings for League:"<<endl;
    if(num_teams%2 == 0){
        ttVal_sort(league_scores,league_array,wins,loses,sets_won,sets_lost,league_size);
        ttValStanding(league_array,league_scores,wins,loses,sets_won,sets_lost,league_size);
        writeInFile(file,header("Final League Table",'-',2,10));
        fileTtValStandings(file,league_array,league_scores,wins,loses,sets_won,sets_lost,league_size);
    }
    else{
        ttVal_sort(league_scores,league_array,wins,loses,sets_won,sets_lost,league_size-1);
        ttValStanding(league_array,league_scores,wins,loses,sets_won,sets_lost,league_size-1);
        writeInFile(file,header("Final League Table",'-',2,10));
        fileTtValStandings(file,league_array,league_scores,wins,loses,sets_won,sets_lost,league_size-1);
    }

}

int maxScoreValidation(int n,int max){
        while(true){
        if(cin>>n){
            if(n>=0 && n<=max){
            return n;
            }
            else{
            cout<<"Invalid Input, input must +ve and be less than or equal to "<<max<<endl;
        }
    }
       else{
        cin.clear();
        cin.ignore();
        cout<<"Invalid Input, input must not be a character "<<max<<endl;
    }
    
    }
}

int noDraws(string a, string b, int &num1,int &num2){
    while (num1 == num2){
        cout<<"draw is not possible enter again"<<endl;
        cout<<"Enter sets won by "<<a<<" ";
        num1 = maxScoreValidation(num1,4);
        cout<<"Enter sets won by "<<b<<" ";
        num2 = maxScoreValidation(num2,4);
    }

    if (num1 > num2){
        return 1;
    }
    else if(num1 < num2){
        return 2;
    }

    return 0;
}

void ttVal_sort(float score[],string str[],int w[],int l[],int sw[],int sl[],int num){
    float temp_num = 0;
    string temp_str;
    int tempsw;
    int tempsl;
    int tempw;
    int templ;
    for (int x = 0; x<num;x++){
        for (int y = 0; y<num-1;y++){
            if (score[y] < score[y+1]){
                    temp_num = score[y];
                    temp_str = str[y];
                    tempsw = sw[y];
                    tempsl = sl[y];
                    tempw = w[y];
                    templ = l[y];

                    score[y] = score[y+1];
                    str[y] = str[y+1];
                    w[y] = w[y+1];
                    l[y] = l[y+1];
                    sw[y] = sw[y+1];
                    sl[y] = sl[y+1];

                    score[y+1] = temp_num;
                    str[y+1] = temp_str;
                    w[y+1] = tempw;
                    l[y+1] = templ;
                    sw[y+1] = tempsw;
                    sl[y+1] = tempsl;
                }
                else if (score[y] == score[y+1]){
                    if(sw[y]<=sw[y+1]){
                        temp_num = score[y];
                        temp_str = str[y];
                        tempsw = sw[y];
                        tempsl = sl[y];
                        tempw = w[y];
                        templ = l[y];

                        score[y] = score[y+1];
                        str[y] = str[y+1];
                        w[y] = w[y+1];
                        l[y] = l[y+1];
                        sw[y] = sw[y+1];
                        sl[y] = sl[y+1];

                        score[y+1] = temp_num;
                        str[y+1] = temp_str;
                        w[y+1] = tempw;
                        l[y+1] = templ;
                        sw[y+1] = tempsw;
                        sl[y+1] = tempsl;
                    }
                }
        }
    }
}

void ttValStanding(string teams[],float scores[],int w[],int l[],int sw[],int sl[],int num){
    cout<<left<<setw(4)<<"Pos"<<setw(30)<<"Team"<<setw(10)<<"Wins"<<setw(10)<<"Losses"<<setw(10)<<"Points"<<setw(15)<<"Sets Won"<<setw(15)<<"Sets Lost"<<endl;
    cout<<endl;
    for (int x= 0;x<num;x++){
        cout<<left<<setw(4)<<to_string(x+1)+"."<<setw(30)<<teams[x]<<setw(10)<<w[x]<<setw(10)<<l[x]<<setw(10)<<scores[x]<<setw(15)<<sw[x]<<setw(15)<<sl[x]<<endl;
    }
}

void fileTtValStandings(fstream &file,string teams[],float scores[],int w[],int l[],int sw[],int sl[],int num){
    file<<left<<setw(4)<<"Pos"<<setw(30)<<"Team"<<setw(10)<<"Wins"<<setw(10)<<"Losses"<<setw(10)<<"Points"<<setw(15)<<"Sets Won"<<setw(15)<<"Sets Lost"<<endl;
    file<<""<<endl;
    for (int x= 0;x<num;x++){
        file<<left<<setw(4)<<to_string(x+1)+"."<<setw(30)<<teams[x]<<setw(10)<<w[x]<<setw(10)<<l[x]<<setw(10)<<scores[x]<<setw(15)<<sw[x]<<setw(15)<<sl[x]<<endl;
    }
}

void ttValKnockout(string teams[],int size,fstream &file,char sport)
{
    int num_rounds = 0;
    int result = 0;
    int p1set = 0;
    int p2set = 0;
    int points = 0;
    int max = 0;

    if (sport == 't'){
        max = 11;
    }
    else{
        max = 13;
    }

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
            cout<<"Enter sets won by "<<teams[y]<<" ";
            p1set = maxScoreValidation(p1set,4);
            cout<<"Enter sets won by "<<teams[size-1-y]<<" ";
            p2set = maxScoreValidation(p2set,4);

            string line;

            if (p1set > p2set){
                result = 1;
            }
            else if(p1set < p2set){
                result = 2;
            }
            else{
                result = noDraws(teams[y],teams[size-1-y],p1set,p2set);
            }

            for (int x = 0;x<p1set+p2set;x++){
                cout<<"Set "<<x+1<<endl;
                cout<<"Points scored by "<<teams[y]<<" ";
                points = maxScoreValidation(points,max);
                line = line + to_string(points) +" - ";
                cout<<"Points scored by "<<teams[size-1-y]<<" ";
                points = maxScoreValidation(points,max);
                line = line + to_string(points) +" |  ";
            }

            if (result == 1)
            {
                pl[y] = teams[y];
                writeInFile(file,string("( ")+teams[y]+" won the match ) -- | "+to_string(p1set)+" - "+to_string(p2set)+" |");
            }
            else if (result == 2)
            {
                pl[y] = teams[size-1-y];
                 writeInFile(file,string("( ")+teams[size-1-y]+" won the match ) -- | "+to_string(p2set)+" - "+to_string(p1set)+" |");
            }
            writeInFile(file,line);
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
            cout<<teams[0]<<" !!!!"<<endl;

            writeInFile(file,header("Winner of the Knockout Tournament is: "+teams[0],'!',3,10));
        }

    }while(size>1);

}