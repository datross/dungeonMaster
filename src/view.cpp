#include <iostream>
#include "view.h"
#include <sstream>
#include <algorithm>

#include "gui.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

View::View()
    : player_input(INPUT_NONE), mouse_pos(0, 0) {
    /* Initialisation de la SDL */
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        std::cout << "Error while initializing SDL: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
	/* Setup window */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);

    /* Version d'OpenGL */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    /* Double Buffer */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    /* taille de la fenêtre par défaut */
    window_width = 1024;
    window_height = 680;

    /* Création de la fenêtre */
    window = SDL_CreateWindow("Dungeon Master", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if(window == 0)
    {
        std::cout << "Error while creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    /* Création du contexte OpenGL */
    context_gl = SDL_GL_CreateContext(window);
    if(context_gl == 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    /* GLEW */
    glewExperimental = GL_TRUE;
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    /* Setup ImGui binding */
    ImGui_ImplSdlGL3_Init(window);

}

View::~View() {
    ImGui_ImplSdlGL3_Shutdown();
    SDL_GL_DeleteContext(context_gl);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void View::update() {
    updateEvent();
}

void View::updateEvent() {
    player_input = INPUT_NONE;
    mouse_pos_rel = glm::ivec2(0, 0);
    while(SDL_PollEvent(&event))
    {

		ImGui_ImplSdlGL3_ProcessEvent(&event);

        if(event.type == SDL_QUIT) {
            player_input = INPUT_QUIT;
        } else if(event.type == SDL_KEYDOWN) {
            if(event.key.keysym.sym == SDLK_z) {
                player_input = INPUT_MOVE_FORWARD;
            } else if(event.key.keysym.sym == SDLK_q) {
                player_input = INPUT_MOVE_LEFT;
            } else if(event.key.keysym.sym == SDLK_s) {
                player_input = INPUT_MOVE_BACKWARD;
            } else if(event.key.keysym.sym == SDLK_d) {
                player_input = INPUT_MOVE_RIGHT;
            }
        } else if(event.type == SDL_MOUSEMOTION) {
            mouse_pos_rel = glm::ivec2(event.motion.x, event.motion.y) - mouse_pos;
            mouse_pos = glm::ivec2(event.motion.x, event.motion.y);
        }
    }

	/* KEYBOARD MENUS EVENTS */
		//SPACE : OPEN ingame menu
		if (ImGui::IsKeyPressed(32)) {
			Gui::getInstance().showHUDIndicators = false;
			Gui::getInstance().showHUDInventory = true;
		}
		//ESCAPE : CLOSE in game menu
		if (ImGui::IsKeyPressed(27)) {
			Gui::getInstance().showHUDIndicators = true;
			Gui::getInstance().showHUDInventory = false;
			Gui::getInstance().showHUDMap = false;
			Gui::getInstance().showHUDOptions = false;
		}

		//E : ATTACK
		if (ImGui::IsKeyPressed(101)) {
			//Morceau de computeSteppedDirection
			glm::ivec2 stepped_dir;
		    if(assets_ptr->map.players.front().orientation.x > 0 && assets_ptr->map.players.front().orientation.z > 0) {
		        if(assets_ptr->map.players.front().orientation.x > assets_ptr->map.players.front().orientation.z)
		            stepped_dir = glm::ivec2(1, 0);
		        else
		            stepped_dir = glm::ivec2(0, 1);
		    } else if(assets_ptr->map.players.front().orientation.x < 0 && assets_ptr->map.players.front().orientation.z > 0) {
		        if(-(assets_ptr->map.players.front().orientation.x) > assets_ptr->map.players.front().orientation.z)
		            stepped_dir = glm::ivec2(-1, 0);
		        else
		            stepped_dir = glm::ivec2(0, 1);
		    } else if(assets_ptr->map.players.front().orientation.x < 0 && assets_ptr->map.players.front().orientation.z < 0) {
		        if(assets_ptr->map.players.front().orientation.x < assets_ptr->map.players.front().orientation.z)
		            stepped_dir = glm::ivec2(-1, 0);
		        else
		            stepped_dir = glm::ivec2(0, -1);
		    } else if(assets_ptr->map.players.front().orientation.x > 0 && assets_ptr->map.players.front().orientation.z < 0) {
		        if(assets_ptr->map.players.front().orientation.x > -(assets_ptr->map.players.front().orientation.z))
		            stepped_dir = glm::ivec2(1, 0);
		        else
		            stepped_dir = glm::ivec2(0, -1);
		    }

			// Chack if an enemy is in front, and attack it
			for(std::list<Enemy>::iterator it = assets_ptr->map.characters.begin();
				it != assets_ptr->map.characters.end();
				++it){
					if((*it).position == (assets_ptr->map.players.front().position + stepped_dir))
						assets_ptr->map.players.front().attack((*it));
			}
		}

		//A : USE Key
		if (ImGui::IsKeyPressed(97)) {
			for(std::vector<Item>::iterator it = assets_ptr->map.players.front().inventory.begin();
				it != assets_ptr->map.players.front().inventory.end();
				++it){
					if((*it).type == KEY) {
						glm::ivec2 door = assets_ptr->map.players.front().isNextDoor((*it).value);
						if(door != glm::ivec2(0,0)){
							assets_ptr->map.datas[door.y][door.x] = 0;
							assets_ptr->map.players.front().use(*it);
						}
					}
			}
		}
}

void View::reshape(unsigned w, unsigned h) {
    window_width  = w;
    window_height = h;
    float aspect_ratio = (float)w/h;

    /* update des matrices de projection des players */
    for(auto it = assets_ptr->map.players.begin(); it != assets_ptr->map.players.end(); ++it) {
        it->cam.init(70., aspect_ratio);
    }
}

void View::render(Game_state& game_state) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* USER INTERFACE */
    ImGui_ImplSdlGL3_NewFrame(window);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    if(game_state == STATE_MENU) {
		SDL_SetRelativeMouseMode(SDL_FALSE);
        mainMenu(game_state);
    } else if (game_state == STATE_GAMEPLAY) {
        renderGame(game_state);
        HUD(game_state);
    } else if (game_state == STATE_QUIT) {

    } else {
		bool isValidate = false;
		Gui::getInstance().errorWindow(isValidate);
		if(isValidate) game_state = STATE_QUIT;
    }
    ImGui::Render();

    SDL_GL_SwapWindow(window);
}

Player_input View::get_input() {
    return player_input;
}

/******************************************
*		USER INTERFACE FUNCTIONS		  *
*******************************************/

void View::initTextures(){
	/* Loading of main menu textures */
	Gui::getInstance().loadTexture(Gui::getInstance().mainMenuTex, assets_ptr->application_path + "res/gui_elements/mainmenu/background.png");
	Gui::getInstance().loadTexture(Gui::getInstance().mainMenuTex, assets_ptr->application_path + "res/gui_elements/mainmenu/new.png");
	Gui::getInstance().loadTexture(Gui::getInstance().mainMenuTex, assets_ptr->application_path + "res/gui_elements/mainmenu/load.png");
	Gui::getInstance().loadTexture(Gui::getInstance().mainMenuTex, assets_ptr->application_path + "res/gui_elements/mainmenu/explications.png");
	Gui::getInstance().loadTexture(Gui::getInstance().mainMenuTex, assets_ptr->application_path + "res/gui_elements/mainmenu/quit.png");
	Gui::getInstance().loadTexture(Gui::getInstance().mainMenuTex, assets_ptr->application_path + "res/gui_elements/mainmenu/arrow.png");
	Gui::getInstance().loadTexture(Gui::getInstance().mainMenuTex, assets_ptr->application_path + "res/gui_elements/mainmenu/controles.png");

	/* Loading of gui icons */
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/Icon_Life.png");
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/Button_Blue.png");
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/Button_Gold.png");
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/Button_Green.png");
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/Button_Red.png");
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/Icon_Attack.png");
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/Icon_Defense.png");
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/Mice.png");

	/* Loading of items icons */
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/Key.png");
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/Armor_Nut.png");
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/Armor_Thimble.png");
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/Weapon_Needle.png");
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/Weapon_Pin.png");
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/Weapon_Stone.png");


	// Load default font
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF((assets_ptr->application_path + "include/extra_fonts/olivier_demo.ttf").c_str(), 30.0f);
}

void View::mainMenu(Game_state& game_state){

//BACKGROUND
	/* Background parameters */
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoSavedSettings;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::SetNextWindowPos(ImVec2(-1,-1));
	ImGui::SetNextWindowSize(ImVec2(window_width+2, window_height+2));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
	ImGui::Begin("background", NULL, window_flags);
	ImGui::Image(Gui::getInstance().mainMenuTex[0].first, ImVec2(window_width+2, window_height+2), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));
	ImGui::End();
	ImGui::PopStyleVar();

	/* Windows parameters */
	window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoSavedSettings;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	// Color rules for button and color background */
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColor(0,0,0,0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0,0,0,0));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(0,0,0,0));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(0,0,0,0));

