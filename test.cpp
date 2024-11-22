// #include <SFML/Graphics.hpp>  // For SFML graphics (window, rendering, text, etc.)
// #include <iostream>            // For console input/output (cin, cout)
// #include <string> 
// #include <chrono>
// #include <thread>

// using namespace std;
// using namespace sf;


// int main(){
//     string word;
//     RenderWindow window1(VideoMode(700,700),"Tournament Organizer!");
//     int x = 0;
//     int num_teams;
//     int screen = 0;
//     string a[25];

//     Font font;
//     if (!font.loadFromFile("coolvetica rg.otf")) {
//         cout << "Error loading font\n";
//         return -1;
//     }

//     Text displaytext("",font,30);
//     displaytext.setFillColor(Color::Red);
//     displaytext.setPosition(50,100);

//     Text inputline("",font,30);
//     inputline.setFillColor(Color::Yellow);
//     inputline.setPosition(50,50);
//     inputline.setStyle(Text::Bold);

    

//     // for starting screen
//     while (window1.isOpen()) {
//         Event event;
//         while (window1.pollEvent(event)) {
//             if (event.type == Event::Closed){
//                 window1.close();
//             }
//             if (event.type == Event::TextEntered && screen != 0){
//                 cout<<"letter was pressed"<<endl;
//                 word = word +static_cast<char>(event.text.unicode);
//             }

//             if (event.type == Event::TextEntered && screen == 0){
//                 cout<<"number was entered"<<endl;
//                 num_teams = static_cast<char>(event.text.unicode) - '0';
//                 word = word+to_string(num_teams);
//             }

//             if (event.type == Event::MouseButtonPressed){
//                 cout<<"mouse button pressed"<<endl;
//             }
//             if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter  && screen != 0){
//                 cout<<"new word"<<endl;
//                 a[x] = word;
//                 word = "";
//                 x++;
//             }
//             if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter && screen == 0){
//                 cout<<"stoped integer input"<<endl;
//                 screen ++;
//                 word = "";
//             }


//             if(screen == 0){
//                 displaytext.setString(word);
//                 inputline.setString("enter number of teams: ");
//                 window1.clear(Color::Blue);
//                 window1.draw(inputline);
//                 window1.draw(displaytext);
//                 window1.display();

//             }
//             else if (x<num_teams){
//                 displaytext.setString(word);
//                 inputline.setString("enter Team "+to_string(x+1));
        
//                 window1.clear(Color::Blue);
//                 window1.draw(inputline);
//                 window1.draw(displaytext);
//                 window1.display();
//             }
//             else if (x == num_teams){
//                 window1.clear(Color::Blue);
//                 inputline.setString("THANK YOU FOR ENTERING TEAMS!");
//                 window1.draw(inputline);
//                 window1.display();
//                 this_thread::sleep_for(chrono::seconds(2));
//                 window1.close();
//             }
//         }
//     }

//     for (int x = 0;x<num_teams;x++){
//         cout<<a[x]<<endl;
//     }

//         ///////////////////////////////////////////////////////////////

//     RenderWindow window2(VideoMode(700,700),"Knockout Round");

//     Text round("",font,30);
//     round.setFillColor(Color::Black);
//     round.setPosition(50,100);

//     Text match("",font,20);
//     match.setFillColor(Color::Red);
//     match.setPosition(50,150);

//     Text matchup("",font,20);
//     matchup.setFillColor(Color::Green);
//     matchup.setPosition(50,200);

//     Text winner_input("",font,20);
//     winner_input.setFillColor(Color::Yellow);
//     winner_input.setPosition(50,250);

//     Text result_input("",font,20);
//     result_input.setFillColor(Color::Magenta);
//     result_input.setPosition(50,300);

//     Text match_winner("",font,20);
//     match_winner.setFillColor(Color::Red);
//     match_winner.setPosition(150,200);


//     string round_text;
//     string match_text;
//     string win_input;
//     string matchup_text;
//     string result_text;
//     string match_win;


//     int num_rounds = 0;
//     int result = 0;
//     bool winner = false;
//     screen = 0;
//     int y = 0;

//     while (window2.isOpen()) {
//         Event event;
//         while (window1.pollEvent(event)) {
//             if (event.type == Event::Closed){
//                     window2.close();
//                 }

