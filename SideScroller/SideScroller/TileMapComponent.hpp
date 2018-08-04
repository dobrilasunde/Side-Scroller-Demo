#pragma once

#include"SpriteComponent.hpp"
#include<string>
#include<vector>
#include "Math.hpp"

class TileMapComponent : public SpriteComponent
{
public:
	TileMapComponent(Actor* owner, int drawOrder = 100);
	void LoadCSV(std::string fileName);
	void Draw(SDL_Renderer* renderer) override;
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetTileNumbers(const int tileRow, const int tileColumn) { mTilePerRow = tileRow; mTilePerColumn = tileColumn; }

private:
	std::vector<int> mTileMap;
	int mRows;
	int mColumns;
	Vector2 mScreenSize;
	int mTilePerRow;
	int mTilePerColumn;
};