//MAIN MENU
	if(Gui::getInstance().showMainMenu){
		ImGui::SetNextWindowPos(ImVec2(50, window_height/3.0f));
		ImGui::Begin("Main menu", &(Gui::getInstance().showMainMenu), window_flags);
		/* New Game button */
		float scale_btn = 0.2f;
		if (ImGui::ImageButton(Gui::getInstance().mainMenuTex[1].first, ImVec2(Gui::getInstance().mainMenuTex[1].second.x * scale_btn, Gui::getInstance().mainMenuTex[1].second.y * scale_btn) , ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))) {
			Gui::getInstance().showMainMenu = false;
			Gui::getInstance().showLevelSelector = true;
		}
		ImGui::Spacing();
		/* Saves select button */
		if (ImGui::ImageButton(Gui::getInstance().mainMenuTex[2].first, ImVec2(Gui::getInstance().mainMenuTex[2].second.x * scale_btn, Gui::getInstance().mainMenuTex[2].second.y * scale_btn), ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))){
			Gui::getInstance().showMainMenu = false;
			Gui::getInstance().showSavesSelector = true;
		}
		ImGui::Spacing();
		/* Options selector buttons */
		if (ImGui::ImageButton(Gui::getInstance().mainMenuTex[3].first, ImVec2(Gui::getInstance().mainMenuTex[3].second.x * scale_btn, Gui::getInstance().mainMenuTex[3].second.y * scale_btn), ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))){
			Gui::getInstance().showMainMenu = false;
			Gui::getInstance().showOptionsSelector = true;
		}
		ImGui::Spacing();
		/* Quit button */
		if (ImGui::ImageButton(Gui::getInstance().mainMenuTex[4].first, ImVec2(Gui::getInstance().mainMenuTex[4].second.x * scale_btn, Gui::getInstance().mainMenuTex[4].second.y * scale_btn), ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))){
			ImGui::OpenPopup("Quit");
		}
		//QUIT CONFIRMAION POPUP
		ImGui::SetNextWindowPosCenter();

		if (ImGui::BeginPopup("Quit")){

			ImGui::Spacing();
			ImGui::Text("Voulez vous vraiment quitter le jeu ?");
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::Button("Oui")){
				game_state = STATE_QUIT;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Non"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		/* End of main menu */
		ImGui::End();
	}


	float scaleReturnBtn = 0.2f;

