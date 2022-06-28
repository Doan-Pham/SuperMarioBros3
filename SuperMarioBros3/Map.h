#pragma once
#include <Windows.h>
#include <vector>

#include "TileLayer.h"
#include "TileSet.h"
#include "GameObject.h"
#include "Text.h"
#include "Card.h"
#include "GridManager.h"

#define COORDINATE_ADJUST_SYNC_TILED 8
#define BOTTOM_HUD_HEIGHT 50

#define TEXT_COURSE_CLEAR	"COURSE CLEAR"
#define TEXT_YOU_GOT_A_CARD	"YOU GOT A CARD"

// Mario has to move past this distance before showing the next text
// This has to be smaller than CAMEARA_SURROUNDING_OFFSET, or else when mario goes past the map's
// right edge, he won't be updated anymore and the next text won't be shown
#define MARIO_DISTANCE_BEFORE_SHOW_NEXT_TEXT	75

// This offset expands the area around the camera so a certain amount of objects outside the camera
// can be updated and rendered before mario even reaches them, allowing for a smoother gameplay
#define CAMERA_SURROUNDING_OFFSET	100

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

	float mapLeftEdge, mapTopEdge, mapRightEdge, mapBottomEdge;

	LPCWSTR mapFilePath;
	std::vector<LPTILESET> tileSets;
	std::vector<LPTILELAYER> tileLayers;
	std::vector<LPGAMEOBJECT> objects;
	int maxObjectId;

	int gameLoopCount;

	CGridManager* mapGrid;
	int firstProcessedGridX, firstProcessedGridY, lastProcessedGridX, lastProcessedGridY;

	unordered_map<string, CText*> texts;
	CCard* clearCourseCard;
	bool isPBlockTurnedOn;

	// This flag shows if camera's y coord has been modified (ex: when mario flies past a certain 
	// point) and then the game adjusts the camera accordingly
	bool isCameraYDefaultValue;

	bool isAddedAcquiredCard;

public:
	CMap(int id, LPCWSTR mapFilePath, int width, int height, int
		tileWidth, int tileHeight, int gridSize = GRID_SIZE_STANDARD);

	void Add(LPTILELAYER layer);
	void Add(LPTILESET tileSet) { tileSets.push_back(tileSet); };
	void Add(LPGAMEOBJECT object) 
	{ 
		// Objects that are not preloaded but added in-game, so they don't have id
		if (object->GetId() == -1)
		{
			maxObjectId++;
			object->SetId(maxObjectId);
		}
		mapGrid->PutObjectInGrid(object);
		objects.push_back(object); 
	};

	void AddText(string textContent, CText* text) { texts[textContent] = text; }
	void AddClearCourseCard(CCard* card) { this->clearCourseCard = card; }

	virtual void Update(DWORD dt);
	void Render();
	void Clear();

	int GetMaxObjectId() { return maxObjectId; }
	void SetMaxObjectId(int maxObjectId) { this->maxObjectId = maxObjectId; }

	void GetSize(int& width, int& height)
	{
		width = this->width, height = this->height;
	};
	void GetTileSize(int& tileWidth, int& tileHeight)
	{
		tileWidth = this->tileWidth, tileHeight = this->tileHeight;
	};

	LPGAMEOBJECT GetPlayer() { return player; }
	void SetPlayer(LPGAMEOBJECT player) { this->player = player; }

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
	void PurgeDeletedObjects();
	
	void TurnPBlockOn() { isPBlockTurnedOn = true; }
};

typedef CMap* LPMAP;