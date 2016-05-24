#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "Audio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "SDL/include/SDL_timer.h"

#include "ModuleFadeToBlack.h"
#include "ModuleUI.h"
#include "ModulePlayer.h"
#include "ModuleText.h"

ModuleUI::ModuleUI()
{
	
}
ModuleUI::~ModuleUI()
{
}
bool ModuleUI::Start()
{
	LOG("Loading intro");
	timestart = SDL_GetTicks();

	UserInterface = App->textures->Load("Images/fonts.png");
	insert_coins.PushBack({ 0, 8, 59, 8 });
	Credit.PushBack({ 59, 8, 31, 8 });
	stage.PushBack({ 91, 8, 65, 8 });
	stage.PushBack({ 254, 9, 5, 5 });
	
	oneup.PushBack({ 163, 8, 17, 8 });
	top.PushBack({ 180, 8, 17, 8 });
	foe.PushBack({ 197, 8, 31, 8 });
	foe_bar.PushBack({ 229, 8,5, 8 });
	foe_bar_white.PushBack({ 234, 8, 5, 8 });
	ball.PushBack({ 240, 8, 9, 8 });
	dynamite_image.PushBack({ 322, 0, 38, 15 });

	App->render->Blit(UserInterface, 90, 15, &(stage.GetCurrentFrame()), 0.0);
	App->render->Blit(UserInterface, 100, 220, &(Credit.GetCurrentFrame()), 0.0);
	App->render->Blit(UserInterface, 90, 15, &(stage.GetCurrentFrame()), 0.0);

	App->render->Blit(UserInterface, 35, 203, &(foe.GetCurrentFrame()), 0.0);
	
	

	return true;
}
bool ModuleUI::CleanUp()
{
	LOG("Unloading intro");

	App->textures->Unload(UserInterface);
	
	return true;
}
update_status ModuleUI::Update()
{
	
	return UPDATE_CONTINUE;
}