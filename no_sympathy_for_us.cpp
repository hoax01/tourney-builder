#include <SFML/Graphics.hpp>  // For SFML graphics (window, rendering, text, etc.)
#include <iostream>            // For console input/output (cin, cout)
#include <cstring> 
#include <iomanip>
#include <cmath>


using namespace std;
using namespace sf;
void score_sort(int score[],string str[],int num);


void handleSwissKnockoutSteps(Event &event, RenderWindow &window, string *teams, int *score, int numTeams,
int &currentMatch, int &currentRound, int &step, string &currentTeam1, string &currentTeam2, string *&knockoutTeams);

void drawSwissKnockoutSteps(RenderWindow &window, Font &font, string *teams, int *score, int numTeams,
int currentMatch, int currentRound, int step, string *&knockoutTeams);


void handleKnockoutSteps(Event &event, RenderWindow &window, string *&knockoutTeams, int *score, int numTeams, 
int &currentMatch, int &currentRound, int &step);

void drawKnockoutStage(RenderWindow &window, Font &font,int numTeams, string *knockoutTeams, int currentMatch, int currentRound, int step);


bool isPowerOfTwo(int num) {
    return (num > 1) && ((num & (num - 1)) == 0);
}

// Validate the number of teams based on the format type
bool validateTeams(int numTeams, const char* formatType, char* errorMessage) {
    if (strcmp(formatType, "Swiss+Knockout") == 0 || strcmp(formatType, "Knockout") == 0) {
        if (!isPowerOfTwo(numTeams)) {
            strcpy(errorMessage, "Error: Swiss and Knockout formats require a power of 2 teams.");
            return false;
        }
    } else if (strcmp(formatType, "League") == 0) {
        if (numTeams < 2) {
            strcpy(errorMessage, "Error: League format requires at least 2 teams.");
            return false;
        }
    }
    return true;
}