//LEVEL MENU
	if(Gui::getInstance().showLevelSelector){
		ImGui::SetNextWindowPos(ImVec2(50, window_height/3.0f));
		ImGui::Begin("Level menu", &(Gui::getInstance().showLevelSelector), window_flags);
		/* Level "Tinymap" */
		if (ImGui::Button("Tinymap")){
			std::string lvl_name = "tinymap";
			assets_ptr->load(lvl_name, true);
			game_state = STATE_GAMEPLAY;
			Gui::getInstance().showLevelSelector = false;
			Gui::getInstance().showMainMenu = true;
			// Load map texture for HUD
			Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/maps/"+ (lvl_name + ".png"));
		}
		/* Level "01" */
		if (ImGui::Button("Un nouveau départ")){
			std::string lvl_name = "01";
			assets_ptr->load(lvl_name, true);
			game_state = STATE_GAMEPLAY;
			Gui::getInstance().showLevelSelector = false;
			Gui::getInstance().showMainMenu = true;
			// Load map texture for HUD
			Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/maps/"+ (lvl_name + ".png"));
		}
		/* Level "Sample" */
		if (ImGui::Button("Encore un nouveau départ")){
			std::string lvl_name = "simplemap";
			assets_ptr->load(lvl_name, true);
			game_state = STATE_GAMEPLAY;
			Gui::getInstance().showLevelSelector = false;
			Gui::getInstance().showMainMenu = true;
			// Load map texture for HUD
			Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/maps/"+ (lvl_name + ".png"));
		}
		//Return button
		if (ImGui::ImageButton(Gui::getInstance().mainMenuTex[5].first, ImVec2(Gui::getInstance().mainMenuTex[5].second.x * scaleReturnBtn, Gui::getInstance().mainMenuTex[5].second.y * scaleReturnBtn), ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))){
			Gui::getInstance().showLevelSelector = false;
			Gui::getInstance().showMainMenu = true;
		}
		/* End of level menu */
		ImGui::End();
	}

