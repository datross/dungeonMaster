#ifndef GUI_H
#define GUI_H

#include <vector>
#include <utility>

#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

class Gui {
public:
	static Gui& getInstance(){
        static Gui instance; // Guaranteed to be destroyed.
							 // Instantiated on first use.
        return instance;
    }

	void errorWindow(bool& isValidate){
		ImGui::Begin("Error");

			ImGui::Text("Le programme rencontre actuellement un problème.");
			ImGui::Text("Veuillez le relancer s'il vous plaît");
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::Button("Ok")){
				isValidate = true;
			}

		ImGui::End();
	}

	Gui(Gui const&)             = delete;
	void operator=(Gui const&)  = delete;

	// Boolean Main menu
	bool  showMainMenu,
		  showLevelSelector,
		  showSavesSelector,
		  showOptionsSelector,
		  showQuitPopUp;

	// Boolean HUD
	bool showIndicators,
		 showInventory,
		 showMap,
		 showOptions;

	// Textures menu (id and size)
	std::vector< std::pair<ImTextureID, ImVec2> > mainMenuTex;
	std::vector< std::pair<ImTextureID, ImVec2> > HUDTex;


private:
	Gui() : showMainMenu(true),
		  showLevelSelector(false),
		  showSavesSelector(false),
		  showOptionsSelector(false),
		  showQuitPopUp(false),

		  showIndicators(true),
		  showInventory(false),
		  showMap(false),
		  showOptions(false)
		  {}                    // Constructor? (the {} brackets) are needed here.
};

#endif
