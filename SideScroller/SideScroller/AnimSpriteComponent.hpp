#pragma once
#include "SpriteComponent.hpp"
#include <vector>
#include <string>

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(Actor* owner, int drawOrder = 100);
	void Update(float deltaTime) override;
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
	void SetAnimation(const int from, const int to, bool loop);

private:
	std::vector<SDL_Texture*> mAnimTextures;
	float mCurrFrame;
	float mAnimFPS;
	int mAnimateFrom;
	int mAnimateTo;
	bool mLoop;
	bool mFinishedAnimation;
};