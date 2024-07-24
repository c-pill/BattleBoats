#include "BB.hpp"

using namespace std;
using namespace characters;
using namespace colors;
using namespace cursor_positions;

// sunk boats displayed on board
string sunkHor[] = {"X X", "X X X X X", "X X X X X X", "X X X X X", "X X"}; 
string sunkVer[] = {
    "X\e[B\e[DX",
    "X\e[B\e[DX\e[B\e[DX\e[B\e[DX\e[B\e[DX", 
    "X\e[B\e[DX\e[B\e[DX\e[B\e[DX\e[B\e[DX\e[B\e[DX", 
    "X\e[B\e[DX\e[B\e[DX\e[B\e[DX\e[B\e[DX",
    "X\e[B\e[DX"
};

string hiddenControls[] = {
    "- h: toggle hidden controls on screen",
    "- r: reveal comp ships. once revealed click any key to reveal ships",
    "- b: launches a barrage of attacks until one lands"
};

// reveals hidden controls on screen
void HiddenBack(bool toggle_hidden_controls)
{
    int i = 4;
    if (toggle_hidden_controls) {
        for (string hc : hiddenControls)
            cout << first_control << "\e[" << i++ << "B" << hc;
    }
    else {
        for (string hc : hiddenControls) {
            cout << first_control << "\e[" << i++ << "B";
            for (int l = 0; l < hc.length(); l++) 
                cout << ' ';
        }
    }
}

// draws attacks on board
void Mark(int * targetPos) 
{
    switch (play_attack_grid[targetPos[0]][targetPos[1]]) {
        case 'H':
            cout << blue << 'H';
            break;
        case 'X':
            cout << black_blue << 'X' << reset;
            break;
        case 'M':
            cout << white << 'M';
            break;
        default:
            cout << red << dot;
    }
}

// reveals comp ships on board
void RevealComp(bool reveal) 
{
    string * boatView;
    int pos[2];
    for (int i = 0; i < num_boats; i++) {
        if (!compSunk[i]) {
            boatView = compBoatPos[i][0] == 0 ? boatHor : boatVer; 
            cout << "\e[" + to_string(compBoatPos[i][2]+5) + ";" + to_string(2*compBoatPos[i][1]+top_left_coords[1]) + "H"; 
            if (reveal)
                cout << boatView[i];
            else {
                for (int j = 0; j < boatSizes[i]; j++) {
                    pos[0] = compBoatPos[i][0] == 0 ? compBoatPos[i][1] + j : compBoatPos[i][1];
                    pos[1] = compBoatPos[i][0] != 0 ? compBoatPos[i][2] + j : compBoatPos[i][2];
                    cout << "\e[" + to_string(pos[1]+5) + ";" + to_string(2*pos[0]+top_left_coords[1]) + "H";
                    Mark(pos);
                }
            }
        }
    }
    if (reveal) {
        if (getch() == 'x') {
            battle = false;
            play = false;
        }
        else 
            RevealComp(!reveal);
    }
}

// launches attacks until one hits
void Barrage()
{
    int attackPos[2];
    int hit;
    do
    {
        attackPos[0] = rand() % (width - 2);
        attackPos[1] = rand() % height;
        hit = Hit(attackPos, true);
    } while (hit == -2);

    if (hit > -1) {
        play_attack_grid[attackPos[0]][attackPos[1]] = 'H';
        cout << "\e[" + to_string(attackPos[1]+5) + ";" + to_string(2*attackPos[0]+top_left_coords[1]) + "H" << 'H';
    }
    
    cout << alert << "Barrage Landed!!";
    if (getch() == 'x') {
        battle = false;
        play = false;
    }
    cout << alert << "                ";
    
}

bool AttackBoat() 
{    
    int hit = -1;
    bool cont = true;

    cout << player_aim_blink[1] << target;

    while (cont && battle) {
        cout << player_aim_blink[0];
        switch (getch()) {
            case 'w':
                if (attack_coords[1] > 0) {
                    cout << aim;
                    Mark(attack_coords);
                    --attack_coords[1];
                }
                break;
            case 's':
                if (attack_coords[1] < height-1) {
                    cout << aim;
                    Mark(attack_coords);
                    ++attack_coords[1];
                }
                break;
            case 'a':
                if (attack_coords[0] > 0) {
                    cout << aim;
                    Mark(attack_coords);
                    --attack_coords[0];
                }
                break;
            case 'd':
                if (attack_coords[0] < width-1) {
                    cout << aim;
                    Mark(attack_coords);
                    ++attack_coords[0];
                }
                break;
            case 'l':
                cout << red << "\e[" + to_string(attack_coords[1]+5) + ";" + to_string(2*attack_coords[0]+top_left_coords[1]) + "H";
                if (PrevHit(attack_coords, true)) {
                    cout << alert << "Location already attacked. No point in firing twice.";
                    if (getch() == 'x') {
                        battle = false;
                        play = false;
                    }
                    cout << alert << "                                                    ";
                }
                else {
                    hit = Hit(attack_coords, true);
                    if (hit > -1) {
                        play_attack_grid[attack_coords[0]][attack_coords[1]] = 'H';
                        cout << blue << 'H' << alert << "COMPUTER Hit!";
                        if (getch() == 'x') {
                            battle = false;
                            play = false;
                        }
                        cout << alert << "               ";
                    }
                    else if (hit == -2) {
                        play_attack_grid[attack_coords[0]][attack_coords[1]] = 'M';
                        cout << white << 'M' << alert << "Missed!";
                        if (getch() == 'x') {
                            battle = false;
                            play = false;
                        }
                        cout << alert << "       ";
                    }
                    aim = "\e[" + to_string(attack_coords[1]+5) + ";" + to_string(2*attack_coords[0]+top_left_coords[1]) + "H";
                    cout << aim;
                    cont = false;
                }
                break;
            case 'x':
                battle = false;
                cont = false;
                play = false;
                break;
            case 'h':
                HiddenBack(toggle_hidden_controls);
                toggle_hidden_controls = !toggle_hidden_controls;
                break;
            case 'r':
                cout << red;
                RevealComp(true);
                break;
            case 'b':
                Barrage();
            default:
                break;
        }
        if (cont) {
            aim = "\e[" + to_string(attack_coords[1]+5) + ";" + to_string(2*attack_coords[0]+top_left_coords[1]) + "H";
            cout << player_aim_blink[1] << aim << target; 
        }       
    }
    return true;
}