int main() {
    RenderWindow window(VideoMode(800, 600), "Tournament Manager");

    Font font;
    if (!font.loadFromFile("Roman_New_Times.otf")) {
        cout << "Error loading font\n";
        return -1;
    }

    char tournamentName[100] = "";
    char sportType[20] = "";
    char formatType[20] = "";
    int numTeams=0;
    char** teamNames = nullptr;
    const int MAX_TEAMS = 16;
    char errorMessage[200] = "";
    int* score = nullptr;
    string* teams = nullptr; 
    static string currentTeam1, currentTeam2;
    static string *knockoutTeams=nullptr;

    static int currentMatch=0;
    static int currentRound=0;

    // Declare error handling variables globally
    bool showError = false;  // Flag to control error display
    string errorMessageDisplay = ""; // Error message string

    int step = 0;
    int teamIndex = 0;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (step == 0) {
                // Input tournament name
                if (event.type == Event::TextEntered) {
                    if (event.text.unicode == '\b' && strlen(tournamentName) > 0)
                        tournamentName[strlen(tournamentName) - 1] = '\0';
                    else if (event.text.unicode < 128 && strlen(tournamentName) < 99)
                        tournamentName[strlen(tournamentName)] = static_cast<char>(event.text.unicode);
                } else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                    step++;
                }
            } else if (step == 1) {
                // Sport type selection
                if (event.type == Event::MouseButtonPressed) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if (mousePos.x > 50 && mousePos.x < 250) {
                        if (mousePos.y > 100 && mousePos.y < 150) strcpy(sportType, "Valorant");
                        if (mousePos.y > 200 && mousePos.y < 250) strcpy(sportType, "Football");
                        if (mousePos.y > 300 && mousePos.y < 350) strcpy(sportType, "Cricket");
                        if (mousePos.y > 400 && mousePos.y < 450) strcpy(sportType, "Table Tennis");
                        if (strlen(sportType) > 0) step++;
                    }
                }
            } else if (step == 2) {
                // Format type selection
                if (event.type == Event::MouseButtonPressed) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if (mousePos.x > 50 && mousePos.x < 250) {
                        if (mousePos.y > 100 && mousePos.y < 150) strcpy(formatType, "Swiss+Knockout");
                        if (mousePos.y > 200 && mousePos.y < 250) strcpy(formatType, "Knockout");
                        if (mousePos.y > 300 && mousePos.y < 350) strcpy(formatType, "League");
                        if (strlen(formatType) > 0) step++;
                    }
                }
            } else if (step == 3) {
                    // Input number of teams
                    if (event.type == Event::TextEntered) {
                        if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                            numTeams = numTeams * 10 + (event.text.unicode - '0');
                        } else if (event.text.unicode == '\b') {
                            numTeams /= 10;
                        }
                    } else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                        if (validateTeams(numTeams, formatType, errorMessage)) {
                            // Dynamically allocate memory based on numTeams
                            teamNames = new char*[numTeams];
                            for (int i = 0; i < numTeams; i++) {
                                teamNames[i] = new char[100];
                                teamNames[i][0] = '\0';   // Each team name can have up to 100 characters
                            }
                            score = new int[numTeams]();
                            teams = new string[numTeams];

                            step++;
                        } else {
                            numTeams = 0; // Reset invalid input
                        }
                    }
                }

             else if (step == 4) {
                    if (event.type == Event::TextEntered) {
                        if (event.text.unicode == '\b' && strlen(teamNames[teamIndex]) > 0) {
                            teamNames[teamIndex][strlen(teamNames[teamIndex]) - 1] = '\0';
                        } else if (event.text.unicode >= 32 && event.text.unicode < 128 && strlen(teamNames[teamIndex]) < 99) {
                            int len = strlen(teamNames[teamIndex]);
                            teamNames[teamIndex][len] = static_cast<char>(event.text.unicode);
                            teamNames[teamIndex][len + 1] = '\0';
                        }
                    } else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                        string currentName = teamNames[teamIndex];

                        if (currentName.empty()) {
                            errorMessageDisplay = "Team name cannot be empty. Please enter a valid name.";
                            showError = true;
                        } else {
                            bool isDuplicate = false;
                            for (int i = 0; i < teamIndex; i++) {
                                if (strcmp(teamNames[i], teamNames[teamIndex]) == 0) {
                                    isDuplicate = true;
                                    break;
                                }
                            }

                            if (isDuplicate) {
                                errorMessageDisplay = "Team name already entered. Please enter a unique name.";
                                showError = true;
                            } else {
                                showError = false;
                                teamIndex++;
                                if (teamIndex == numTeams) {
                                    for (int i = 0; i < numTeams; i++) {
                                        teams[i] = string(teamNames[i]);  // Convert char arrays to strings
                                    }
                                    step++; // Move to the next step after all names are entered
                                }
                            }
                        }
                    }
                }

        else if (step == 5) {
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
         // Debugging output
        step = 6;  // Transition to Step 6
        }
    }
    else if (strcmp(formatType, "Swiss+Knockout") == 0) {
    handleSwissKnockoutSteps(event, window, teams, score, numTeams, currentMatch, currentRound, step, currentTeam1, currentTeam2, knockoutTeams);
        //f(step==8){
            handleKnockoutSteps(event,window,knockoutTeams,score, numTeams, currentMatch,currentRound,step);
        
    }





        }

        // Drawing logic
        window.clear();

        Text prompt("", font, 20);
        prompt.setPosition(50, 50);

        if (step == 0) {
            prompt.setString("Enter Tournament Name: " + string(tournamentName));
        } else if (step == 1) {
            prompt.setString("Select Sport Type:\n\n\n\n1. Valorant\n\n\n\n2. Football\n\n\n\n3. Cricket\n\n\n\n4. Table Tennis");
        } else if (step == 2) {
            prompt.setString("Select Format Type:\n\n\n\n1. Swiss+Knockout\n\n\n\n2. Knockout\n\n\n\n3. League");
        } else if (step == 3) {
            prompt.setString("Enter Number of Teams: " + to_string(numTeams));
            if (strlen(errorMessage) > 0) {
                Text errorText(errorMessage, font, 20);
                errorText.setPosition(50, 150);
                errorText.setFillColor(Color::Red);
                window.draw(errorText);
            }
        } else if (step == 4) {
            // Draw team name prompt and input
            prompt.setString("Enter Team Name " + to_string(teamIndex + 1) + ": " + string(teamNames[teamIndex]));
            window.draw(prompt);

            // Display error message if needed
            if (showError) {
                Text errorText(errorMessageDisplay, font, 20);
                errorText.setPosition(50, 150); // Adjust position as needed
                errorText.setFillColor(Color::Red);
                window.draw(errorText);
            }
        }
            else if (step == 5) {
                // Calculate number of columns
                int numColumns = (numTeams > 20) ? 3 : 2;  // Use 3 columns if there are more than 20 teams
                int rowsPerColumn = (numTeams + numColumns - 1) / numColumns;  // Rows per column (round up)

                // Display tournament details
                string summary = "Tournament Name: " + string(tournamentName) + "\n";
                summary += "Sport Type: " + string(sportType) + "\n";
                summary += "Format Type: " + string(formatType) + "\n";
                summary += "Number of Teams: " + to_string(numTeams) + "\n";

                // Draw the tournament details at the top
                Text summaryText(summary, font, 20);
                summaryText.setPosition(50, 20);  // Position at the top of the screen
                window.draw(summaryText);

                // Display team names in multiple columns
                float columnWidth = 200;       // Width of each column
                float rowHeight = 30;          // Height of each row
                float startX = 50;             // Starting X position for the first column
                float startY = 150;            // Starting Y position for the first row

                for (int i = 0; i < numTeams; i++) {
                    // Calculate column and row for the current team
                    int column = i / rowsPerColumn;
                    int row = i % rowsPerColumn;

                    // Calculate the position for the current team
                    float x = startX + column * columnWidth;
                    float y = startY + row * rowHeight;

                    // Create and draw the team name text
                    string teamText = to_string(i + 1) + ". " + string(teamNames[i]);
                    Text teamNameText(teamText, font, 20);
                    teamNameText.setPosition(x, y);
                    window.draw(teamNameText);
                }

                // Wait for the user to press Enter to continue to the next step
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                    step = 6; // Proceed to Step 6
                }
            }

            else if (strcmp(formatType, "Swiss+Knockout") == 0) {
        drawSwissKnockoutSteps(window, font, teams, score, numTeams, currentMatch, currentRound, step, knockoutTeams);
            //if(step ==8){
                drawKnockoutStage(window,font,numTeams,knockoutTeams,currentMatch,currentRound,step);
            
        }









    




    

        window.draw(prompt);
        window.display();
    }
    for(int i=0;i<4;i++){
        cout<<knockoutTeams[i]<<endl;
    }
    return 0;
}




