#pragma once
#include "SpriteComponent.hpp"
#include <vector>
#include "Math.hpp"

class BGSpriteComponent : public SpriteComponent
{
public:
	BGSpriteComponent(Actor* owner, int drawOrder = 10);

	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;

	void SetBGTextures(const std::vector<SDL_Texture*> & textures);
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed() { return mScrollSpeed; }

private:
	struct BGTexture
	{
		SDL_Texture* mTexture;
		Vector2 mOffset;
	};
	std::vector<BGTexture> mBGTextures;
	Vector2 mScreenSize;
	float mScrollSpeed;
};
