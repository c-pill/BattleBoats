#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "BB.hpp"

using namespace std;

void Title()
{
    usleep(100000);
    cout << "\t\t\t\e[s";
    cout << "***** " << "\e[u" << "\e[B";
    cout << "*    *" << "\e[u" << "\e[2B";
    cout << "***** " << "\e[u" << "\e[3B";
    cout << "*    *" << "\e[u" << "\e[4B";
    cout << "***** " << "\e[u\t" << "\e[s";

    usleep(100000);
    cout << " **** " << "\e[u" << "\e[B";
    cout << "*    *" << "\e[u" << "\e[2B";
    cout << "******" << "\e[u" << "\e[3B";
    cout << "*    *" << "\e[u" << "\e[4B";
    cout << "*    *" << "\e[u\t" << "\e[s"; 
 
    for (int i = 0; i < 2; i++) {
        usleep(100000);
        cout << "******" << "\e[u" << "\e[B";
        cout << "  **  " << "\e[u" << "\e[2B";
        cout << "  **  " << "\e[u" << "\e[3B";
        cout << "  **  " << "\e[u" << "\e[4B";
        cout << "  **  " << "\e[u\t" << "\e[s";   
    }
    
    usleep(100000);
    cout << "**    " << "\e[u" << "\e[B";
    cout << "**    " << "\e[u" << "\e[2B";
    cout << "**    " << "\e[u" << "\e[3B";
    cout << "**    " << "\e[u" << "\e[4B";
    cout << "******" << "\e[u\t" << "\e[s";   

    usleep(100000);
    cout << " *****" << "\e[u" << "\e[B";
    cout << "*     " << "\e[u" << "\e[2B";
    cout << " **** " << "\e[u" << "\e[3B";
    cout << "*     " << "\e[u" << "\e[4B";
    cout << " *****" << "\e[u\t" << "\e[s";

    usleep(100000);
    cout << "      " << "\e[u" << "\e[B";
    cout << "      " << "\e[u" << "\e[2B";
    cout << "      " << "\e[u" << "\e[3B";
    cout << "      " << "\e[u" << "\e[4B";
    cout << "      " << "\e[u\t" << "\e[s";

    usleep(100000);
    cout << "***** " << "\e[u" << "\e[B";
    cout << "*    *" << "\e[u" << "\e[2B";
    cout << "***** " << "\e[u" << "\e[3B";
    cout << "*    *" << "\e[u" << "\e[4B";
    cout << "*****" << "\e[u\t" << "\e[s";    

    usleep(100000);
    cout << " **** " << "\e[u" << "\e[B";
    cout << "*    *" << "\e[u" << "\e[2B";
    cout << "*    *" << "\e[u" << "\e[3B";
    cout << "*    *" << "\e[u" << "\e[4B";
    cout << " **** " << "\e[u\t" << "\e[s";

    usleep(100000);
    cout << " **** " << "\e[u" << "\e[B";
    cout << "*    *" << "\e[u" << "\e[2B";
    cout << "******" << "\e[u" << "\e[3B";
    cout << "*    *" << "\e[u" << "\e[4B";
    cout << "*    *" << "\e[u\t" << "\e[s"; 

    usleep(100000);
    cout << "******" << "\e[u" << "\e[B";
    cout << "  **  " << "\e[u" << "\e[2B";
    cout << "  **  " << "\e[u" << "\e[3B";
    cout << "  **  " << "\e[u" << "\e[4B";
    cout << "  **  " << "\e[u\t" << "\e[s"; 

    usleep(100000);
    cout << " **** " << "\e[u" << "\e[B";
    cout << "*     " << "\e[u" << "\e[2B";
    cout << "  *   " << "\e[u" << "\e[3B";
    cout << "    * " << "\e[u" << "\e[4B";
    cout << " ***  " << "\e[u\t" << "\e[s"; 

    cout << endl << endl << "\e[4B\t\t\t\t\t";
}

void GameOver()
{
    usleep(100000);
    cout << cursor_positions::player_label << "\e[11D" "\t\e[s";
    cout << " **** " << "\e[u" << "\e[B";
    cout << "*     " << "\e[u" << "\e[2B";
    cout << "*  ***" << "\e[u" << "\e[3B";
    cout << "*    *" << "\e[u" << "\e[4B";
    cout << " **** " << "\e[u\t" << "\e[s"; 

    usleep(100000);
    cout << " **** " << "\e[u" << "\e[B";
    cout << "*    *" << "\e[u" << "\e[2B";
    cout << "******" << "\e[u" << "\e[3B";
    cout << "*    *" << "\e[u" << "\e[4B";
    cout << "*    *" << "\e[u\t" << "\e[s"; 

    cout << "*     *" << "\e[u" << "\e[B";
    cout << "* * * *" << "\e[u" << "\e[2B";
    cout << "*  *  *" << "\e[u" << "\e[3B";
    cout << "*     *" << "\e[u" << "\e[4B";
    cout << "*     *" << "\e[u\t" << "\e[s"; 

    usleep(100000);
    cout << " *****" << "\e[u" << "\e[B";
    cout << "*     " << "\e[u" << "\e[2B";
    cout << " **** " << "\e[u" << "\e[3B";
    cout << "*     " << "\e[u" << "\e[4B";
    cout << " *****" << "\e[u\t" << "\e[s";

    usleep(100000);
    cout << "      " << "\e[u" << "\e[B";
    cout << "      " << "\e[u" << "\e[2B";
    cout << "      " << "\e[u" << "\e[3B";
    cout << "      " << "\e[u" << "\e[4B";
    cout << "      " << "\e[u\t" << "\e[s";

    usleep(100000);
    cout << " **** " << "\e[u" << "\e[B";
    cout << "*    *" << "\e[u" << "\e[2B";
    cout << "*    *" << "\e[u" << "\e[3B";
    cout << "*    *" << "\e[u" << "\e[4B";
    cout << " **** " << "\e[u\t" << "\e[s";

    usleep(100000);
    cout << "*    *" << "\e[u" << "\e[B";
    cout << "*    *" << "\e[u" << "\e[2B";
    cout << "*    *" << "\e[u" << "\e[3B";
    cout << " *  * " << "\e[u" << "\e[4B";
    cout << "  **  " << "\e[u\t" << "\e[s"; 

    usleep(100000);
    cout << " *****" << "\e[u" << "\e[B";
    cout << "*     " << "\e[u" << "\e[2B";
    cout << " **** " << "\e[u" << "\e[3B";
    cout << "*     " << "\e[u" << "\e[4B";
    cout << " *****" << "\e[u\t" << "\e[s";

    usleep(100000);
    cout << " **** " << "\e[u" << "\e[B";
    cout << "*    *" << "\e[u" << "\e[2B";
    cout << "***** " << "\e[u" << "\e[3B";
    cout << "*   * " << "\e[u" << "\e[4B";
    cout << "*    *" << "\e[u\t" << "\e[s"; 
}