void score_sort(int score[],string str[],int num){
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


void handleSwissKnockoutSteps(Event &event, RenderWindow &window, string *teams, int *score, int numTeams,
int &currentMatch, int &currentRound, int &step, string &currentTeam1, string &currentTeam2, string *&knockoutTeams) {
    static int rounds = log2(numTeams);  // Calculate total number of rounds
    static bool initialized = false;    // Ensure teams are initialized only once

    // Initialize teams and scores for the Swiss rounds (Step 6)
    if (step == 6 && !initialized) {
        for (int i = 0; i < numTeams; i++) {
            score[i] = 0;  // Reset scores
        }
        initialized = true;
    }

    // Step 6: Swiss Rounds
    if (step == 6) {
        // Handle match results
        if (event.type == Event::MouseButtonPressed) {
    Vector2i mousePos = Mouse::getPosition(window);  // Pass the window to calculate position

    // Team 1 button click
    if (mousePos.x > 50 && mousePos.x < 200 && mousePos.y > 150 && mousePos.y < 200) {
        score[currentMatch * 2] += 1;  // Team 1 wins
        currentMatch++;               // Move to the next match
    }
    // Team 2 button click
    else if (mousePos.x > 250 && mousePos.x < 400 && mousePos.y > 150 && mousePos.y < 200) {
        score[currentMatch * 2 + 1] += 1;  // Team 2 wins
        currentMatch++;                    // Move to the next match
    }

    // Update the next match's teams
    if (currentMatch * 2 < numTeams) {
        currentTeam1 = teams[currentMatch * 2];
        currentTeam2 = teams[currentMatch * 2 + 1];
    }

    // Check if all matches in the round are completed
    if (currentMatch >= numTeams / 2) {
        currentMatch = 0;  // Reset match count
        currentRound++;    // Move to the next round

        // Sort teams by score for the next round
        score_sort(score, teams, numTeams);

        // Reinitialize the first match's teams for the next round
        if (currentRound < log2(numTeams)) {
            currentTeam1 = teams[0];
            currentTeam2 = teams[1];
        } else {
            knockoutTeams = new string[numTeams / 2];
            for (int i = 0; i < numTeams / 2; i++) {
                knockoutTeams[i] = teams[i]; // Copy top half teams to knockoutTeams
             }
            step=7;  // Proceed to Step 7 after all rounds are completed
        }
    }
}

    }

    // Step 7: Display Teams Advancing to Knockout
    else if (step == 7) {
        static bool sorted = false;  // Ensure teams are sorted only once
        if (!sorted) {
            score_sort(score, teams, numTeams);  // Sort teams by score
            sorted = true;
        }

        // Move to Step 8 on Enter key
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
            step++;
        }
    }  

}








