#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, SDL_Rect sect, SDL_Texture* texture, SDL_Texture* textureSelected) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->texture = texture;
	this->textureSelected = textureSelected;
	this->sect = sect;

	canClick = true;
	drawBasic = false;
}

GuiButton::~GuiButton()
{

}

bool GuiButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {
			
			state = GuiControlState::FOCUSED;
			/*if (previousState != state) {
				LOG("Change state from %d to %d",previousState,state);
				app->audio->PlayFx(audioFxId);
			}*/

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN) {
				state = GuiControlState::PRESSED;
				NotifyObserver();
			}

			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::SELECTED;
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}
	}

	return false;
}

bool GuiButton::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State

	switch (state)
	{
	case GuiControlState::DISABLED:
		//app->render->DrawTexture(texture, bounds.x, bounds.y, &sect, false);
		break;
	case GuiControlState::NORMAL:
		app->render->DrawTexture(texture, bounds.x, bounds.y, &sect, false);
		break;
	case GuiControlState::FOCUSED:
		app->render->DrawTexture(textureSelected, bounds.x, bounds.y, &sect, false);
		break;
	case GuiControlState::PRESSED:
		app->render->DrawTexture(textureSelected, bounds.x, bounds.y, &sect, false);
		break;
	}

	return false;
}