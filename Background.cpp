#include <iostream>
#include <stdio.h>
#include "AstWord.cpp"
#include "BB.hpp"

using namespace std;
using namespace colors;
using namespace characters;
using namespace cursor_positions;

string set_controls[]   = {
    "- w s a d: up down left right",
    "- p: place selected boat (once positioned on grid press again to set)",
    "- r: rotate boat 90 degrees",
    "- g: generates board for player (will wipe out placed boats)",
    "- u: undos selection",
    "- f: when all boats are set press to begin game", 
    "- x: quit game"
};

string battle_controls[]   = {
    "- w s a d: up down left right",
    "- l: launch attack at desired location",
    "- x: quit game"
};

// creates start screen
void StartUp()
{
    cout << toggle_cursor[0];
    system("cls");
    cout << endl;
    Title();
    cout << "\t\t\t\e[s";
    while (!_kbhit()) {
        usleep(200000);
        cout << "\e[uPress any key to begin!";
        usleep(200000);
        cout << "\e[u                       ";
    }
    cout << "\e[H";
    system("cls");
}

// draws default background
void Background() 
{
    cout << blue << player_label << "Player";
    cout << red << computer_label << "Computer";

    cout << blue << player_top_left << "\e[2D" << "\e[A";
    for (int i = -1; i <= width; i++)
        cout << "# ";
    cout << red << computer_top_left << "\e[2D" << "\e[A";
    for (int i = -1; i <= width; i++)
        cout << "# ";

    for (int i = 1; i <= height; i++) {
        cout << blue << player_top_left << "\e[2D" << "\e[A" << "\e[" << i << "B" << "# ";
        for (int j = 0; j < width; j++)
            cout << char(249) << ' ';
        cout << "#";
        cout << red << computer_top_left << "\e[2D" << "\e[A" << "\e[" << i << "B" << "# "; 
        for (int j = 0; j < width; j++)
            cout << char(249) << ' ';
        cout << '#';
    }

    cout << blue << player_top_left << "\e[2D" << "\e[" << height << "B";
    for (int i = -1; i <= width; i++)
        cout << "# ";
    cout << red << computer_top_left << "\e[2D" << "\e[" << height << "B";
    for (int i = -1; i <= width; i++)
        cout << "# ";

    cout << player_top_left;
}

// draws background for each phase
// phase = 0 for set, 1 for battle
void PhaseBack(int phase) 
{
    cout << white << phase_label;
    
    phase == 0 ? cout << "Set UP!" : cout << "BATTLE!";
    cout << middle_label;
    phase == 0 ? cout << "Choose boat to set:" << namePos[0] << magenta << boats[0] << white : cout << "Boats in the water:";
    
    for (int i = phase == 0 ? 1 : 0; i < num_boats; i++)
        cout << namePos[i] << boats[i];

    cout << control_label << "Controls:";
    
    int i = 1;
    if (phase == 0) {
        for (string c : set_controls)
            cout << first_control << "\e[" << i++ << "B" << c;
        cout << namePos[0];
    }
    else {
        for (string c : battle_controls)
            cout << first_control << "\e[" << i++ << "B" << c;
    }   

    for (int i = 0; i < num_boats && phase == 1; i++)
        cout << play_boat_life[i] << blue << dot << comp_boat_life[i] << red << dot;
}

// creates game over screen. player true if player wins
void End()
{
    system("cls");
    outcome ? cout << blue : cout << red; 
    GameOver();

    while (!_kbhit()) {
        usleep(400000);
        cout << middle_label;
        outcome ? cout << "Player Wins!!!" : cout << "Computer Wins!!";
        usleep(400000);
        cout << middle_label <<"               ";
    }
    _getch();
    cout << "\eb[H";
    system("cls");
}