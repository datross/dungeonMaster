#include <iostream>
#include "view.h"
#include <sstream>

#include "gui.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

View::View()
    : player_input(INPUT_NONE) {
    // Initialisation de la SDL
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        std::cout << "Error while initializing SDL: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
	// Setup window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);

    // Version d'OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Double Buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    glEnable(GL_DEPTH_TEST);

    // taille de la fenêtre par défaut
    window_width = 1024;
    window_height = 680;

    // Création de la fenêtre
    window = SDL_CreateWindow("Dungeon Master", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if(window == 0)
    {
        std::cout << "Error while creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Création du contexte OpenGL
    context_gl = SDL_GL_CreateContext(window);
    if(context_gl == 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // GLEW
    glewExperimental = GL_TRUE;
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Setup ImGui binding
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
        }
    }

	/* KEYBOARD MENUS EVENTS */
		//SPACE
		if (ImGui::IsKeyPressed(32)) {
			Gui::getInstance().showHUDIndicators = false;
			Gui::getInstance().showHUDInventory = true;
		}
		//ESCAPE
		if (ImGui::IsKeyPressed(27)) {
			Gui::getInstance().showHUDIndicators = true;
			Gui::getInstance().showHUDInventory = false;
			Gui::getInstance().showHUDMap = false;
			Gui::getInstance().showHUDOptions = false;
		}
}

void View::reshape(unsigned w, unsigned h) {
    window_width  = w;
    window_height = h;
    float aspect_ratio = (float)w/h;

    // update des matrices de projection des players
    // TODO gérer proprement la fov des camera
    for(auto it = assets_ptr->map.players.begin(); it != assets_ptr->map.players.end(); ++it) {
        it->cam.init(70., aspect_ratio);
    }
}

void View::render(Game_state& game_state) {
    glClear(GL_COLOR_BUFFER_BIT);

//     Mesh mesh;
//     mesh.loadFromFile("res/tete.obj");
//     mesh.loadShader("res/shaders/3D.vs.glsl",
//                     "res/shaders/directionallight.fs.glsl");
//     Camera camera;
//     camera.init(70, 1.);
//     camera.position = glm::vec3(map_ptr->players.begin()->position.x, 0.5, map_ptr->players.begin()->position.y);
//     camera.direction = glm::vec3(0.5,-0.5,-1);
//
//     glm::mat4 v = camera.getVMatrix();
//     glm::mat4 mv = v;
//     //mv = glm::scale(v, glm::vec3(0.1,0.1,0.1));
//     mv = glm::translate(mv, glm::vec3(0,0,0));
// //     glm::mat4 mv = glm::translate(glm::mat4(1.), glm::vec3(0,0,1));
//
//     mesh.activateShader();
//
//     mesh.setMVMatrix(mv);
//     mesh.setMVPMatrix(camera.getPMatrix() * mv);
//     mesh.setNormalMatrix(glm::transpose(glm::inverse(mv)));
//     mesh.setNormalMatrix(glm::mat4(1.));
//     mesh.setShininess(1.);
//     mesh.setLightDir_vs(glm::vec3(1,1,1));
//     mesh.setLightIntensity(glm::vec3(1,1,1));
//     mesh.setKs(glm::vec3(1,1,1));
//     mesh.setKd(glm::vec3(1,1,1));
//     mesh.render();

	// USER INTERFACE
	ImGui_ImplSdlGL3_NewFrame(window);
	if(game_state == STATE_MENU) {
		mainMenu(game_state);
	} else if (game_state == STATE_GAMEPLAY) {
		HUD(game_state);
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
	/* Loading of menu and hud textures */
	Gui::getInstance().loadTexture(Gui::getInstance().mainMenuTex, assets_ptr->application_path + "res/gui_elements/mainmenu/background.png");
	Gui::getInstance().loadTexture(Gui::getInstance().mainMenuTex, assets_ptr->application_path + "res/gui_elements/mainmenu/new.png");
	Gui::getInstance().loadTexture(Gui::getInstance().mainMenuTex, assets_ptr->application_path + "res/gui_elements/mainmenu/load.png");
	Gui::getInstance().loadTexture(Gui::getInstance().mainMenuTex, assets_ptr->application_path + "res/gui_elements/mainmenu/explications.png");
	Gui::getInstance().loadTexture(Gui::getInstance().mainMenuTex, assets_ptr->application_path + "res/gui_elements/mainmenu/quit.png");
	Gui::getInstance().loadTexture(Gui::getInstance().mainMenuTex, assets_ptr->application_path + "res/gui_elements/mainmenu/arrow.png");

	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/life.png");
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/score.png");
	Gui::getInstance().loadTexture(Gui::getInstance().HUDTex, assets_ptr->application_path + "res/gui_elements/hud/score.png");


	// Load Fonts
    // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF((assets_ptr->application_path + "include/extra_fonts/olivier_demo.ttf").c_str(), 50.0f);
}

void View::mainMenu(Game_state& game_state){

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoSavedSettings;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	// background
		ImGui::SetNextWindowPos(ImVec2(-1,-1));
		ImGui::SetNextWindowSize(ImVec2(window_width+2, window_height+2));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
		ImGui::Begin("background", NULL, window_flags);
		ImGui::Image(Gui::getInstance().mainMenuTex[0].first, ImVec2(window_width+2, window_height+2), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));
		ImGui::End();
		ImGui::PopStyleVar();

	window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoSavedSettings;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	// STYLES rules
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColor(0,0,0,0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0,0,0,0));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(0,0,0,0));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(0,0,0,0));

	//MAIN MENU
	if(Gui::getInstance().showMainMenu){

		ImGui::SetNextWindowPos(ImVec2(50, window_height/3.0f));
		ImGui::Begin("Main menu", &(Gui::getInstance().showMainMenu), window_flags);

		//Buttons
		float scale_btn = 0.2f;
		if (ImGui::ImageButton(Gui::getInstance().mainMenuTex[1].first, ImVec2(Gui::getInstance().mainMenuTex[1].second.x * scale_btn, Gui::getInstance().mainMenuTex[1].second.y * scale_btn) , ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))) {
			Gui::getInstance().showMainMenu = false;
			Gui::getInstance().showLevelSelector = true;
		}
		ImGui::Spacing();

		if (ImGui::ImageButton(Gui::getInstance().mainMenuTex[2].first, ImVec2(Gui::getInstance().mainMenuTex[2].second.x * scale_btn, Gui::getInstance().mainMenuTex[2].second.y * scale_btn), ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))){
			Gui::getInstance().showMainMenu = false;
			Gui::getInstance().showSavesSelector = true;
		}
		ImGui::Spacing();

		if (ImGui::ImageButton(Gui::getInstance().mainMenuTex[3].first, ImVec2(Gui::getInstance().mainMenuTex[3].second.x * scale_btn, Gui::getInstance().mainMenuTex[3].second.y * scale_btn), ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))){
			Gui::getInstance().showMainMenu = false;
			Gui::getInstance().showOptionsSelector = true;
		}
		ImGui::Spacing();

		if (ImGui::ImageButton(Gui::getInstance().mainMenuTex[4].first, ImVec2(Gui::getInstance().mainMenuTex[4].second.x * scale_btn, Gui::getInstance().mainMenuTex[4].second.y * scale_btn), ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))){
			ImGui::OpenPopup("Quit");
			//QUIT CONFIRMAION POPUP
		}

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

		ImGui::End();
	}


	float scaleReturnBtn = 0.2f;

	//LEVEL MENU
	if(Gui::getInstance().showLevelSelector){
		ImGui::SetNextWindowPos(ImVec2(50, window_height/3.0f));
		ImGui::Begin("Level menu", &(Gui::getInstance().showLevelSelector), window_flags);
		//Buttons
		if (ImGui::Button("Tinymap")){
			assets_ptr->load("tinymap", true);
			game_state = STATE_GAMEPLAY;
			Gui::getInstance().showLevelSelector = false;
			Gui::getInstance().showMainMenu = true;
		}
		if (ImGui::ImageButton(Gui::getInstance().mainMenuTex[5].first, ImVec2(Gui::getInstance().mainMenuTex[5].second.x * scaleReturnBtn, Gui::getInstance().mainMenuTex[5].second.y * scaleReturnBtn), ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))){
			Gui::getInstance().showLevelSelector = false;
			Gui::getInstance().showMainMenu = true;
		}

		ImGui::End();
	}


	//LOAD SAVES MENU
	if(Gui::getInstance().showSavesSelector){
		ImGui::Begin("Load saves menu", &(Gui::getInstance().showSavesSelector), window_flags);
		//Buttons
		if (ImGui::Button("Tinymap")){
			assets_ptr->load("tinymap", false);
			game_state = STATE_GAMEPLAY;
			Gui::getInstance().showSavesSelector = false;
			Gui::getInstance().showMainMenu = true;
		}
		if (ImGui::ImageButton(Gui::getInstance().mainMenuTex[5].first, ImVec2(Gui::getInstance().mainMenuTex[5].second.x * scaleReturnBtn, Gui::getInstance().mainMenuTex[5].second.y * scaleReturnBtn), ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))){
			Gui::getInstance().showSavesSelector = false;
			Gui::getInstance().showMainMenu = true;
		}

		ImGui::End();
	}

	//OPTION MENU
	if(Gui::getInstance().showOptionsSelector){
		ImGui::Begin("Level menu", &(Gui::getInstance().showOptionsSelector), window_flags);
		//Buttons

		if (ImGui::ImageButton(Gui::getInstance().mainMenuTex[5].first, ImVec2(Gui::getInstance().mainMenuTex[5].second.x * scaleReturnBtn, Gui::getInstance().mainMenuTex[5].second.y * scaleReturnBtn), ImVec2(0,0), ImVec2(1,1),-1, ImColor(0,0,0,0))){
			Gui::getInstance().showOptionsSelector = false;
			Gui::getInstance().showMainMenu = true;
		}

		ImGui::End();
	}


	// FORGET STYLE RULES
	ImGui::PopStyleColor(4);

}

