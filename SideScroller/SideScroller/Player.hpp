#pragma once
#include "Actor.hpp"
#include"AnimSpriteComponent.hpp"
class Player : public Actor
{
public:
	Player(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ProcessKeyboard(const uint8_t* state);
	float GetRightSpeed() const { return mRightSpeed; }
	//float GetDownSpeed() const { return mDownSpeed; }

private:
	float mRightSpeed;
	//float mDownSpeed;
};