//             do
//             {
//                 num_rounds = num_teams / 2;
//                 string pl[num_rounds];
//                 if(num_teams != 2){
//                     cout<<"Round of "<<num_teams<<endl;
//                     round_text = round_text+"Round of "+to_string(num_teams);
//                 }
//                 else{
//                     cout<<"Grand Final"<<endl;
//                     round_text = round_text+"Grand Final";
//                 }

//                 while (y<num_rounds && window2.isOpen())
//                 {

//                     if (event.type == Event::Closed){
//                         window2.close();
//                     }
//                     cout<<"Match "<<y+1<<endl;
//                     match_text = match_text+"Match "+to_string(y+1);

//                     cout<<a[y]<<" vs "<<a[num_teams-1-y]<<endl;
//                     matchup_text = matchup_text+a[y]+" vs "+a[num_teams-1-y];

//                     win_input="whats the result of the match?";

//                     if (event.type == Event::TextEntered && screen == 0){
//                         cout<<"number was entered"<<endl;
//                         result = static_cast<char>(event.text.unicode) - '0';
//                         result_text = result_text+to_string(result);
//                     }
                    

//                     if (result == 1)
//                     {
//                         pl[y] = a[y];
//                         match_win = match_win+a[y]+" won the match!";
//                     }
//                     else{
//                         pl[y] = a[num_teams-1-y];
//                         match_win = match_win+a[num_teams-1-y]+" won the match!";
//                     }

//                     if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter && screen == 0){
//                         cout<<"stoped integer input"<<endl;
//                         screen ++;
//                         round_text ="";
//                         match_text ="";
//                         win_input ="";
//                         matchup_text="";
//                         result_text="";
//                     } 

//                     if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter && screen == 1){
//                         screen--;
//                         y++;
//                         match_win = "";
//                     }

//                     if(screen == 0){
//                         round.setString(round_text);
//                         match.setString(match_text);
//                         matchup.setString(matchup_text);
//                         winner_input.setString(win_input);
//                         result_input.setString(result_text);

//                         window2.clear(Color::Blue);
//                         window2.draw(round);
//                         window2.draw(match);
//                         window2.draw(matchup);
//                         window2.draw(winner_input);
//                         window2.draw(result_input);
//                         window2.display();
//                     }
//                     if(screen == 1){
//                         match_winner.setString(match_win);

//                         window2.clear(Color::Blue);
//                         window2.draw(match_winner);
//                         window2.display();
//                     }

                

                    

//                 }

//                 // updating teams array
//                 for (int z = 0; z<num_rounds ; z++){
//                     a[z] = pl[z];
//                 }

//                 num_teams = num_teams / 2;
//                 cout<<endl;

//                 if (num_teams <= 1)
//                 {
//                     cout<<"Winner of the Knockout Tournament is: "<<endl;
//                     cout<<a[0]<<" !!!!"<<endl;
//                 }

//             }while(num_teams>1);
//         }
//     }




//     return 0;

// }

//////////barrier/////////


#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;
using namespace sf;

