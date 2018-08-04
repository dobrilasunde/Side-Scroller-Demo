#include "Game.hpp"
#include <SDL_image.h>
#include <algorithm>
#include "Actor.hpp"
#include "SpriteComponent.hpp"
#include "Player.hpp"
#include "BGSpriteComponent.hpp"
#include"TileMapComponent.hpp"

Game::Game() :mWindow(nullptr), mRenderer(nullptr), mIsRunning(true), mUpdatingActors(false) {}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Side Scroller", 100, 100, 1024, 768, 0);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}
	

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::LoadData()
{
	mPlayer = new Player(this);
	mPlayer->SetPosition(Vector2(100.0f, 650.0f));
	mPlayer->SetScale(1.5f);

	Actor* temp = new Actor(this);
	temp->SetPosition(Vector2(512.0f, 384.0f));

	TileMapComponent* tmc = new TileMapComponent(temp, 90);
	tmc->LoadCSV("Assets/MapLayer1.csv");
	SDL_Texture* tmpTex = GetTexture("Assets/Tiles.png");
	tmc->SetScreenSize(Vector2(1024.0f, 768.0f));
	tmc->SetTexture(tmpTex);
	tmc->SetTileNumbers(8, 24);

	TileMapComponent* tmc1 = new TileMapComponent(temp, 80);
	tmc1->LoadCSV("Assets/MapLayer2.csv");
	tmc1->SetScreenSize(Vector2(1024.0f, 768.0f));
	tmc1->SetTexture(tmpTex);
	tmc1->SetTileNumbers(8, 24);

	TileMapComponent* tmc2 = new TileMapComponent(temp, 70);
	tmc2->LoadCSV("Assets/MapLayer3.csv");
	tmc2->SetScreenSize(Vector2(1024.0f, 768.0f));
	tmc2->SetTexture(tmpTex);
	tmc2->SetTileNumbers(8, 24);

	/*
	BGSpriteComponent* bg = new BGSpriteComponent(temp, 10);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/sky.png"),
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(0.0f);

	bg = new BGSpriteComponent(temp, 110);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/clouds_1.png"),
		GetTexture("Assets/clouds_1.png")
	};

	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);

	bg = new BGSpriteComponent(temp, 20);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/clouds_2.png"),
		GetTexture("Assets/clouds_2.png")
	};

	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-150.0f);

	bg = new BGSpriteComponent(temp, 30);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/rocks_1.png"),
		GetTexture("Assets/rocks_1.png")
	};

	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);

	bg = new BGSpriteComponent(temp, 40);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/clouds_3.png"),
		GetTexture("Assets/clouds_3.png")
	};

	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-250.0f);

	bg = new BGSpriteComponent(temp, 50);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/rocks_2.png"),
		GetTexture("Assets/rocks_2.png")
	};

	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-300.0f);

	bg = new BGSpriteComponent(temp, 60);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/clouds_4.png"),
		GetTexture("Assets/clouds_4.png")
	};

	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-350.0f);

*/
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}

	mTextures.clear();
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mPlayer->ProcessKeyboard(state);
	
}

void Game::Shutdown()
{
	UnloadData();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		//avoid erase copies
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		//avoid erase copies
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	//removes them from mActors
	for (auto actor : deadActors)
	{
		delete actor;
	}
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;

	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);

		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}

	return tex;
}

void Game::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}
