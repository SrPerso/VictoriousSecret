#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"

#define NUM_MODULES 10


class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleRender;
class ModulePlayer;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleLevel1;
class ModuleLevel2;
class ModuleTitleScreen;
class Module;

class Application
{
public:

	Module* modules[NUM_MODULES];
	ModuleWindow* window;
	ModuleRender* render;
	ModuleInput* input;
	ModuleTextures* textures;
	ModuleTitleScreen* titlescreen;
	ModuleLevel1* level1;
	ModuleLevel2* level2;
	ModulePlayer* player;
	ModuleAudio* audio;
	ModuleFadeToBlack* fade;


public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__