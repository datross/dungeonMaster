#include "game.h"
#include <iostream>

Game::Game(char* _application_path)
    : assets(_application_path), application_path(_application_path), game_state(STATE_MENU) {
		view.setAssets(assets);
}

Game::~Game() {

}

int Game::run() {
    while(game_state != STATE_QUIT) {
        // TODO temporaire
        std::cout << "\033[2J";
        
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
    // TODO ça paraît très temporaire et crade tout ça...
    if(view.get_input() == INPUT_QUIT)
        game_state = STATE_QUIT;
    else if(view.get_input() == INPUT_MOVE_BACKWARD) {
        assets.map.players.begin()->position = assets.map.players.begin()->position + glm::ivec2(0,1);
        std::cout << assets.map.players.begin()->position << std::endl;
    } else if(view.get_input() == INPUT_MOVE_FORWARD) {
        assets.map.players.begin()->position = assets.map.players.begin()->position + glm::ivec2(0,-1);
        std::cout << assets.map.players.begin()->position << std::endl;
    } else if(view.get_input() == INPUT_MOVE_LEFT) {
        assets.map.players.begin()->position = assets.map.players.begin()->position + glm::ivec2(-1,0);
        std::cout << assets.map.players.begin()->position << std::endl;
    } else if(view.get_input() == INPUT_MOVE_RIGHT) {
        assets.map.players.begin()->position = assets.map.players.begin()->position + glm::ivec2(1,0);
        std::cout << assets.map.players.begin()->position << std::endl;
    }
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