//LOAD SAVES MENU
	if(Gui::getInstance().showSavesSelector){
		ImGui::SetNextWindowPos(ImVec2(50, window_height/3.0f));
		ImGui::Begin("Load saves menu", &(Gui::getInstance().showSavesSelector), window_flags);
		//Save 01
		if (ImGui::Button("Save 01")){
			assets_ptr->load("01", false);
			game_state = STATE_GAMEPLAY;
			Gui::getInstance().showSavesSelector = false;
			Gui::getInstance().showMainMenu = true;
		}
		//Return button
		if (ImGui::ImageButton(Gui::getInstance().mainMenuTex[5].first, ImVec2(Gui::getInstance().mainMenuTex[5].second.x * scaleReturnBtn, Gui::getInstance().mainMenuTex[5].second.y * scaleReturnBtn), ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))){
			Gui::getInstance().showSavesSelector = false;
			Gui::getInstance().showMainMenu = true;
		}
		/* End of save menu */
		ImGui::End();
	}

//OPTION MENU
	if(Gui::getInstance().showOptionsSelector){
		ImGui::SetNextWindowPos(ImVec2(window_width*0.1f, window_height*0.1f));
		ImGui::Begin("Options menu", &(Gui::getInstance().showOptionsSelector), window_flags);

		ImGui::Image(Gui::getInstance().mainMenuTex[6].first, ImVec2(window_width*0.8f, window_height*0.8f) , ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,0,0,0));

		//Return button
		if (ImGui::ImageButton(Gui::getInstance().mainMenuTex[5].first, ImVec2(Gui::getInstance().mainMenuTex[5].second.x * scaleReturnBtn, Gui::getInstance().mainMenuTex[5].second.y * scaleReturnBtn), ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))){
			Gui::getInstance().showOptionsSelector = false;
			Gui::getInstance().showMainMenu = true;
		}

		ImGui::End();
	}


// FORGET COLOR RULES
	ImGui::PopStyleColor(4);

}

void View::HUD(Game_state& game_state) {

// Cursor masked when no menus are opened
	if(Gui::getInstance().showHUDIndicators){
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	/* Windows parameters */
	ImGuiWindowFlags window_flags = 0;

	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoSavedSettings;

	/* Convertissor from datas */
	std::stringstream life, score, power, defense;
	life << (*(assets_ptr->map.players.begin())).life;
	score << (*(assets_ptr->map.players.begin())).score;
	power << (*(assets_ptr->map.players.begin())).power;
	defense << (*(assets_ptr->map.players.begin())).defense;

	// Color rules
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor(32,73,79,255));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(29,62,69,255));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(9,42,49,255));

