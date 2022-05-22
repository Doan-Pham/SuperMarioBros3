#pragma once
#include <Windows.h>
#include <vector>

#include "TileLayer.h"
#include "TileSet.h"
#include "GameObject.h"

class CMap
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
	std::vector<LPGAMEOBJECT> objects;
public:
	CMap(int id, LPCWSTR mapFilePath, int width, int height, int tileWidth, int tileHeight);

	void Add(LPTILELAYER layer);
	void Add(LPTILESET tileSet) { tileSets.push_back(tileSet); };
	void Add(LPGAMEOBJECT object) { objects.push_back(object); };

	void Render();
	void Clear();

	void GetSize(int& width, int& height) { width = this->width, height = this->height; };
	void GetTileSize(int& tileWidth, int& tileHeight) 
	{tileWidth = this->tileWidth, tileHeight = this->tileHeight; };
	std::vector<LPGAMEOBJECT> GetObjectsVector() { return objects; };
};

typedef CMap* LPMAP;