#include <iostream>
#include "view.h"

#include "gui.h"

View::View()
    : player_input(INPUT_NONE) {
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
    glEnable(GL_DEPTH_TEST);

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
    while(SDL_PollEvent(&event))
    {
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
}

void View::reshape(unsigned w, unsigned h) {
    window_width  = w;
    window_height = h;
    float aspect_ratio = (float)w/h;

    /* update des matrices de projection des players */
    // TODO gérer proprement la fov des camera
    for(auto it = assets_ptr->map.players.begin(); it != assets_ptr->map.players.end(); ++it) {
        it->cam.init(70., aspect_ratio);
    }
}

void View::render(Game_state& game_state) {
    glClear(GL_COLOR_BUFFER_BIT);

    /* USER INTERFACE */
    ImGui_ImplSdlGL3_NewFrame(window);
    if(game_state == STATE_MENU) {
        mainMenu(game_state);
    } else if (game_state == STATE_GAMEPLAY) {
        renderGame(game_state);
    } else if (game_state == STATE_QUIT) {

    } else {

    }
    ImGui::Render();

    SDL_GL_SwapWindow(window);
}

Player_input View::get_input() {
    return player_input;
}

void View::mainMenu(Game_state& game_state){

	/*Menu's textures */
	float tex_w = (float)ImGui::GetIO().Fonts->TexWidth;
	float tex_h = (float)ImGui::GetIO().Fonts->TexHeight;
	ImTextureID tex_id = ImGui::GetIO().Fonts->TexID;

	/* Windows parameters */
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	/*MAIN MENU */
	if(Gui::getInstance().showMainMenu){
		/*Create window */
		ImGui::Begin("Main menu", &(Gui::getInstance().showMainMenu), window_flags);

		/*Buttons */
		if (ImGui::Button("Nouvelle partie")){
			Gui::getInstance().showMainMenu = false;
			Gui::getInstance().showLevelSelector = true;
		}
		if (ImGui::Button("Charger partie")){
			Gui::getInstance().showMainMenu = false;
			Gui::getInstance().showSavesSelector = true;
		}
		if (ImGui::Button("Options")){
			Gui::getInstance().showMainMenu = false;
			Gui::getInstance().showOptionsSelector = true;
		}
		if (ImGui::Button("Quitter")){
			ImGui::OpenPopup("Quit");
		}

		/*QUIT CONFIRMAION POPUP */
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

	/*LOAD SAVES MENU */
	if(Gui::getInstance().showSavesSelector){
		/*init window */
		ImGui::Begin("Load saves menu", &(Gui::getInstance().showSavesSelector), window_flags);

		/*Buttons */
		if (ImGui::Button("Tinymap")){
			assets_ptr->load("tinymap", false);
			game_state = STATE_GAMEPLAY;
			Gui::getInstance().showSavesSelector = false;
			Gui::getInstance().showMainMenu = true;
		}
		if (ImGui::Button("Retour")){
			Gui::getInstance().showSavesSelector = false;
			Gui::getInstance().showMainMenu = true;
		}

		ImGui::End();
	}

	/*LEVEL MENU */
	if(Gui::getInstance().showLevelSelector){
		/*Init window */
		ImGui::Begin("Level menu", &(Gui::getInstance().showLevelSelector), window_flags);

		/*Buttons */
		if (ImGui::Button("Tinymap")){
			assets_ptr->load("tinymap", true);
			game_state = STATE_GAMEPLAY;
			Gui::getInstance().showLevelSelector = false;
			Gui::getInstance().showMainMenu = true;
		}
		if (ImGui::Button("Retour")){
			Gui::getInstance().showLevelSelector = false;
			Gui::getInstance().showMainMenu = true;
		}

		ImGui::End();
	}

	/*OPTION MENU */
	if(Gui::getInstance().showOptionsSelector){
		/*Init window */
		ImGui::Begin("Level menu", &(Gui::getInstance().showOptionsSelector), window_flags);

		/*Buttons */
		if (ImGui::Button("Retour")){
			Gui::getInstance().showOptionsSelector = false;
			Gui::getInstance().showMainMenu = true;
		}

		ImGui::End();
	}
}

void View::renderGame(Game_state& game_state) {   
    // TODO temporaire je sais pas trop où le mettre pour l'instant
    reshape(window_width, window_height);
    
    Mesh mesh;
    mesh.buildPlane(1, 1);
    auto shader = glimac::loadProgram("res/shaders/3D.vs.glsl",
                    "res/shaders/directionallight.fs.glsl");
    mesh.setUniformsId(shader);
    
    /* Pour toutes les vues des joueurs */
    for(auto p = assets_ptr->map.players.begin(); p != assets_ptr->map.players.end(); ++p) {
        // TODO checker la position
        p->cam.position = glm::vec3(p->position.x, 0.7, -p->position.y);
        std::cout << p->cam.position.x << " " << p->cam.position.z << std::endl;
        p->orientation = glm::vec3(0,0,-1);
        p->cam.direction = p->orientation;

        glm::mat4 v = p->cam.getVMatrix();
        
        shader.use();
        
        /* Walls rendering */
        for(unsigned x = 0; x < assets_ptr->map.datas.size(); ++x) {
            for(unsigned y = 0; y < assets_ptr->map.datas[0].size(); ++y) {
                glm::mat4 mv = v;
                //mv = glm::scale(v, glm::vec3(0.1,0.1,0.1));
                mv = glm::translate(mv, glm::vec3(x,0,-y));
                
                mesh.setMVMatrix(mv);
                mesh.setMVPMatrix(p->cam.getPMatrix() * mv);
                mesh.setNormalMatrix(glm::transpose(glm::inverse(mv)));
                mesh.setNormalMatrix(glm::mat4(1.));
                mesh.setShininess(1.);
                mesh.setLightDir_vs(glm::vec3(1,1,1));
                mesh.setLightIntensity(glm::vec3(1,1,1));
                mesh.setKs(glm::vec3(1,1,1));
                mesh.setKd(glm::vec3(1,1,1));
                
                mesh.render();
            }
        }
    }
}

void View::setAssets(Assets& _assets_ptr) {
    assets_ptr = std::shared_ptr<Assets>(&_assets_ptr);
}
