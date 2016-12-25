#ifndef GAME_H
#define GAME_H

#include <string>

#include "FilePath.h"
#include "view.h"
#include "assets.h"

enum Game_state {
    STATE_MENU,
    STATE_GAMEPLAY,
    STATE_QUIT
};

class Game {
public:
    Game(char* application_path);
    ~Game();

    // fonction principale
    int run();

    // le jeu est dans l'état 'menu'
    void menu();

    // leu jeu est dans l'état 'gameplay'
    void gameplay();

	// Pour debugage
	void print();

    Assets assets;
    View view;
    glimac::FilePath application_path;

    Game_state game_state;
};

#endif
