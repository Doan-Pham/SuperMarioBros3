#pragma once
#include <Windows.h>
#include <vector>

#include "TileLayer.h"
#include "TileSet.h"
#include "GameObject.h"

#define COORDINATE_ADJUST_SYNC_TILED 8
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
	bool isPBlockTurnedOn;
	bool isCameraYDefaultValue;
	LPGAMEOBJECT player;
public:
	CMap(int id, LPCWSTR mapFilePath, int width, int height, int tileWidth, int tileHeight);

	void Add(LPTILELAYER layer);
	void Add(LPTILESET tileSet) { tileSets.push_back(tileSet); };
	void Add(LPGAMEOBJECT object) { objects.push_back(object); };

	virtual void Update(DWORD dt);
	void Render();
	void Clear();
	void SetPlayer(LPGAMEOBJECT player) { this->player = player; }
	void GetSize(int& width, int& height) { width = this->width, height = this->height; };
	void GetTileSize(int& tileWidth, int& tileHeight) 
	{tileWidth = this->tileWidth, tileHeight = this->tileHeight; };
	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
	const std::vector<LPGAMEOBJECT>& GetObjectsVector() { return objects; };
	void PurgeDeletedObjects();
	void TurnPBlockOn() { isPBlockTurnedOn = true; }
	LPGAMEOBJECT GetPlayer() { return player; }
};

typedef CMap* LPMAP;