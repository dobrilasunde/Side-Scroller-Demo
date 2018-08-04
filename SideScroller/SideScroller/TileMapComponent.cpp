#include"TileMapComponent.hpp"
#include<fstream>
#include<iostream>
#include<sstream>

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder) : SpriteComponent(owner, drawOrder) {}

void TileMapComponent::LoadCSV(std::string fileName)
{
	
	std::string line;
	std::ifstream fin(fileName.c_str());
	if (!fin)
	{
		std::cout << "Can't load CSV file" << std::endl;
	}

	getline(fin, line);

	std::stringstream ss(line);

	int i;

	while (ss >> i)
	{
		mTileMap.push_back(i);

		if (ss.peek() == ',')
			ss.ignore();
	}

	mColumns = mTileMap.size();

	while (!fin.eof())
	{
		getline(fin, line);
		std::stringstream ss(line);

		while (ss >> i)
		{
			mTileMap.push_back(i);

			if (ss.peek() == ',')
				ss.ignore();
		}
	}

	mRows = mTileMap.size() / mColumns;
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		for (int i = 0; i < mTileMap.size(); ++i)
		{
			
			if (mTileMap[i] != -1)
			{
				SDL_Rect rd;
				SDL_Rect rs;

				rd.w = static_cast<int>(mScreenSize.x / mColumns);
				rd.h = static_cast<int>(mScreenSize.y / mRows);
				rd.x = static_cast<int>((i % mColumns) * rd.w);
				rd.y = static_cast<int>((i / mColumns) * rd.h);

				rs.w = static_cast<int>(mTexWidth / mTilePerRow);
				rs.h = static_cast<int>(mTexHeight / mTilePerColumn);
				rs.x = static_cast<int>((mTileMap[i] % mTilePerRow) * rs.w);
				rs.y = static_cast<int>((mTileMap[i] / mTilePerRow) * rs.h);

				if (i == 1)
				{
					std::cout << mTileMap[i] << std::endl;
					std::cout << static_cast<int>((mTileMap[i] % mTilePerRow) * rs.w) << " " << static_cast<int>((mTileMap[i] / mTilePerRow) * rs.h) << std::endl;
					std::cout << rs.w << " " << rs.h << std::endl;
				}

				SDL_RenderCopy(renderer,
					mTexture,
					&rs,
					&rd
				);
			}
			
		}
	}
}