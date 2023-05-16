#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

int main(void) {

    int n;
    cin >> n;
    
    char table[n][n];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> table[i][j];
        }
    }

    cout << "\n\n\n";
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << table[i][j] << " ";
        }
        cout << "\n";
    }

    int pacman_x, pacman_y;
    int ghost_x, ghost_y;

    cin >> pacman_x;
    cin >> pacman_y;
    cin >> ghost_x;
    cin >> ghost_y;

    int qtt_mov;
    cin >> qtt_mov;

    char movs[qtt_mov];

    for (int i = 0; i < qtt_mov; i++) {
        cin >> movs[i];
    }

    
    
}
