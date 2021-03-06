#include "game.h"
#include <iostream>

#include "gui.h"

Game::Game(char* _application_path)
    : assets(_application_path), application_path(_application_path), game_state(STATE_MENU) {
		view.setAssets(assets);
		view.initTextures();
}

Game::~Game() {

}

int Game::run() {
    while(game_state != STATE_QUIT) {
        view.update();
        if(game_state == STATE_MENU) {
            menu();
        } else if(game_state == STATE_GAMEPLAY) {
            gameplay();
        }

        view.render(game_state);
    }
    return 0;
}

void Game::menu() {
}

void Game::gameplay() {
    /* update de l'orientation du player */
    assets.map.players.begin()->updateCam(view.mouse_pos_rel);
    assets.map.players.begin()->orientation = assets.map.players.begin()->cam.direction;

    /* update enemies positions */
    for (std::list<Enemy>::iterator it = assets.map.characters.begin(); it != assets.map.characters.end(); ++it) {
		if (it->near(*(assets.map.players.begin()))) {
			std::cout << "game.cpp - ATTACK !" << std::endl;
		} else {
			Movement reachPlayer = it->reach(0, &(assets.map));
			if (reachPlayer != MOVEMENT_NONE) {
				it->move(reachPlayer, SDL_GetTicks());
			}
		}
	}

    if(view.get_input() == INPUT_QUIT)
        game_state = STATE_QUIT;
    else if(view.get_input() == INPUT_MOVE_BACKWARD) {
        assets.map.players.begin()->move(MOVEMENT_BACKWARD, SDL_GetTicks());
        std::cout << "game.cpp - " << assets.map.players.begin()->position << std::endl;
    } else if(view.get_input() == INPUT_MOVE_FORWARD) {
        assets.map.players.begin()->move(MOVEMENT_FORWARD, SDL_GetTicks());
        std::cout << "game.cpp - " << assets.map.players.begin()->position << std::endl;
    } else if(view.get_input() == INPUT_MOVE_LEFT) {
        assets.map.players.begin()->move(MOVEMENT_LEFT, SDL_GetTicks());
        std::cout << "game.cpp - " << assets.map.players.begin()->position << std::endl;
    } else if(view.get_input() == INPUT_MOVE_RIGHT) {
        assets.map.players.begin()->move(MOVEMENT_RIGHT, SDL_GetTicks());
        std::cout << "game.cpp - " << assets.map.players.begin()->position << std::endl;
    }

	/* Detection de fin de niveau (case == 2)*/
	if(assets.map.datas[assets.map.players.begin()->position.y][assets.map.players.begin()->position.x] == 2)
		Gui::getInstance().showHUDEndLevelPopUp = true;
}

void Game::print(){
	std::cout << "*** GAME - BEGIN ***" << std::endl;
	std::cout << "Application path : " << application_path << std::endl;
	std::cout << "Game_state : " << game_state << std::endl;
	std::cout << std::endl;
	assets.print();
	std::cout << std::endl;
	std::cout << "*** GAME - END ***" << '\n';
}
