#pragma once
#include "SpriteComponent.hpp"
#include <vector>

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(Actor* owner, int drawOrder = 100);
	void Update(float deltaTime) override;
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }

private:
	std::vector<SDL_Texture*> mAnimTextures;
	float mCurrFrame;
	float mAnimFPS;
};