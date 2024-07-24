#include "BB.hpp"

using namespace std;
using namespace characters;
using namespace colors;
using namespace cursor_positions;

string boatColors[] = {green, magenta, cyan, bright_red, yellow, white};

// marks boats on screen
void MarkBoats()
{
    string * boatView = boatHor;
    for (int i = 0; i < num_boats; i++) {
        if (playSet[i]) {
            boatView = playBoatPos[i][0] == 0 ? boatHor : boatVer;
            cout << boatColors[i];
            cout << "\e[" + to_string(playBoatPos[i][2]+5) + ";" + to_string(2*playBoatPos[i][1]+top_left_coords[0]) + "H" << boatView[i]; 
        }
    }
}

// clears set controls to help transition to battle screen
void ClearSetControls()
{
    int i = 1;
    for (string c : set_controls) {
        cout << first_control << "\e[" << i++ << "B";
        for (int l = 0; l < c.length(); l++)
            cout << ' ';
    }
}

// logic for setting boats
void SetBoat() 
{
    string * boatView = boatHor;
    int boatPos[] = {0, 0};
    bool cont = true;
    bool hor  = true;
    
    cout << namePos[boat] << boatColors[boat] << boats[boat]; 

    if (playSet[boat]) {
        playSet[boat] = false;
        boatView      = playBoatPos[boat][0] == 0 ? boatHor : boatVer;
        hor           = playBoatPos[boat][0] == 0;
        boatPos[0]    = playBoatPos[boat][1];
        boatPos[1]    = playBoatPos[boat][2];
        numSet--;

        cout << "\e[" + to_string(boatPos[1]+5) + ";" + to_string(2*boatPos[0]+top_left_coords[0]) + "H";
    }
    else
        cout << player_top_left;
    cout << boatView[boat];

    string boatSpot = "\e[" + to_string(boatPos[1]+5) + ";" + to_string(2*boatPos[0]+top_left_coords[0]) + "H";
    
    while (cont) {
        cout << blue;
        switch (getch()) {
            case 'r':
                // makes sure rotation doesn't cause out of bounds
                if ((hor && boatPos[1] > height-boatSizes[boat]) || (!hor && boatPos[0] > width-boatSizes[boat]))
                    break;
                cout << boatSpot;
                ReplaceBack(boat, hor, dot);
                hor = !hor;
                boatView = hor ? boatHor : boatVer;
                break;
            case 'w':
                boatPos[1] = boatPos[1] > 0 ? --boatPos[1] : boatPos[1];
                break;
            case 's':
                if (!hor)
                    boatPos[1] = boatPos[1] < height-boatSizes[boat] ? ++boatPos[1] : boatPos[1];
                else
                    boatPos[1] = boatPos[1] < height-1 ? ++boatPos[1] : boatPos[1];
                break;
            case 'a':
                boatPos[0] = boatPos[0] > 0 ? --boatPos[0] : boatPos[0];
                break;
            case 'd':
                if (!hor)
                    boatPos[0] = boatPos[0] < width-1 ? ++boatPos[0] : boatPos[0];
                else
                    boatPos[0] = boatPos[0] < width-boatSizes[boat] ? ++boatPos[0] : boatPos[0];
                break;
            case 'p':
                if (Collision(boat, boatPos[0], boatPos[1], hor, true)) {
                    cout << alert << "Error. Can't stack boats.";
                    if (getch() == 'x') {
                        cont = false;
                        set = false;
                        play = false;
                    }
                    cout << alert << "                         ";
                    break;
                }
                else {
                    cont = false;
                    playSet[boat]   = true;
                    playBoatPos[boat][0] = hor ? 0 : 1; 
                    playBoatPos[boat][1] = boatPos[0];
                    playBoatPos[boat][2] = boatPos[1];
                    numSet++;
                }
                
                break;
            case 'u':
                cont = false;
                cout << boatSpot;
                ReplaceBack(boat, hor, dot);
                break;
            case 'x':
                cont = false;
                set = false;
                play = false;
                break;
            default:
                break;
        }
        if (cont) {
            cout << boatSpot << blue;
            ReplaceBack(boat, hor, dot);
            MarkBoats();
            boatSpot = "\e[" + to_string(boatPos[1]+5) + ";" + to_string(2*boatPos[0]+top_left_coords[0]) + "H";
            cout << boatColors[boat] << boatSpot << boatView[boat]; 
        }
        
    }
}

// option for player to generate boats
void GenPlayerBoats()
{
    string * boatView;
    int x, y; 
    bool hor;

    srand(time(NULL));
    for (int i = 0; i < num_boats; i++) {
        if (playSet[i]) {
            cout << "\e[" + to_string(playBoatPos[i][2]+5) + ";" + to_string(2*playBoatPos[i][1]+top_left_coords[0]) + "H" << blue;
            ReplaceBack(i, playBoatPos[i][0] == 0, dot);
            playSet[i] = false;
        }
    }

    for (int i = 0; i < num_boats; i++) {
        do {
            hor = rand() % 2;
            x = hor ? rand() % (width - boatSizes[i]+1) : rand() % width;
            y = hor ? rand() % height : rand() % (height - boatSizes[i]);

        } while (Collision(i, x, y, hor, true));

        playBoatPos[i][0] = hor ? 0 : 1;
        playBoatPos[i][1] = x;
        playBoatPos[i][2] = y;
        playSet[i]    = true;

        boatView = hor ? boatHor : boatVer;

        cout << boatColors[i] << "\e[" + to_string(playBoatPos[i][2]+5) + ";" + to_string(2*playBoatPos[i][1]+top_left_coords[0]) + "H" << boatView[i];
    }
    numSet = 5;
}

// logic for selecting boat to set
// returns true when player is ready to start and has placed every boat on the board
bool SelectBoat()
{
    char c = getch();
    cout << namePos[boat] << boatColors[5] << boats[boat] << boatColors[0];
    switch (c) {
        case 'w':
            if (boat > 0) 
                boat--;
            else
                boat = 4;
            break;
        case 's':
            if (boat < 4)
                boat++;
            else
                boat = 0;
            break;
        case 'x':
            set = false;
            play = false;
            break;
        case 'p':
            if (playSet[boat]) {
                cout << alert << boatColors[boat] << "Boat already set. Relocate? Press 'Y' for yes and any other key for no.";
                if (getch() == 'y') {
                    cout << alert << "                                                                           ";
                    cout << "\e[" << to_string(playBoatPos[boat][2]+5) << ";" << to_string(playBoatPos[boat][1]+top_left_coords[0]) << "H";
                    SetBoat();
                } 
                cout << alert << "                                                                           ";
            }
            else
                SetBoat();
            break;
        case 'g':
            GenPlayerBoats();
            break;
        case 'f':
            if (numSet == 5) {
                ClearSetControls();
                return false;
            }
            else {
                cout << alert << "Make sure to finish placing boats before the battle begins!";
                if (getch() == 'x') {
                    set = false;
                    play = false;
                }
                cout << alert << "                                                           ";
            }
            break;
        default: 
            break;
    }
    cout << namePos[boat] << boatColors[boat] << boats[boat];
    return true;
}