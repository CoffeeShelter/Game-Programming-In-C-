#pragma once
#include "Actor.h"
#include "SDL/SDL.h"

class Ship : public Actor
{
public:
	Ship(class Game *game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t *keyState) override;

private:
	float mLaserCooldown;
};