//INDICATORS
	if(Gui::getInstance().showHUDIndicators){
		ImGui::SetNextWindowPos(ImVec2(0,0));
		ImGui::Begin("HUD - Life & score", NULL, window_flags);
		//Life indicator
		ImGui::Image(Gui::getInstance().HUDTex[0].first, ImVec2(30,30), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,0,0,0));
		ImGui::SameLine();
		ImGui::Text(life.str().c_str());
		//Score indicator
		ImGui::Image(Gui::getInstance().HUDTex[1].first, ImVec2(30,30), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,0,0,0));
		ImGui::SameLine();
		ImGui::Text(score.str().c_str());
		ImGui::End();
	}

	if(Gui::getInstance().showHUDEndLevelPopUp){
		ImGui::OpenPopup("End");
		Gui::getInstance().showHUDIndicators = false;
	}
	//END LEVEL POPUP
	ImGui::SetNextWindowPosCenter();
	ImGui::SetNextWindowFocus();
	if (ImGui::BeginPopup("End")){
		ImGui::Spacing();
		ImGui::Text("Félicitations, Vous avez terminé le niveau.");
		ImGui::Text("Voulez vous retourner au menu principal ou quitter ?");
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		if (ImGui::Button("Retour au menu principal")){
			Gui::getInstance().showHUDOptions = false;
			Gui::getInstance().showHUDMap = false;
			Gui::getInstance().showHUDInventory = false;
			assets_ptr->reset();
			game_state = STATE_MENU;
		}
		ImGui::SameLine();
		if (ImGui::Button("Quitter")){
			game_state = STATE_QUIT;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	// Background color
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColor(9,42,49,255));

//INVENTORY
	if(Gui::getInstance().showHUDInventory){
		ImGui::SetNextWindowPos(ImVec2(window_width*0.1,window_height*0.1));
		ImGui::SetNextWindowSize(ImVec2(window_width*0.8, window_height*0.8));
		ImGui::Begin("HUD_Inventory", NULL, window_flags);
		//TABS buttons
		ImGui::Text("- Inventaire -");
		ImGui::SameLine();
		if (ImGui::Button("- Carte -")){
			Gui::getInstance().showHUDInventory = false;
			Gui::getInstance().showHUDOptions = false;
			Gui::getInstance().showHUDMap = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("- Options -")){
			Gui::getInstance().showHUDInventory = false;
			Gui::getInstance().showHUDMap = false;
			Gui::getInstance().showHUDOptions = true;
		}
		ImGui::Separator();

		//Index for items icons textures
		std::vector<std::string> indexTex = {
			"Key",
			"Armor_Nut",
			"Armor_Thimble",
			"Weapon_Needle",
			"Weapon_Pin",
			"Weapon_Stone"
		};

		// Visual for map and Statistiques
		ImGui::Columns(2, "visual");
		float scaleVisual = 0.25f;
		ImGui::Image(Gui::getInstance().HUDTex[7].first, ImVec2(Gui::getInstance().HUDTex[7].second.x*scaleVisual,  Gui::getInstance().HUDTex[7].second.y*scaleVisual), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,0,0,0));
		ImGui::NextColumn();
		ImGui::Text("- Statistiques -");
		ImGui::Image(Gui::getInstance().HUDTex[0].first, ImVec2(30,30), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,0,0,0));
		ImGui::SameLine();
		ImGui::Text(std::string("Points de vie : " + life.str()).c_str());
		ImGui::Image(Gui::getInstance().HUDTex[2].first, ImVec2(30,30), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,0,0,0));
		ImGui::SameLine();
		ImGui::Text(std::string("Score : " + score.str()).c_str());
		ImGui::Image(Gui::getInstance().HUDTex[5].first, ImVec2(30,30), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,0,0,0));
		ImGui::SameLine();
		ImGui::Text(std::string("Attaque : " + power.str()).c_str());
		ImGui::Image(Gui::getInstance().HUDTex[6].first, ImVec2(30,30), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,0,0,0));
		ImGui::SameLine();
		ImGui::Text(std::string("Defense : " + defense.str()).c_str());
		ImGui::Spacing();
		ImGui::NextColumn();
		ImGui::Columns(1);

		// Display inventory, with highlights on equiped items
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0,0,0,0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(29,62,69,255));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(9,42,49,255));
		ImGui::Separator();
		if((*(assets_ptr->map.players.begin())).inventory.size()>0){

			ImGui::Columns((*(assets_ptr->map.players.begin())).inventory.size(), "bag");
			for(std::vector<Item>::iterator it = (*(assets_ptr->map.players.begin())).inventory.begin();
				it != (*(assets_ptr->map.players.begin())).inventory.end();
				it++){
					int index = 0;
					for (size_t i = 0; i < indexTex.size(); i++) {
						if(indexTex[i].compare((*it).id) == 0) index = i;
					}
					ImGui::Text((*it).id.c_str());

					ImColor btnBg = ImColor(0,0,0,0);
					if((*(assets_ptr->map.players.begin())).equiped.end() != std::find((*(assets_ptr->map.players.begin())).equiped.begin(), (*(assets_ptr->map.players.begin())).equiped.end(), (*it))){
						ImGui::PushID(0);
						ImGui::PushStyleColor(ImGuiCol_Button, ImColor(255,200,0,255));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(155,100,0,255));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(105,50,0,255));
						if (ImGui::ImageButton(Gui::getInstance().HUDTex[8+index].first, ImVec2(100,100), ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))){
							(*(assets_ptr->map.players.begin())).unequip((*it));
						}
						ImGui::PopStyleColor(3);
						ImGui::PopID();
					}else {
						if (ImGui::ImageButton(Gui::getInstance().HUDTex[8+index].first, ImVec2(100,100), ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))){
							(*(assets_ptr->map.players.begin())).equip((*it));
						}
					}
					ImGui::ProgressBar((*it).durability, ImVec2(0.0f,0.0f));
					if(it != (*(assets_ptr->map.players.begin())).inventory.end()-1) ImGui::NextColumn();
			}
			ImGui::Columns(1);
			ImGui::Separator();
		}
		ImGui::PopStyleColor(3);
		/* end of inventory */
		ImGui::End();
	}

