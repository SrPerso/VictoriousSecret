#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleEnemies.h"
#include "ModuleParticles.h"
#include "ModuleTextures.h"
#include "Enemy.h"
#include "Charriot.h"
#include "GreenCowBoy.h"
#include "PurplePlane.h"
#include "BlueCowBoy.h"
#include "GreenRight.h"
#include "ModulePlayer.h"
#include "Jumper.h"
#include "GreenCowBoyMedium.h"
#include "IndianMedium.h"
#include "GreenCowBoyLeavingTheHouse.h"
#include "IndianLast.h"
#include "IndianLast2.h"
#include "FarIndian.h"
#include "FarIndian2.h"

/*#include "Enemy_RedBird.h"
#include "Enemy_Cookie.h"
#include "Enemy_Mech.h"*/

#define SPAWN_MARGIN 50

ModuleEnemies::ModuleEnemies()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
	
}

// Destructor
ModuleEnemies::~ModuleEnemies()
{
}

bool ModuleEnemies::Start()
{

	sprites = App->textures->Load("Images/Enemies.png");
	hits = 0;
	

	return true;
}

update_status ModuleEnemies::PreUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			if (queue[i].x * SCREEN_SIZE < App->render->camera.x + (App->render->camera.w * SCREEN_SIZE) + SPAWN_MARGIN)
			{
				SpawnEnemy(queue[i]);
				queue[i].type = ENEMY_TYPES::NO_TYPE;
				LOG("Spawning enemy at %d", queue[i].x * SCREEN_SIZE);
			}
		}
	}

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->to_delete == true)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	return UPDATE_CONTINUE;
}

// Called before render is available
update_status ModuleEnemies::Update()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Move();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Draw(sprites);

	return UPDATE_CONTINUE;
}

update_status ModuleEnemies::PostUpdate()
{
	// check camera position to decide what to spawn
	/*for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			if (enemies[i]->position.x * SCREEN_SIZE < (App->render->camera.x) - SPAWN_MARGIN)
			{
				LOG("DeSpawning enemy at %d", enemies[i]->position.x * SCREEN_SIZE);
				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	}*/

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEnemies::CleanUp()
{
	LOG("Freeing all enemies");

	App->textures->Unload(sprites);
	

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	return true;
}

bool ModuleEnemies::AddEnemy(ENEMY_TYPES type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void ModuleEnemies::SpawnEnemy(const EnemyInfo& info)
{

	uint i = 0;
	for (; enemies[i] != nullptr && i < MAX_ENEMIES; ++i); {

		if (i != MAX_ENEMIES)
		{
			switch (info.type)
			{
			case ENEMY_TYPES::CHARRIOT:
				enemies[i] = new Enemy_Charriot(info.x, info.y);
				break;
			case ENEMY_TYPES::GREENCOWBOY:
				enemies[i] = new GreenCowboy(info.x, info.y);
				break;
			case ENEMY_TYPES::PURPLEPLANE:
				enemies[i] = new PurplePlane(info.x, info.y);
				break;
			case ENEMY_TYPES::BLUECOWBOY:
				enemies[i] = new BlueCowboy(info.x, info.y);
				break;
			case ENEMY_TYPES::GREENRIGHTCOWBOY:
				enemies[i] = new GreenRightCowboy(info.x, info.y);
				break;
			case ENEMY_TYPES::INDIANLAST:
				enemies[i] = new IndianLast(info.x, info.y);
				break;
			case ENEMY_TYPES::INDIANLAST2:
				enemies[i] = new IndianLast2(info.x, info.y);
				break;
		
			
			}
			
			

		}
	}
	
}

void ModuleEnemies::OnCollision(Collider* c1, Collider* c2)
{
	if (App->player->hit == true){
		App->player->hit = false;
		for (uint i = 0; i < MAX_ENEMIES; ++i)
		{
			if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
			{
				hits++;
				App->player->score+=100;
				enemies[i]->OnCollision(c1, c2);
				//delete enemies[i];
				//enemies[i] = nullptr;
				break;
			}
		}
	}
}
