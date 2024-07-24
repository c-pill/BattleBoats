#ifndef BB_HPP
#define BB_HPP

#include <conio.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

using namespace std;

// both must be at least 10 and no greater than 60
const int height = 10;
const int width = 10;

// 0 off 1 on
string player_aim_blink[] = {"\e[1;34;25m", "\e[1;34;5m"};
string toggle_cursor[]    = {"\e[?25l", "\e[?25h"};

string boats[]   = {"CRUISER", "DESTROYER", "BATTLEBOAT!", "CARRIER", "SUBMARINE"};
string namePos[] = {"\e[9;73H", "\e[10;72H", "\e[11;71H", "\e[12;73H", "\e[13;72H"};
string play_boat_life[] = {"\e[9;69H", "\e[10;69H", "\e[11;69H", "\e[12;69H", "\e[13;69H"};
string comp_boat_life[] = {"\e[9;83H", "\e[10;83H", "\e[11;83H", "\e[12;83H", "\e[13;83H"};

// boats displayed on board
string boatHor[] = {"C R", "D E S T R", "B A T T L E", "C A R R Y", "S B"}; 
string boatVer[] = {
    "C\e[B\e[DR",
    "D\e[B\e[DE\e[B\e[DS\e[B\e[DT\e[B\e[DR", 
    "B\e[B\e[DA\e[B\e[DT\e[B\e[DT\e[B\e[DL\e[B\e[DE", 
    "C\e[B\e[DA\e[B\e[DR\e[B\e[DR\e[B\e[DY",
    "S\e[B\e[DB"
};

int boatSizes[] = {2, 5, 6, 5, 2};
const int num_boats = sizeof(boatSizes) / sizeof(int);

// first number is 0 when horizontal, 1 when vertical
// second and third numbers are x,y coords 
int compBoatPos[num_boats][3], playBoatPos[num_boats][3];

// stores where attack lands and the result. [x], [y], 'M' miss, 'H' hit, 'X' sunk
char play_attack_grid[height][width], comp_attack_grid[height][width];

// tracks number of hits on boats
int compHit[num_boats];
int playHit[num_boats];

// used for aiming in attacks
int attack_coords[2];

// x coords of top-lefts of grids
// 0 is player, 1 is computer
int top_left_coords[2] = {61-2*width, 93};

// used to access specific boat
int boat;

int numSet, num_play_shots, num_comp_shots;

bool compSet[num_boats];
bool playSet[num_boats];
bool compSunk[num_boats];
bool playSunk[num_boats];

bool play, set, battle, toggle_hidden_controls;

// true if player wins, else computer wins
bool outcome;

namespace characters {
    char degree = 248;
    char dot    = 249;
    char target = 254;
}

namespace colors { 
    // resets to white foreground and black background
    string reset         = "\e[1;37;40m";

    string black         = "\e[1;30m";
    string black_blue    = "\e[1;30;44m";
    string black_red     = "\e[1;30;41m";
    string blue          = "\e[1;34m";
    string bright_blue   = "\e[1;94m";
    string bright_red    = "\e[1;91m";
    string bright_yellow = "\e[1;93m";
    string cyan          = "\e[1;36m";
    string green         = "\e[1;32m";
    string magenta       = "\e[1;35m";
    string red           = "\e[1;31m";
    string white         = "\e[1;37m";
    string yellow        = "\e[1;33m";
}

namespace cursor_positions {
    string alert             = "\e[30;1H";
    string computer_label    = "\e[2;" + to_string(top_left_coords[1] + width-6) + "H";
    string computer_top_left = "\e[5;" + to_string(top_left_coords[1]) + "H";
    string middle_label      = "\e[7;67H";
    string player_label      = "\e[2;" + to_string(top_left_coords[0] + width-6) + "H";
    string player_top_left   = "\e[5;" + to_string(top_left_coords[0]) + "H";
    string phase_label         = "\e[1;73H";
    string control_label     = "\e[28;100H";
    string first_control     = "\e[29;80H";

    // used to aim in attacks
    string aim = computer_top_left;
}