//MAP
	if(Gui::getInstance().showHUDMap){
		ImGui::SetNextWindowPos(ImVec2(window_width*0.1,window_height*0.1));
		ImGui::SetNextWindowSize(ImVec2(window_width*0.8, window_height*0.8));
		ImGui::Begin("HUD_Map", NULL, window_flags);
		//TABS buttons
		if (ImGui::Button("- Inventaire -")){
			Gui::getInstance().showHUDMap = false;
			Gui::getInstance().showHUDOptions = false;
			Gui::getInstance().showHUDInventory = true;
		}
		ImGui::SameLine();
		ImGui::Text("- Carte -");
		ImGui::SameLine();
		if (ImGui::Button("- Options -")){
			Gui::getInstance().showHUDMap = false;
			Gui::getInstance().showHUDInventory = false;
			Gui::getInstance().showHUDOptions = true;
		}
		ImGui::Separator();

		// Texture of map
		ImGui::Image((*(Gui::getInstance().HUDTex.end()-1)).first, ImVec2(ImGui::GetWindowHeight()*0.8f, ImGui::GetWindowHeight()*0.8f) , ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,0,0,0));

		/* End of map */
		ImGui::End();
	}

//OPTIONS
	if(Gui::getInstance().showHUDOptions){
		ImGui::SetNextWindowPos(ImVec2(window_width*0.1,window_height*0.1));
		ImGui::SetNextWindowSize(ImVec2(window_width*0.8, window_height*0.8));
		ImGui::Begin("HUD_Options", NULL, window_flags);
		//TABS buttons
		if (ImGui::Button("- Inventaire -")){
			Gui::getInstance().showHUDOptions = false;
			Gui::getInstance().showHUDMap = false;
			Gui::getInstance().showHUDInventory = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("- Carte -")){
			Gui::getInstance().showHUDOptions = false;
			Gui::getInstance().showHUDInventory = false;
			Gui::getInstance().showHUDMap = true;
		}
		ImGui::SameLine();
		ImGui::Text("- Options -");
		ImGui::Separator();

		// Return to main menu
		if (ImGui::Button("Retour au menu principal")){
			Gui::getInstance().showHUDOptions = false;
			Gui::getInstance().showHUDMap = false;
			Gui::getInstance().showHUDInventory = false;
			assets_ptr->reset();
			game_state = STATE_MENU;
		}

		//Quit Game
		if (ImGui::Button("Quitter")){
			ImGui::OpenPopup("Quit");
		}

		//QUIT CONFIRMAION POPUP
		ImGui::SetNextWindowPosCenter();
		if (ImGui::BeginPopup("Quit")){

			ImGui::Spacing();
			ImGui::Text("Voulez vous vraiment quitter le jeu ?");
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::Button("Oui")){
				game_state = STATE_QUIT;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Non"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		/* End of options */
		ImGui::End();
	}

	// FORGET COLOR RULES
	ImGui::PopStyleColor(4);

}

