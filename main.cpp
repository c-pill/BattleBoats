#include "Background.cpp"
#include "Battle.cpp"
#include "Enemy.cpp"
#include "Set.cpp"
#include "BB.hpp"

using namespace std;

void Intialize()
{
    play = set = battle = toggle_hidden_controls = true;
    outcome = false;
    boat = numSet = attack_coords[0] = attack_coords[1] = num_play_shots = num_comp_shots = 0;

    for (int i = 0; i < num_boats; i++) {
        for (int j = 0; j < 3; j++)
            playBoatPos[i][j] = compBoatPos[i][j] = -1;
        playSet[i] = compSet[i] = playSunk[i] = compSunk[i] = false;
        compHit[i] = 0;
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++)
            play_attack_grid[i][j] = comp_attack_grid[i][j] = characters::dot;
    }
}

int main()
{
    StartUp();
    play = true;
    while (play) {
        Intialize();        
        Background();
        GenEnemyBoats();

        PhaseBack(0);
        
        while (set && SelectBoat());
            
        PhaseBack(1);

        while (play && battle) {
            cout << aim;
            AttackBoat();
            if (play && battle)
                ComputerTurn();
        }

        cout << player_aim_blink[0];
        End();

        cout << middle_label << "Play Again?";
        cout << middle_label << "\e[B - Press 'Y' to continue.";
        cout << middle_label << "\e[2B - Press any other key to exit.";

        play = getch() == 'y';

        system("cls");
    }
    cout << "Rip boats" << toggle_cursor[1];
    return 0;
}