// makes sure boats don't collide
bool Collision(int boat, int x, int y, bool hor, bool player)
{
    int (*boatPos)[3] = player ? playBoatPos : compBoatPos;
    bool * boatSet = player ? playSet : compSet;
    int potenCoords[boatSizes[boat]][2];

    for (int i = 0; i < boatSizes[boat]; i++) {
        potenCoords[i][0] = hor ? x + i : x;
        potenCoords[i][1] = hor ? y : y + i;
    }

    for (int i = 0; i < 5; i++) {
        if (boatSet[i]) {
            for (int s = 0, takenX = boatPos[i][1], takenY = boatPos[i][2]; s < boatSizes[i]; s++) {
                for (int p = 0; p < boatSizes[boat]; p++) {
                    if (takenX == potenCoords[p][0] && takenY == potenCoords[p][1])
                        return true;
                }
                boatPos[i][0] == 0 ? takenX++ : takenY++; 
            }
        }
    }
    return false;
}

// replaces parts of background after moving boats 
void ReplaceBack(int boat, bool hor, char back)
{
    cout << back;
    for (int i = 0; hor && i < boatSizes[boat]-1; i++)
        cout << ' ' << back;
    for (int i = 0; !hor && i < boatSizes[boat]-1; i++)
        cout << "\e[B\e[D"<< back; 
}

// sets ship as sunk
// used for player if player true, computer if false
void Sunk(int boat, bool player)
{
    string life = player ? comp_boat_life[boat] : play_boat_life[boat];
    int * boatPos = player ? compBoatPos[boat] : playBoatPos[boat];
    char (*sunk_coords)[height] = player ? play_attack_grid : comp_attack_grid; 
    bool * sunk = player ? compSunk : playSunk;
    
    bool over = sunk[boat] = true;

    player ? cout << colors::black_blue : cout << colors::black_red;
    cout << "\e[" << to_string(boatPos[2] + 5) << ";";
    player ? cout << to_string(2*boatPos[1]+top_left_coords[1]) : cout << to_string(2*boatPos[1]+top_left_coords[0]);
    cout << "H";

    ReplaceBack(boat, boatPos[0] == 0, 'X');

    for (int i = 0; i < boatSizes[boat]; i++) 
        boatPos[0] == 0 ? sunk_coords[boatPos[1]+i][boatPos[2]] = 'X' : sunk_coords[boatPos[1]][boatPos[2]+i] = 'X';

    cout << colors:: reset << cursor_positions::alert;

    player ? cout << "COMPUTER " : cout << "PLAYER "; 
    cout << boats[boat] << " SUNK!!!";

    cout << life << ' ';

    if (getch() == 'x') {
        battle = false;
        play = false;
    }

    cout << cursor_positions::alert << "                                   ";

    for (int i = 0; over && i < num_boats; i++)
        over = sunk[i];

    if (over) {
        battle = false;
        outcome = player;
    }
}

// checks if attack hits. Returns boat number if hit, -1 if sunk, -2 if missed 
// used for player if player true, computer if false
int Hit(int * attackPos, bool player)
{   
    int (*boatPos)[3] = player ? compBoatPos : playBoatPos;
    int * boatHit = player ? compHit : playHit;
    bool * sunk = player ? compSunk : playSunk;

    for (int i = 0; i < num_boats; i++) {
        if (sunk[i] == false) {
            int x = boatPos[i][1];
            int y = boatPos[i][2];
            bool hor = boatPos[i][0] == 0;
            for (int s = 0; s < boatSizes[i]; s++) {
                if (x == attackPos[0] && y == attackPos[1]) {
                    boatHit[i] += 1;
                    if (boatHit[i] < boatSizes[i])
                        return i; 
                    else {
                        Sunk(i, player);
                        return -1;
                    }
                }
                hor ? ++x : ++y;
            }
        }
    } 
    return -2;
}

// checks if location was previously attacked
bool PrevHit(int * attackPos, bool player)
{
    char (*hit_coords)[height] = player ? play_attack_grid : comp_attack_grid;
    return hit_coords[attackPos[0]][attackPos[1]] != characters::dot;
}

#endif