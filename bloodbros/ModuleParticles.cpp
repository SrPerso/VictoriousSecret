#include <math.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "Application.h"
#include "ModulePlayer.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;

	Scope.anim.PushBack({ 209, 278, 11, 7 });
	Scope.anim.PushBack({ 226, 278, 11, 9 });
	Scope.anim.loop = true;
	Scope.anim.speed = 0.3f;
	Scope.speed.y = -3;
	Scope.life = 2000;

	housesmoke.anim.PushBack({ 5, 92, 100, 34 });
	housesmoke.anim.PushBack({ 105, 92, 100, 34 });
	housesmoke.anim.PushBack({ 205, 92, 100, 34 });
	housesmoke.anim.PushBack({ 305, 92, 100, 34 });
	housesmoke.anim.loop = true;
	housesmoke.anim.speed = 0.1f;
	housesmoke.life = 3000;


	Cowboyshot.anim.PushBack({ 213, 9, 7, 7 });
	Cowboyshot.anim.PushBack({ 230, 9, 7, 7 });
	Cowboyshot.anim.loop = true;
	Cowboyshot.anim.speed = 0.1f;
	Cowboyshot.speed.x = 1;
	Cowboyshot.speed.y = 1;
	Cowboyshot.collides = true;
	Cowboyshot.life = 3000;

	gunflare.anim.PushBack({ 150, 4, 16, 17 });
	gunflare.anim.PushBack({ 171, 4, 16, 17 });
	gunflare.anim.PushBack({ 190, 4, 16, 17 });
	gunflare.anim.speed = 0.3f;

	

	Planebomb.anim.PushBack({286, 6, 16, 16});
	Planebomb.anim.PushBack({ 303, 6, 16, 16 });
	Planebomb.anim.PushBack({ 320, 6, 16, 16 });
	Planebomb.anim.PushBack({ 337, 6, 16, 16 });
	Planebomb.anim.PushBack({ 354, 6, 16, 16 });
	Planebomb.anim.speed = 0.15f;
	Planebomb.anim.loop = false;
	Planebomb.life = 3000;
	Planebomb.type = planebomb;

	Hitbomb.anim.PushBack({ 5, 29, 33, 58 });
	Hitbomb.anim.PushBack({ 39, 29, 33, 58 });
	Hitbomb.anim.PushBack({ 73, 29, 33, 58 });
	Hitbomb.anim.PushBack({ 107, 29, 33, 58 });
	Hitbomb.anim.PushBack({ 141, 29, 33, 58 });
	Hitbomb.anim.PushBack({ 175, 29, 33, 58 });
	Hitbomb.anim.PushBack({ 209, 29, 33, 58 });
	Hitbomb.anim.PushBack({ 243, 29, 33, 58 });
	Hitbomb.anim.speed = 0.15f;
	Hitbomb.anim.loop = true;
	Hitbomb.life = 800;
	

}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("Images/Particles.png");
	
	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
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
update_status ModuleParticles::Update()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* p = active[i];

		if (p == nullptr)
			continue;

		if (p->Update() == false)
		{
			delete p;
			active[i] = nullptr;
		}
		else if (SDL_GetTicks() >= p->born)
		{
			App->render->Blit(graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()));
			if (p->fx_played == false)
			{
				p->fx_played = true;
				// Play particle fx here
			}
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y, Uint32 delay)
{
	Particle* p = new Particle(particle);
	p->born = SDL_GetTicks() + delay;
	p->position.x = x;
	p->position.y = y;

	active[last_particle++] = p;
}
void ModuleParticles::AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type, Uint32 delay)
{
	Particle* p = new Particle(particle);
	p->born = SDL_GetTicks() + delay;
	p->position.x = x;
	p->position.y = y;

	if (collider_type != COLLIDER_NONE)
		p->collider = App->collision->AddCollider(p->anim.GetCurrentFrame(), collider_type, this);
	active[last_particle++] = p;

}
void ModuleParticles::AddParticle(const Particle& particle, int x, int y, float speedx, float speedy, COLLIDER_TYPE collider_type, Uint32 delay)
{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			p->speed.x = speedx;
			p->speed.y = speedy;
			if (collider_type != COLLIDER_NONE)
				p->collider = App->collision->AddCollider(p->anim.GetCurrentFrame(), collider_type, this);
			active[last_particle++] = p;
	
}


void ModuleParticles::OnCollision(Collider* c1, Collider* c2){
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{

		if (active[i] != nullptr && active[i]->collider->type==COLLIDER_BOMB){
			active[i]->anim.Reset();
			App->particles->AddParticle(App->particles->Hitbomb, active[i]->position.x, active[i]->position.y-45, 0.0f, +0.0f, COLLIDER_NONE, 0);
			delete active[i];
			active[i] = nullptr;
			break;
		}
		// Always destroy particles that collide
		else if (active[i] != nullptr && active[i]->collider == c1 && active[i]->type!=planebomb)
		{
			//AddParticle(explosion, active[i]->position.x, active[i]->position.y);
			delete active[i];
			active[i] = nullptr;
			break;
		}
		
	}
}
// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
anim(p.anim), position(p.position), speed(p.speed),
fx(p.fx), born(p.born), life(p.life)
{}

Particle::~Particle()
{
	if (collider != nullptr)
		App->collision->EraseCollider(collider);
}

bool Particle::Update()
{
	bool ret = true;

	if (life > 0)
	{
		if ((SDL_GetTicks() - born) > life)
			ret = false;
	}
	else
	if (anim.Finished())
		ret = false;

	position.x += speed.x;
	position.y += speed.y;

	

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	return ret;
}

