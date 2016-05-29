#ifndef __INDIANLAST2_H__
#define __INDIANLAST2_H__

#include "Enemy.h"

class IndianLast2 : public Enemy
{
private:
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;
	iPoint original_pos;
	Animation fly;
	Animation fly2;
	Animation dead;
	Animation idle;
	bool isdead = false;


public:


	IndianLast2(int x, int y);

	void Move();
	void OnCollision(Collider* c1, Collider* c2);
};

#endif // __INDIANLAST2_H__