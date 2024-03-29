#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "GuiButton.h"

#include "List.h"

class GuiManager : public Module
{
public:

	// Constructor
	GuiManager(bool startEnabled);

	// Destructor
	virtual ~GuiManager();

	// Called before the first frame
	 bool Start();

	 bool Update(float dt);

	bool Draw();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, SDL_Rect bounds, Module* observer, SDL_Rect sect, SDL_Texture* texture, SDL_Texture* textureSelected, SDL_Rect sliderBounds = { 0,0,0,0 });

public:

	List<GuiControl*> guiControlsList;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;

	SDL_Texture* texture;

};

#endif // __GUIMANAGER_H__