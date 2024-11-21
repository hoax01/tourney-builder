#include <SFML/Graphics.hpp>  // For SFML graphics (window, rendering, text, etc.)
#include <iostream>            // For console input/output (cin, cout)
#include <cstring> 

using namespace std;
using namespace sf;
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
    int numTeams = 0;
    char teamNames[16][100] = {};
    const int MAX_TEAMS = 16;
    char errorMessage[200] = "";

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
            } else if (step == 4) {
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
            step++; // Proceed to the next step after displaying the summary
        }
    }

        window.draw(prompt);
        window.display();
    }

    // Display collected data
    cout << "Tournament Name: " << tournamentName << endl;
    cout << "Sport Type: " << sportType << endl;
    cout << "Format Type: " << formatType << endl;
    cout << "Number of Teams: " << numTeams << endl;
    cout << "Team Names:\n";
    for (int i = 0; i < numTeams; i++) {
        cout << i + 1 << ". " << teamNames[i] << endl;
    }

    return 0;
}