void View::HUD(Game_state& game_state) {

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoSavedSettings;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	if (!Gui::getInstance().showHUDIndicators){
		// background
			ImGui::SetNextWindowPos(ImVec2(-1,-1));
			ImGui::SetNextWindowSize(ImVec2(window_width+2, window_height+2));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
			ImGui::Begin("background", NULL, window_flags);
			ImGui::Image(Gui::getInstance().mainMenuTex[0].first, ImVec2(window_width+2, window_height+2), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));
			ImGui::End();
			ImGui::PopStyleVar();
	}

	window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoSavedSettings;

	std::stringstream life, score;
	life << (*(assets_ptr->map.players.begin())).life;
	score << (*(assets_ptr->map.players.begin())).score;

	// STYLES rules
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColor(0,0,0,0));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(255,255,255,255));

	//Indicators
	if(Gui::getInstance().showHUDIndicators){
		ImGui::SetNextWindowPos(ImVec2(0,0));
		ImGui::Begin("HUD - Life & score", NULL, window_flags);
		ImGui::Image(Gui::getInstance().HUDTex[0].first, ImVec2(50,50), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,0,0,0));
		ImGui::SameLine();
		ImGui::Text(life.str().c_str());
		ImGui::Image(Gui::getInstance().HUDTex[1].first, ImVec2(50,50), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,0,0,0));
		ImGui::SameLine();
		ImGui::Text(score.str().c_str());
		ImGui::End();