void View::renderGame(Game_state& game_state) {
    reshape(window_width, window_height);

    Mesh& wall = assets_ptr->wall;
    Mesh& ground = assets_ptr->ground;
    Mesh& ceiling = assets_ptr->ceiling;
    Mesh& water = assets_ptr->water;

    glimac::Program& shader = assets_ptr->generalShader->program;

    wall.setUniformsId(shader);
    ground.setUniformsId(shader);
    water.setUniformsId(shader);
    ceiling.setUniformsId(shader);

    /* Pour toutes les vues des joueurs */
    for(auto p = assets_ptr->map.players.begin(); p != assets_ptr->map.players.end(); ++p) {


        glm::mat4 v = p->cam.getVMatrix();
        glm::mat4 mv;

        glm::vec3 lightPos = p->cam.position;

        shader.use();

        /* on décale tout le décor de (0.5,0.5) pour être au milieu des cases */
        glm::mat4 v_origin = v;
        v = glm::translate(v, glm::vec3(-0.5,0,-0.5));


        /* Ground and ceiling rendering */
        for(unsigned y = 0; y < assets_ptr->map.datas.size(); ++y) {
            for(unsigned x = 0; x < assets_ptr->map.datas[0].size(); ++x) {

                mv = v;
                mv = glm::translate(mv, glm::vec3(1.0 * x,0,(1.0 * y)+1));
                if(assets_ptr->map.datas[y][x] == -3) { /* water */
                        water.setMVMatrix(mv);
                        water.setMVPMatrix(p->cam.getPMatrix() * mv);
                        water.setNormalMatrix(glm::transpose(glm::inverse(mv)));
                        water.setShininess(1.);
                        water.setLightPos_vs(glm::vec3(v_origin * glm::vec4(lightPos,1.)));
                        water.setLightIntensity(glm::vec3(1,1,1));
                        water.setKs(glm::vec3(1,1,1));
                        water.setKd(glm::vec3(1,1,1));

                        water.render();
                }

                if(assets_ptr->map.isCaseEmpty(x, y)) {
                    mv = v;
                    mv = glm::translate(mv, glm::vec3(1.0 * x,0,(1.0 * y)+1));

                    if(assets_ptr->map.datas[y][x] == 0 || assets_ptr->map.datas[y][x] == 1 || assets_ptr->map.datas[y][x] == 2) {  /* ground */
                        ground.setMVMatrix(mv);
                        ground.setMVPMatrix(p->cam.getPMatrix() * mv);
                        ground.setNormalMatrix(glm::transpose(glm::inverse(mv)));
                        ground.setShininess(1.);
                        ground.setLightPos_vs(glm::vec3(v_origin * glm::vec4(lightPos,1.)));
                        ground.setLightIntensity(glm::vec3(1,1,1));
                        ground.setKs(glm::vec3(1,1,1));
                        ground.setKd(glm::vec3(1,1,1));

                        ground.render();
                    }

                    /* ceiling */

                    mv = glm::translate(mv, glm::vec3(0,1,0));

                    ceiling.setMVMatrix(mv);
                    ceiling.setMVPMatrix(p->cam.getPMatrix() * mv);
                    ceiling.setNormalMatrix(glm::transpose(glm::inverse(mv)));
                    ceiling.setShininess(1.);
                    ceiling.setLightPos_vs(glm::vec3(v_origin * glm::vec4(lightPos,1.)));
                    ceiling.setLightIntensity(glm::vec3(1,1,1));
                    ceiling.setKs(glm::vec3(1,1,1));
                    ceiling.setKd(glm::vec3(1,1,1));

                    ceiling.render();
                }
            }
        }

        /* Walls rendering */
        for(unsigned y = 0; y < assets_ptr->map.datas.size(); ++y) {
            for(unsigned x = 0; x < assets_ptr->map.datas[0].size(); ++x) {
                if(!assets_ptr->map.isCaseEmpty(x, y)) {
                    mv = v;
                    mv = glm::translate(mv, glm::vec3(1.0 * x,0,(1.0 * y)+1));

                    wall.setMVMatrix(mv);
                    wall.setMVPMatrix(p->cam.getPMatrix() * mv);
                    wall.setNormalMatrix(glm::transpose(glm::inverse(mv)));
                    wall.setShininess(1.);
                    wall.setLightPos_vs(glm::vec3(v_origin * glm::vec4(lightPos,1.)));
                    wall.setLightIntensity(glm::vec3(1,1,1));
                    wall.setKs(glm::vec3(1,1,1));
                    wall.setKd(glm::vec3(1,1,1));

                    wall.render();
                }
            }
        }

        /* Enemies rendering */
		 for (std::list<Enemy>::iterator it = assets_ptr->map.characters.begin(); it != assets_ptr->map.characters.end(); ++it) {
			mv = v;
			mv = glm::translate(mv, glm::vec3(1.0*it->position.x, 0, 1.0*it->position.y));
			(*it).mesh_ptr->setMVMatrix(mv);
			(*it).mesh_ptr->setMVPMatrix(p->cam.getPMatrix() * mv);
			(*it).mesh_ptr->setNormalMatrix(glm::transpose(glm::inverse(mv)));
			(*it).mesh_ptr->setShininess(1.);
			(*it).mesh_ptr->setLightPos_vs(glm::vec3(v_origin * glm::vec4(lightPos,1.)));
			(*it).mesh_ptr->setLightIntensity(glm::vec3(1,1,1));
			(*it).mesh_ptr->setKd(glm::vec3(1,1,1));

			(*it).mesh_ptr->render();
     	}
        /* Items rendering */
		for (std::list<Item>::iterator it = assets_ptr->map.items.begin(); it != assets_ptr->map.items.end(); ++it) {
			mv = v;
			mv = glm::translate(mv, glm::vec3(1.0*it->position.x, 0, 1.0*it->position.y));
			(*it).mesh_ptr->setMVMatrix(mv);
			(*it).mesh_ptr->setMVPMatrix(p->cam.getPMatrix() * mv);
			(*it).mesh_ptr->setNormalMatrix(glm::transpose(glm::inverse(mv)));
			(*it).mesh_ptr->setShininess(1.);
			(*it).mesh_ptr->setLightPos_vs(glm::vec3(v_origin * glm::vec4(lightPos,1.)));
			(*it).mesh_ptr->setLightIntensity(glm::vec3(1,1,1));
			(*it).mesh_ptr->setKd(glm::vec3(1,1,1));

			(*it).mesh_ptr->render();
     	}
    }
}

void View::setAssets(Assets& _assets_ptr) {
    assets_ptr = &_assets_ptr;
}
