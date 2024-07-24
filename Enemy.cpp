#include "BB.hpp"

using namespace std;
using namespace colors;
using namespace characters;
using namespace cursor_positions;

int last_hit[2]   = {-1, -1};
int next_guess[2] = {-1, -1};

// true if computer hit ship and that ship hasn't sunk
bool boat_afloat = false;

// if last hit guess true hor, false ver
bool last_hit_dir = rand() % 2;

// generates enemy boats
void GenEnemyBoats()
{
    string * boatView;
    int x, y; 
    bool hor;

    srand(time(NULL));
    cout << alert;
    for (int i = 0; i < num_boats; i++) {
        do {
            hor = rand() % 2;
            x = hor ? rand() % (width - boatSizes[i]+1) : rand() % width;
            y = hor ? rand() % height : rand() % (height - boatSizes[i]);

        } while (Collision(i, x, y, hor, false));
        compBoatPos[i][0] = hor ? 0 : 1;
        compBoatPos[i][1] = x;
        compBoatPos[i][2] = y;
        compSet[i] = true;
    }

}

// checks if a boat could fit where computer guesses to make more intelligent guesses
bool BoatFit(int * attackPos)
{
    int x = attackPos[0];
    int y = attackPos[1];
    int room1 = 0;
    int room2 = 0;

    // right down
    while (x < width-1 && y < height-1 && comp_attack_grid[x][y] != 'X'){
        if (last_hit_dir)
            x++;
        else
            y++;
        room1++;
    }

    x = attackPos[0];
    y = attackPos[1];

    // left up
    while (x > 0 && y > 0 && comp_attack_grid[x][y] != 'X'){
        if (last_hit_dir)
            x--;
        else
            y--;
        room2++;
    }

    room1 = room1 < room2 ? room2 : room1;

    for (int i = 0; i < num_boats; i++) {
        if (!playSunk[i]) {
            if (room1 >= boatSizes[i])
                return true;
        }
    }
    return false;
}

// after hitting a boat, generates next guess to further damage it
bool NextGuess()
{
    int x = last_hit[0];
    int y = last_hit[1];

    // left right up down
    if (last_hit_dir) {
        next_guess[1] = y;
        for (int l = x-1; l >= 0 && comp_attack_grid[l][y] != 'M' && comp_attack_grid[l][y] != 'X'; l--) {
            if (comp_attack_grid[l][y] == dot) {
                next_guess[0] = l;
                return true;
            }
        }
        for (int r = x+1; r < width && comp_attack_grid[r][y] != 'M' && comp_attack_grid[r][y] != 'X'; r++) {
            if (comp_attack_grid[r][y] == dot) {
                next_guess[0] = r;
                return true;
            }
        }
        last_hit_dir = false;
    }
    if (!last_hit_dir) {
        next_guess[0] = x;
        for (int d = y-1; d >= 0 && comp_attack_grid[x][d] != 'M' && comp_attack_grid[x][d] != 'X'; d--) {
            if (comp_attack_grid[x][d] == dot) {
                next_guess[1] = d;
                return true;
            }
        }
        for (int u = y+1; u < height && comp_attack_grid[x][u] != 'M' && comp_attack_grid[x][u] != 'X'; u++) {
            if (comp_attack_grid[x][u] == dot) {
                next_guess[1] = u;
                return true;
            }
        }
    }
    last_hit_dir = true;
    NextGuess();
    return false;
}

// after sinking a boat, scans grid to go back to any other boats damaged in the process
bool Scan()
{
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (comp_attack_grid[i][j] == 'H') {
                boat_afloat = true;
                last_hit[0] = i;
                last_hit[1] = j;
                NextGuess();
                return true;
            }
        }
    }
    return false;
}

// computes computer attack
void ComputerTurn()
{
    int attackPos[2] = {0, 0};
    int hit;

    if (!boat_afloat) {
        do {
        attackPos[0] = rand() % width;
        attackPos[1] = rand() % height;
        } while (PrevHit(attackPos, false) || !BoatFit(attackPos));
        
        last_hit_dir = rand() % 2;
    }
    else {
        attackPos[0] = next_guess[0];
        attackPos[1] = next_guess[1];
    }

    hit = Hit(attackPos, false);
    
    cout << "\e[" << to_string(attackPos[1]+5) << ";" << to_string(2*attackPos[0]+top_left_coords[0]) << "H";

    if (hit == -2) {
        comp_attack_grid[attackPos[0]][attackPos[1]] = 'M';
        cout << white << 'M' << alert << "Computer Miss!";
        if (boat_afloat)
            NextGuess();
    }
    else if (hit > -1) {
        comp_attack_grid[attackPos[0]][attackPos[1]] = 'H';
        cout << red << 'H' << alert << "Computer Hit!";
        last_hit[0] = attackPos[0];
        last_hit[1] = attackPos[1];
        NextGuess();
        boat_afloat = true;
    }
    else {
        boat_afloat = false;
        Scan();
    }
    
    if (getch() == 'x') 
        battle = false;
    cout << alert << "                  ";
}