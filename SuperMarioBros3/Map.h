#pragma once
#include <Windows.h>
#include <vector>

#include "TileLayer.h"
#include "TileSet.h"
#include "GameObject.h"
#include "Text.h"

#define COORDINATE_ADJUST_SYNC_TILED 8
#define TEXT_COURSE_CLEAR	"COURSE CLEAR"
#define TEXT_YOU_GOT_A_CARD	"YOU GOT A CARD"
class CMap
{
protected:
	// Now a playscene's player becomes maps' player
	LPGAMEOBJECT player;

	int id;
	int width; //Unit: tile
	int height; //Unit: tile
	int tileWidth;
	int tileHeight;

	LPCWSTR mapFilePath;
	std::vector<LPTILESET> tileSets;
	std::vector<LPTILELAYER> tileLayers;
	std::vector<LPGAMEOBJECT> objects;

	unordered_map<string, CText*> texts;

	bool isPBlockTurnedOn;

	// This flag shows if camera's y coord has been modified (ex: when mario flies past a certain 
	// point) and then the game adjusts the camera accordingly
	bool isCameraYDefaultValue;

	bool isCourseCleared;

public:
	CMap(int id, LPCWSTR mapFilePath, int width, int height, int tileWidth, int tileHeight);

	void Add(LPTILELAYER layer);
	void Add(LPTILESET tileSet) { tileSets.push_back(tileSet); };
	void Add(LPGAMEOBJECT object) { objects.push_back(object); };
	void AddText(string textContent, CText* text) { texts[textContent] = text; }

	virtual void Update(DWORD dt);
	void Render();
	void Clear();
	void SetPlayer(LPGAMEOBJECT player) { this->player = player; }

	void GetSize(int& width, int& height)
	{
		width = this->width, height = this->height;
	};
	void GetTileSize(int& tileWidth, int& tileHeight)
	{
		tileWidth = this->tileWidth, tileHeight = this->tileHeight;
	};

	LPGAMEOBJECT GetPlayer() { return player; }

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
	void PurgeDeletedObjects();
	
	void TurnPBlockOn() { isPBlockTurnedOn = true; }
	void ClearCourse() { isCourseCleared = true; }
};

typedef CMap* LPMAP;