void drawSwissKnockoutSteps(RenderWindow &window, Font &font, string *teams, int *score, int numTeams,
int currentMatch, int currentRound, int step, string *&knockoutTeams) {
    if (step == 6) {
        // Display the current round and matchup
        string roundInfo = "Round " + to_string(currentRound + 1) + "\n";
        if (currentMatch * 2 < numTeams) {
            roundInfo += teams[currentMatch * 2] + " vs " + teams[currentMatch * 2 + 1] + "\n";
        } else {
            roundInfo += "Waiting for next round...\n";
        }

        Text roundText(roundInfo, font, 20);
        roundText.setPosition(50, 50);
        window.draw(roundText);

        // Create and draw Team 1 button
        RectangleShape team1Button(Vector2f(150, 50));
        team1Button.setFillColor(Color::Green);
        team1Button.setPosition(50, 150);
        Text team1Text(teams[currentMatch * 2], font, 20);
        team1Text.setPosition(75, 160);
        window.draw(team1Button);
        window.draw(team1Text);

        // Create and draw Team 2 button
        RectangleShape team2Button(Vector2f(150, 50));
        team2Button.setFillColor(Color::Red);
        team2Button.setPosition(250, 150);
        Text team2Text(teams[currentMatch * 2 + 1], font, 20);
        team2Text.setPosition(275, 160);
        window.draw(team2Button);
        window.draw(team2Text);
    }

    if (step == 7) {
        // Teams advancing to Knockout display
        int knockoutTeamsCount = numTeams / 2;
        string knockoutInfo = "Teams Advancing to Knockout Stage:\n";
        for (int i = 0; i < knockoutTeamsCount; i++) {
            knockoutInfo += to_string(i + 1) + ". " + knockoutTeams[i] + "\n";
        }

        Text knockoutText(knockoutInfo, font, 20);
        knockoutText.setPosition(50, 50);
        window.draw(knockoutText);

        Text proceedText("Press Enter to begin Knockout Stage", font, 20);
        proceedText.setPosition(50, 500);
        window.draw(proceedText);
    }
}


void handleKnockoutSteps(Event &event, RenderWindow &window, string *&knockoutTeams, int *score, int numTeams, 
int &currentMatch, int &currentRound, int &step) {
    static bool initializedKnockout = false;
    static string *nextRoundTeams = nullptr;  // Temporary array for the next round's teams
    static int knockoutTeamsCount = numTeams / 2;
    
    // Initialize knockout teams (step 7)
    if (!initializedKnockout) {
        knockoutTeams = new string[knockoutTeamsCount];  // Allocate memory for knockout teams
        nextRoundTeams = new string[knockoutTeamsCount / 2];  // For next round teams
        for (int i = 0; i < knockoutTeamsCount; i++) {
            knockoutTeams[i] = knockoutTeams[i];  // Take the top half of teams for knockout
        }
        currentMatch = 0;  // Reset match counter
        currentRound = 0;  // Reset round counter
        initializedKnockout = true;
    }

    // Step 8: Knockout rounds
    if (step == 8) {
        if (event.type == Event::MouseButtonPressed) {
            Vector2i mousePos = Mouse::getPosition(window);

            // Team 1 button click
            if (mousePos.x > 50 && mousePos.x < 200 && mousePos.y > 150 && mousePos.y < 200) {
                nextRoundTeams[(currentMatch*2) / 2] = knockoutTeams[currentMatch * 2];
                currentMatch++;
            }
            // Team 2 button click
            else if (mousePos.x > 250 && mousePos.x < 400 && mousePos.y > 150 && mousePos.y < 200) {
                nextRoundTeams[(currentMatch*2) / 2] = knockoutTeams[currentMatch * 2 + 1];
                currentMatch++;
            }

            // After all matches in the current round are completed
            if (currentMatch >= knockoutTeamsCount / 2) {
                if (knockoutTeamsCount > 2){
                currentMatch = 0;  // Reset match count for the next round
               knockoutTeamsCount /= 2;  // Half the number of teams for the next round
                currentRound++;  // Increment round count

                // Update knockoutTeams for the next round
                for (int i = 0; i < knockoutTeamsCount; i++) {
                    knockoutTeams[i] = nextRoundTeams[i];
                }
            }// If all rounds are completed, proceed to Step 9 (final match)
                if (knockoutTeamsCount == 2 && currentMatch >= 1) {
                    currentMatch=0;
                        step = 9;  // Move to final match
                    }
            }
        }
    }
    
    // Step 9: Final match logic
    if (step == 9) {
        // Final match logic: Display the last 2 teams
        if (knockoutTeams[0] != "" && knockoutTeams[1] != "") {
            if (event.type == Event::MouseButtonPressed) {
                Vector2i mousePos = Mouse::getPosition(window);
                // Team 1 button click (final match)
                if (mousePos.x > 50 && mousePos.x < 200 && mousePos.y > 150 && mousePos.y < 200) {
                    // Team 1 wins the final match
                    step = 10;  // Move to Step 10 (after final match)
                }
                // Team 2 button click (final match)
                else if (mousePos.x > 250 && mousePos.x < 400 && mousePos.y > 150 && mousePos.y < 200) {
                    // Team 2 wins the final match
                    step = 10;  // Move to Step 10 (after final match)
                }
            }
        }
    }

}

    