/*
		ImGui::SetNextWindowPos(ImVec2(0,(window_height*4)/5.0f));
		ImGui::SetNextWindowSize(ImVec2(window_width/2.0f, window_height/5.0f));
		ImGui::Begin("HUD - Commands", NULL, window_flags);
		ImGui::Image(Gui::getInstance().HUDTex[0].first, Gui::getInstance().HUDTex[0].second, ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,0,0,0));
		ImGui::End();
*/
	}
	//Inventory
	if(Gui::getInstance().showHUDInventory){

		ImGui::SetNextWindowPos(ImVec2(window_width*0.1,window_height*0.1));
		ImGui::SetNextWindowSize(ImVec2(window_width*0.8, window_height*0.8));
		ImGui::SetNextWindowFocus();
		ImGui::Begin("HUD_Inventory", NULL, window_flags);
		if (ImGui::Button("Inventaire")){
		}
		ImGui::SameLine();
		if (ImGui::Button("Carte")){
			Gui::getInstance().showHUDInventory = false;
			Gui::getInstance().showHUDOptions = false;
			Gui::getInstance().showHUDMap = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Options")){
			Gui::getInstance().showHUDInventory = false;
			Gui::getInstance().showHUDMap = false;
			Gui::getInstance().showHUDOptions = true;
		}
		ImGui::Separator();

		ImGui::Text(std::string("Points de vie : " + life.str()).c_str());
		ImGui::Text(std::string("Score : " + score.str()).c_str());
		ImGui::Text("Sac à dos :");

		ImGui::End();
	}
	//Map
	if(Gui::getInstance().showHUDMap){

		ImGui::SetNextWindowPos(ImVec2(window_width*0.1,window_height*0.1));
		ImGui::SetNextWindowSize(ImVec2(window_width*0.8, window_height*0.8));
		ImGui::SetNextWindowFocus();
		ImGui::Begin("HUD_Map", NULL, window_flags);
		if (ImGui::Button("Inventaire")){
			Gui::getInstance().showHUDMap = false;
			Gui::getInstance().showHUDOptions = false;
			Gui::getInstance().showHUDInventory = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Carte")){
		}
		ImGui::SameLine();
		if (ImGui::Button("Options")){
			Gui::getInstance().showHUDMap = false;
			Gui::getInstance().showHUDInventory = false;
			Gui::getInstance().showHUDOptions = true;
		}
		ImGui::Separator();

		ImGui::End();
	}
	//Options
	if(Gui::getInstance().showHUDOptions){

		ImGui::SetNextWindowPos(ImVec2(window_width*0.1,window_height*0.1));
		ImGui::SetNextWindowSize(ImVec2(window_width*0.8, window_height*0.8));
		ImGui::SetNextWindowFocus();
		ImGui::Begin("HUD_Options", NULL, window_flags);
		if (ImGui::Button("Inventaire")){
			Gui::getInstance().showHUDOptions = false;
			Gui::getInstance().showHUDMap = false;
			Gui::getInstance().showHUDInventory = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Carte")){
			Gui::getInstance().showHUDOptions = false;
			Gui::getInstance().showHUDInventory = false;
			Gui::getInstance().showHUDMap = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Options")){
		}
		ImGui::Separator();

		ImGui::End();
	}

	// FORGET STYLE RULES
	ImGui::PopStyleColor(2);

}

void View::setAssets(Assets& _assets_ptr) {
    assets_ptr = std::shared_ptr<Assets>(&_assets_ptr);
}
