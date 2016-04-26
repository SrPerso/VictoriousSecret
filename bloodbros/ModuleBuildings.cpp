#include <math.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "Application.h"
#include "ModulePlayer.h"
#include"Animation.h"
#include "Extras.h"
#include "ModuleFadeToBlack.h"
#include "ModuleLevel1.h"
#include "ModuleLevel2.h"
#include "Audio.h"
#include "ModuleBuildings.h"

#include "SDL/include/SDL_timer.h"

ModuleBuilding::ModuleBuilding()
{
	for (uint i = 0; i < MAX_BUILDINGS; ++i)
		active[i] = nullptr;
}

ModuleBuilding::~ModuleBuilding()
{
	if (yellow.collider != nullptr){
		App->collision->EraseCollider(yellow.collider);
	}
}

// Load assets
bool ModuleBuilding::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("buildings.png");


	yellow.build.x = 1;
	yellow.build.y = 2;
	yellow.build.w = 94;
	yellow.build.h = 95;
	yellow.mytype = YELLOW;

	yellow2.build.x = 104;
	yellow2.build.y = 2;
	yellow2.build.w = 94;
	yellow2.build.h = 95;

	yellow.destroy.PushBack({ 104, 2, 94, 95 });
	yellow.destroy.PushBack({ 104, 2, 94, 82});
	yellow.destroy.PushBack({ 104, 2, 94, 69 });
	yellow.destroy.PushBack({ 104, 2, 94, 56 });
	yellow.destroy.PushBack({ 104, 2, 94, 43 });
	yellow.destroy.PushBack({ 104, 2, 94, 30 });
	yellow.destroy.PushBack({ 104, 2, 94, 17 });
	yellow.destroy.PushBack({ 104, 2, 94, 4 });
	yellow.destroy.loop = false;
	yellow.destroy.speed = 0.05f;

	return true;
}

// Unload assets
bool ModuleBuilding::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);

	for (uint i = 0; i < MAX_BUILDINGS; ++i)
	{
		if (active[i] != nullptr)
		{
			delete active[i];
			active[i] = nullptr;
		}
	}

	return true;
}

// Update: draw background
update_status ModuleBuilding::Update()
{

	for (uint i = 0; i < MAX_BUILDINGS; ++i)
	{
		Building* p = active[i];

		if (p == nullptr)
			continue;

		if (p->Update() == false)
		{
			p->collider->to_delete = true;
			delete p;
			active[i] = nullptr;

		}
		else if (p->hits <= 1)
		{
			App->render->Blit(graphics, p->position.x, p->position.y, &p->build, 0);
			if (p->fx_played == false)
			{
				p->fx_played = true;
				// Play particle fx here
			}
		}
		else if (p->hits > 1){
		
			App->render->Blit(graphics, p->position.x, p->position.y+=0.6f, &p->destroy.GetCurrentFrame());
			
			/*p->collider->to_delete = true;
			delete p;*/
			if (p->destroy.Finished()){
				active[i] = nullptr;
			}
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleBuilding::AddBuilding(const Building& particle, int x, int y)
{
	Building* p = new Building(particle);
	p->position.x = x;
	p->position.y = y;
	p->collider = App->collision->AddCollider({p->position.x,p->position.y, 96, 98 }, COLLIDER_EXTRA, this);
	active[last_building++] = p;
}

const Collider* Building::get_collider() const
{
	return collider;
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Building::Building()
{
	position.SetToZero();
}

Building::Building(const Building& p) :
build(p.build), position(p.position),
fx(p.fx), mytype(p.mytype), destroy(p.destroy)
{}

Building::~Building(){
	if (collider != nullptr){
		App->collision->EraseCollider(collider);
	}
}

bool Building::Update()
{
	bool ret = true;

	return ret;
}
void ModuleBuilding::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_BUILDINGS; ++i){
		if (active[i] != nullptr && active[i]->get_collider() == c1 && active[i]->mytype==YELLOW){
			if (active[i]->hits == 0){
				active[i]->build.x = yellow2.build.x;
				active[i]->build.y = yellow2.build.y;
				active[i]->build.w = yellow2.build.w;
				active[i]->build.h = yellow2.build.h;
				active[i]->hits++;
				break;
			}
			else if (active[i]->hits == 1){
				active[i]->build.x = 0;
				active[i]->build.y = 0;
				active[i]->build.w = 0;
				active[i]->build.h = 0;
				active[i]->hits++;
				App->particles->AddParticle(App->particles->housesmoke, active[i]->position.x + 94, active[i]->position.y + 95);
				delete[] active[i]->collider;
				active[i]->collider = nullptr;//PROBLEM
			}
		}
	}
}