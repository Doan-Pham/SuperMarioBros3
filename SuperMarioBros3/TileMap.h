#pragma once
#include <Windows.h>
#include <vector>

#include "TileLayer.h"
#include "TileSet.h"

// A simple map that contains only tile layers! (Useful for scenes that manage their own objects)
class CTileMap
{
protected:
	int id;
	int width; //Unit: tile
	int height; //Unit: tile
	int tileWidth;
	int tileHeight;

	LPCWSTR mapFilePath;
	std::vector<LPTILESET> tileSets;
	std::vector<LPTILELAYER> tileLayers;

public:
	CTileMap(int id, LPCWSTR mapFilePath, int width, int height, int tileWidth, int tileHeight);

	void Add(LPTILELAYER layer);
	void Add(LPTILESET tileSet) { tileSets.push_back(tileSet); };

	void Render();
	void Clear();

	void GetSize(int& width, int& height) { width = this->width, height = this->height; };
	void GetTileSize(int& tileWidth, int& tileHeight)
	{
		tileWidth = this->tileWidth, tileHeight = this->tileHeight;
	};
};

typedef CTileMap* LPTILEMAP;
