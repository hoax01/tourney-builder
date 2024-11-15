#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <random>


using namespace std;

void knockout(string teams[],int size);
void league (string teams[],int scores [], int size);



int main()
{
    char sport = ' ';
    char tourn_type = ' ';
    int team_num = 0;

    cout<<"Enter number of Teams:";
    cin>>team_num;

    string teams[team_num];
    float scores[team_num];

    for (int x = 0; x<team_num ; x++)
    {
        cout<<"Team name?";
        cin>>teams[x];
        scores[x] = 0;
    }


    return 0;

}


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