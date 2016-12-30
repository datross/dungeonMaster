#include "gui.h"
#include "Image.h"

#include <string>

Gui::Gui() : showMainMenu(true),
	  showLevelSelector(false),
	  showSavesSelector(false),
	  showOptionsSelector(false),
	  showQuitPopUp(false),

	  showIndicators(true),
	  showInventory(false),
	  showMap(false),
	  showOptions(false)
	  {}

void Gui::loadTexture (std::vector< std::pair<ImTextureID, ImVec2> >& container, glimac::FilePath path){
	    std::unique_ptr<glimac::Image> loadImg = glimac::loadImage(path);
	    if (loadImg == NULL) {
	        std::cerr << "Error while loading texture : " << path << std::endl;
	    }
	    GLuint TexId;
	    glGenTextures(1, &TexId);
	    glBindTexture(GL_TEXTURE_2D, TexId);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, loadImg->getWidth(), loadImg->getHeight(), 0, GL_RGBA, GL_FLOAT, loadImg->getPixels());
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glBindTexture(GL_TEXTURE_2D,0);

		std::pair<ImTextureID, ImVec2> new_tex;
		new_tex.first = (void *)(intptr_t)TexId;
		new_tex.second = ImVec2(loadImg->getWidth(), loadImg->getHeight());
		container.push_back(new_tex);
}

void Gui::errorWindow(bool& isValidate){
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
