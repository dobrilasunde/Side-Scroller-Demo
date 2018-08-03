#include "AnimSpriteComponent.hpp"
#include "Math.hpp"
#include<iostream>


AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder) : SpriteComponent(owner, drawOrder), mCurrFrame(0.0f), mAnimFPS(24.0f), mAnimateFrom(0), mAnimateTo(0) {}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimateTo > 0 && mAnimTextures.size() > 0)
	{
		mCurrFrame += mAnimFPS * deltaTime;

		if (mLoop == true)
		{
			while (mCurrFrame >= mAnimateTo + 1)
			{
				mCurrFrame -= (mAnimateTo + 1 - mAnimateFrom);
			}

		}
		else
		{
			if (mFinishedAnimation)
			{
				mCurrFrame = static_cast<float> (mAnimateFrom);
			}
			else if (!mFinishedAnimation && mCurrFrame >= mAnimateTo + 1)
			{
				mFinishedAnimation = true;
				mCurrFrame = static_cast<float> (mAnimateFrom);
			}
		}

		SetTexture(mAnimTextures[static_cast<int> (mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0 && mAnimateTo > 0)
	{
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}
}

void AnimSpriteComponent::SetAnimation(const int from, const int to, bool loop)
{
	mAnimateFrom = from;
	mAnimateTo = to;
	mLoop = loop;
	mFinishedAnimation = false;

	if (mAnimateTo > 0 && mAnimTextures.size() > 0)
	{
		mCurrFrame = static_cast<float> (mAnimateFrom);
		SetTexture(mAnimTextures[mAnimateFrom]);
	}
}