int main() {
    string word;
    RenderWindow window1(VideoMode(700, 700), "Tournament Organizer!");
    int x = 0;
    int num_teams;
    int screen = 0;
    string a[25]; // Array to store team names

    // Load font for the text display
    Font font;
    if (!font.loadFromFile("coolvetica rg.otf")) {
        cout << "Error loading font\n";
        return -1;
    }

    Text displaytext("", font, 30);
    displaytext.setFillColor(Color::Red);
    displaytext.setPosition(50, 100);

    Text inputline("", font, 30);
    inputline.setFillColor(Color::Yellow);
    inputline.setPosition(50, 50);
    inputline.setStyle(Text::Bold);

    // Screen 1: Input number of teams and team names
    while (window1.isOpen()) {
        Event event;
        while (window1.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window1.close();
            }
            if (event.type == Event::TextEntered && screen != 0) {
                word = word + static_cast<char>(event.text.unicode);
            }

            if (event.type == Event::TextEntered && screen == 0) {
                num_teams = static_cast<char>(event.text.unicode) - '0';
                word = word + to_string(num_teams);
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter && screen != 0) {
                a[x] = word;
                word = "";
                x++;
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter && screen == 0) {
                screen++;
                word = "";
            }

            // Draw input screen
            if (screen == 0) {
                displaytext.setString(word);
                inputline.setString("Enter number of teams: ");
                window1.clear(Color::Blue);
                window1.draw(inputline);
                window1.draw(displaytext);
                window1.display();
            } else if (x < num_teams) {
                displaytext.setString(word);
                inputline.setString("Enter Team " + to_string(x + 1));
                window1.clear(Color::Blue);
                window1.draw(inputline);
                window1.draw(displaytext);
                window1.display();
            } else if (x == num_teams) {
                window1.clear(Color::Blue);
                inputline.setString("THANK YOU FOR ENTERING TEAMS!");
                window1.draw(inputline);
                window1.display();
                this_thread::sleep_for(chrono::seconds(2));
                window1.close();
            }
        }
    }

    // Show entered teams in console
    for (int i = 0; i < num_teams; i++) {
        cout << a[i] << endl;
    }

    // Screen 2: Tournament matches and results
    RenderWindow window2(VideoMode(1200, 700), "Knockout Round");

    Text round("", font, 40);
    round.setFillColor(Color::Black);
    round.setPosition(50, 100);

    Text match("", font, 30);
    match.setFillColor(Color::Red);
    match.setPosition(50, 150);

    Text matchup("", font, 30);
    matchup.setFillColor(Color::Green);
    matchup.setPosition(50, 200);

    Text winner_input("", font, 25);
    winner_input.setFillColor(Color::Yellow);
    winner_input.setPosition(50, 250);

    Text result_input("", font, 30);
    result_input.setFillColor(Color::Magenta);
    result_input.setPosition(50, 300);

    Text match_winner("", font, 30);
    match_winner.setFillColor(Color::Red);
    match_winner.setPosition(150, 200);
    match_winner.setStyle(Text::Bold);

    string round_text;
    string match_text;
    string win_input;
    string matchup_text;
    string result_text;
    string match_win;

    int num_rounds = 0;
    int result = 0;
    bool winner = false;
    int y = 0;
    int screen2_state = 0;

    while (window2.isOpen()) {
        Event event;
        while (window2.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window2.close();
            }

            // Prepare text for round information
            num_rounds = num_teams / 2;
            if (num_teams != 2) {
                round_text = "Round of " + to_string(num_teams);
            } else {
                round_text = "Grand Final";
            }

            // Handle user input for match results (1 or 2)
            if (event.type == Event::TextEntered) {
                if (event.text.unicode == '1') {
                    result = 1;
                    result_text = "1";
                } else if (event.text.unicode == '2') {
                    result = 2;
                    result_text = "2";
                }
            }

            // Handle Enter key to move to next match or winner screen
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                if (screen2_state == 0) {
                    if (result == 1) {
                        a[y] = a[y]; // Winner of match
                        match_win = a[y] + " won the match!";
                    } else if (result == 2) {
                        a[y] = a[num_teams - 1 - y]; // Winner of match
                        match_win = a[num_teams - 1 - y] + " won the match!";
                    } else {
                        match_win = "Invalid input! Please enter 1 or 2.";
                    }
                    screen2_state = 1; // Move to winner screen
                } else if (screen2_state == 1) {
                    // Prepare for the next match
                    y++;
                    if (y < num_rounds) {
                        screen2_state = 0; // Go back to match input screen
                    } else {
                        // Update teams for the next round
                        for (int z = 0; z < num_rounds; z++) {
                            a[z] = a[z];
                        }
                        num_teams = num_teams / 2; // Half the number of teams after each round
                        y = 0; // Reset match index
                        screen2_state = 0;
                    }
                    match_win = "";
                }
            }

            // Render match screen
            if (screen2_state == 0) {
                match_text = "Match " + to_string(y + 1);
                matchup_text = a[y] + " vs " + a[num_teams - 1 - y];
                win_input = "What is the result of the match? (1 = " + a[y] + " wins, 2 = " + a[num_teams - 1 - y] + " wins)";
                result_input.setString(result_text);

                round.setString(round_text);
                match.setString(match_text);
                matchup.setString(matchup_text);
                winner_input.setString(win_input);

                window2.clear(Color::Blue);
                window2.draw(round);
                window2.draw(match);
                window2.draw(matchup);
                window2.draw(winner_input);
                window2.draw(result_input);
                window2.display();
            }

            // Render winner screen
            if (screen2_state == 1) {
                match_winner.setString(match_win);
                window2.clear(Color::Blue);
                window2.draw(match_winner);
                window2.display();
            }

            // Check if tournament has ended
            if (num_teams <= 1) {
                cout << "Winner of the Knockout Tournament is: " << endl;
                cout << a[0] << " !!!!" << endl;
                window2.close();
            }
        }
    }

    return 0;
}
