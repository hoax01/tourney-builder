#include <SFML/Graphics.hpp>  // For SFML graphics (window, rendering, text, etc.)
#include <iostream>            // For console input/output (cin, cout)
#include <cstring> 
#include <iomanip>
#include <cmath>


using namespace std;
using namespace sf;
void score_sort(int score[],string str[],int num);
// Helper function to check if a number is a power of 2
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
    char teamNames[16][100] = {};
    const int MAX_TEAMS = 16;
    char errorMessage[200] = "";
    int score[16]={0};
    string teams[16];

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
                        step++;
                    } else {
                        numTeams = 0; // Reset invalid input
                    }
                }
            }
            else if (step == 4) {
                // Team name entry with validation
                if (event.type == Event::TextEntered) {
                    // Handle backspace
                    if (event.text.unicode == '\b' && strlen(teamNames[teamIndex]) > 0) {
                        teamNames[teamIndex][strlen(teamNames[teamIndex]) - 1] = '\0';
                    }
                    // Handle valid character input
                    else if (event.text.unicode >= 32 && event.text.unicode < 128 && strlen(teamNames[teamIndex]) < 99) {
                        int len = strlen(teamNames[teamIndex]);
                        teamNames[teamIndex][len] = static_cast<char>(event.text.unicode);
                        teamNames[teamIndex][len + 1] = '\0';
                    }
                } 
                // Handle validation on Enter key
                else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                    string currentName = teamNames[teamIndex];

                    // Check if the name is empty
                    if (currentName.empty()) {
                        errorMessageDisplay = "Team name cannot be empty. Please enter a valid name.";
                        showError = true;
                    } 
                    // Check for duplicate names
                    else {
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
                            // Valid name entered
                            showError = false;
                            teamIndex++;
                            if (teamIndex == numTeams) {
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
           else if (step == 6) {
    static int currentMatch = 0;  // Track the current match within the round
    static int currentRound = 0;  // Track the current round
    static int rounds = log2(numTeams);  // Total number of rounds   // Team scores     // Team names
    static bool initialized = false;

    // Initialize teams and scores (only once)
    if (!initialized) {
        for (int i = 0; i < numTeams; i++) {
            teams[i] = string(teamNames[i]);
        }
        initialized = true;
    }

    // Create a new window for each match
    RenderWindow matchWindow(VideoMode(800, 600), "Match: " + teams[currentMatch * 2] + " vs " + teams[currentMatch * 2 + 1]);

    // Handle event for match window (Clicking Team 1 or Team 2)
    while (matchWindow.isOpen()) {
        Event event;
        while (matchWindow.pollEvent(event)) {
            if (event.type == Event::Closed) {
                matchWindow.close();
            }

            // Check for button clicks (Team 1 or Team 2)
            if (event.type == Event::MouseButtonPressed) {
                Vector2i mousePos = Mouse::getPosition(matchWindow);

                // Team 1 button click
                if (mousePos.x > 50 && mousePos.x < 200 && mousePos.y > 150 && mousePos.y < 200) {
                    score[currentMatch * 2] += 1;  // Team 1 wins
                    currentMatch++;
                    matchWindow.close();  // Close the match window
                }
                // Team 2 button click
                else if (mousePos.x > 250 && mousePos.x < 400 && mousePos.y > 150 && mousePos.y < 200) {
                    score[currentMatch * 2 + 1] += 1;  // Team 2 wins
                    currentMatch++;
                    matchWindow.close();  // Close the match window
                }
            }
        }

        // Drawing logic for each match window
        matchWindow.clear();

        // Display the match-up
        string matchInfo = teams[currentMatch * 2] + " vs " + teams[currentMatch * 2 + 1];
        Text matchText(matchInfo, font, 30);
        matchText.setPosition(250, 100);
        matchWindow.draw(matchText);

        // Create and draw Team 1 button
        RectangleShape team1Button(Vector2f(150, 50));
        team1Button.setFillColor(Color::Green);
        team1Button.setPosition(50, 150);
        matchWindow.draw(team1Button);
        Text team1Text(teams[currentMatch * 2], font, 20);
        team1Text.setPosition(75, 160);
        matchWindow.draw(team1Text);

        // Create and draw Team 2 button
        RectangleShape team2Button(Vector2f(150, 50));
        team2Button.setFillColor(Color::Red);
        team2Button.setPosition(250, 150);
        matchWindow.draw(team2Button);
        Text team2Text(teams[currentMatch * 2 + 1], font, 20);
        team2Text.setPosition(275, 160);
        matchWindow.draw(team2Text);

        matchWindow.display();
    }

    // After the match window closes, check if all matches in the round are completed
    if (currentMatch >= numTeams / 2) {
        currentMatch = 0;  // Reset match count for next round
        currentRound++;    // Move to the next round

        // Sort teams by score for the next round
        score_sort(score, teams, numTeams);

        // If all rounds are completed, move to the next step
        if (currentRound >= rounds) {
            step++;  // Proceed to the next step
        }
    }
}
        else if (step == 7) {
    // Find the team with the highest score
    int winnerIndex = 0;
    for (int i = 1; i < numTeams; i++) {
        if (score[i] > score[winnerIndex]) {
            winnerIndex = i;  // Update winner if a higher score is found
        }
    }
    cout << "Winner Index: " << winnerIndex << ", Winner Team: " << teams[winnerIndex] 
     << ", Score: " << score[winnerIndex] << endl;


    // Create a new window to display the winner
    RenderWindow winnerWindow(VideoMode(800, 600), "Tournament Winner");

    // Event handling for winner window
    while (winnerWindow.isOpen()) {
        Event event;
        while (winnerWindow.pollEvent(event)) {
            if (event.type == Event::Closed)
                winnerWindow.close();
        }

        // Drawing the winner text
        winnerWindow.clear();

        // Display the winner's name and score
        string winnerMessage = "Winner: " + teams[winnerIndex];
        winnerMessage += "\nScore: " + to_string(score[winnerIndex]);  // Correctly append the score

        // Create text to display the winner message
        Text winnerText(winnerMessage, font, 30);
        winnerText.setPosition(100, 200);  // Adjust position as needed
        winnerWindow.draw(winnerText);

        // Display the winner window
        winnerWindow.display();
    }

    // Once winner window is closed, end the tournament (or move to next step)
    step++;  // Proceed to the next step (or exit)
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
        // Display all collected data in step 5
        string summary = "Tournament Name: " + string(tournamentName) + "\n";
        summary += "Sport Type: " + string(sportType) + "\n";
        summary += "Format Type: " + string(formatType) + "\n";
        summary += "Number of Teams: " + to_string(numTeams) + "\n";
        summary += "Teams: \n";

        // Display team names
        for (int i = 0; i < numTeams; i++) {
            summary += to_string(i + 1) + ". " + string(teamNames[i]) + "\n";
        }

        // Create a Text object to display the summary
        Text summaryText(summary, font, 20);
        summaryText.setPosition(50, 50);

        // Draw the summary text
        window.draw(summaryText);

        // Wait for the user to press Enter to continue to the next step
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
            step=6; // Proceed to the next step after displaying the summary
        }
    }else if (step == 6) {
    static int currentMatch = 0;    // Track the current match within the round
    static int currentRound = 0;    // Track the current round
    static int score[16] = {0};     // Team scores
    static string teams[16];        // Team names
    static bool initialized = false;

    // Initialize teams and scores (only once)
    if (!initialized) {
        for (int i = 0; i < numTeams; i++) {
            teams[i] = string(teamNames[i]);
        }
        initialized = true;
    }

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

    // Create and draw Team 1 button with dynamic text
    if (currentMatch * 2 < numTeams) {
        RectangleShape team1Button(Vector2f(150, 50));
        team1Button.setFillColor(Color::Green);
        team1Button.setPosition(50, 150);
        Text team1Text(teams[currentMatch * 2], font, 20);
        team1Text.setPosition(75, 160);
        window.draw(team1Button);
        window.draw(team1Text);

        // Create and draw Team 2 button with dynamic text
        RectangleShape team2Button(Vector2f(150, 50));
        team2Button.setFillColor(Color::Red);
        team2Button.setPosition(250, 150);
        Text team2Text(teams[currentMatch * 2 + 1], font, 20);
        team2Text.setPosition(275, 160);
        window.draw(team2Button);
        window.draw(team2Text);
    }

    // Display standings after the round (if matches are done)
    if (currentMatch == 0 && currentRound > 0) {
        string standings = "Standings after Round " + to_string(currentRound) + ":\n";
        for (int i = 0; i < numTeams; i++) {
            standings += to_string(i + 1) + ". " + teams[i] + " - Score: " + to_string(score[i]) + "\n";
        }

        Text standingsText(standings, font, 20);
        standingsText.setPosition(50, 250);
        window.draw(standingsText);
    }
}



    




    

        window.draw(prompt);
        window.display();
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