void drawKnockoutStage(RenderWindow &window, Font &font,int numTeams, string *knockoutTeams, int currentMatch, int currentRound, int step) {
    int knockoutTeamsCount = numTeams / 2;
    currentRound=0;
    // Step 8: Knockout round
    if (step == 8) {
        string knockoutInfo = "Knockout Round " + to_string(currentRound + 1) + "\n";

        // Display current match or final match
        if (currentMatch * 2 <= knockoutTeamsCount) {
            knockoutInfo += knockoutTeams[currentMatch * 2] + " vs " + knockoutTeams[currentMatch * 2 + 1] + "\n";
        } else {
            knockoutInfo += "Waiting for next match...\n";  // Display a message if all matches have been completed
        }

        Text knockoutText(knockoutInfo, font, 20);
        knockoutText.setPosition(50, 50);
        window.draw(knockoutText);

        // Create and draw Team 1 button (left side)
        RectangleShape team1Button(Vector2f(150, 50));
        team1Button.setFillColor(Color::Green);
        team1Button.setPosition(50, 150);

        if (currentMatch * 2 < knockoutTeamsCount) {
            Text team1Text(knockoutTeams[currentMatch * 2], font, 20);  // Display team name dynamically
            team1Text.setPosition(75, 160);
            window.draw(team1Button);
            window.draw(team1Text);
        }

        // Create and draw Team 2 button (right side)
        RectangleShape team2Button(Vector2f(150, 50));
        team2Button.setFillColor(Color::Red);
        team2Button.setPosition(250, 150);

        if (currentMatch * 2 + 1 < knockoutTeamsCount) {
            Text team2Text(knockoutTeams[currentMatch * 2 + 1], font, 20);  // Display team name dynamically
            team2Text.setPosition(275, 160);
            window.draw(team2Button);
            window.draw(team2Text);
        }
    }
     // Step 9: Final match display
    if (step == 9) {
        // Display the final match
        string finalMatchInfo = "Final Match: " + knockoutTeams[0] + " vs " + knockoutTeams[1];
        Text finalText(finalMatchInfo, font, 20);
        finalText.setPosition(50, 50);  // Position it as needed
        window.draw(finalText);

        // Create and draw Team 1 button (green, left side)
        RectangleShape finalTeam1Button(Vector2f(150, 50));
        finalTeam1Button.setFillColor(Color::Green);
        finalTeam1Button.setPosition(50, 150);
        Text finalTeam1Text(knockoutTeams[0], font, 20);  // Display Team 1 name
        finalTeam1Text.setPosition(75, 160);
        window.draw(finalTeam1Button);
        window.draw(finalTeam1Text);

        // Create and draw Team 2 button (red, right side)
        RectangleShape finalTeam2Button(Vector2f(150, 50));
        finalTeam2Button.setFillColor(Color::Red);
        finalTeam2Button.setPosition(250, 150);
        Text finalTeam2Text(knockoutTeams[1], font, 20);  // Display Team 2 name
        finalTeam2Text.setPosition(275, 160);
        window.draw(finalTeam2Button);
        window.draw(finalTeam2Text);
    }
    if (step == 10) {
                // Display "Winner" message
                string winnerMessage = "Congratulations!\nWinner: " + knockoutTeams[0];
                  Text winnerText(winnerMessage, font, 40);  // Large font for emphasis
                winnerText.setFillColor(Color::Yellow);    // Bright color for celebration
                winnerText.setStyle(Text::Bold | Text::Underlined);
                winnerText.setPosition(100, 200);          // Center it on the screen
                window.draw(winnerText);

                // Optional: Add a "Thank you" message
                Text thankYouText("Thank you for playing!", font, 30);
                thankYouText.setFillColor(Color::White);
                thankYouText.setPosition(150, 400);
                window.draw(thankYouText);
            }

}
    





