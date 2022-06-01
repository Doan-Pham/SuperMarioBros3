#pragma once
#include "TinyXml/tinyxml.h"

#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "Map.h"
#include "GameObject.h"
#include "HUD.h"
#include "PMeter.h"

// The time it takes to reduce one second from the game's playscene time (which is 1 second by default)
#define REDUCE_TIME_TIMEOUT 1000
#define TIME_REDUCE_AMOUNT_DEFAULT 1

#define TIME_REDUCE_AMOUNT_COURSE_CLEAR		20
#define SCORE_PER_SECOND_AFTER_COURSE_CLEAR	50

class CPlayScene : public CScene
{
protected:
	// A play scene has to have player, right? - well now playscene has maps and maps have their own
	// player
	static vector<LPGAMEOBJECT> objects;
	static unordered_map<int, LPMAP> maps;
	static int current_map;
	int next_map = -1;

	bool isGameOver;
	bool static isCourseClear;

	ULONGLONG reduce_time_start;

	CHUD* bottomHUD;
	CPMeter* pMeter;

	void SwitchMap();

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SPRITE(string line);
	void _ParseSection_ANIMATION(string line);

	void _ParseSection_MAP(string line);
	void LoadMap(LPCWSTR mapFile);
	void _ParseSection_TILESET(TiXmlElement* xmlElementTileSet, int mapId);
	void _ParseSection_TILELAYER(TiXmlElement* xmlElementTileLayer, int mapId);
	void _ParseSection_OBJECTGROUP(TiXmlElement* xmlElementObjectGroup, int mapId);

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	// This method still exists because some objects rely on it
	LPGAMEOBJECT GetPlayer() { return maps[current_map]->GetPlayer(); }

	CPMeter* GetPMeter() { return pMeter; }

	void InitiateSwitchMap(int map_id);
	LPMAP GetCurrentMap() { return maps[current_map]; }

	// This method still exists because some objects rely on it
	void static AddObject(LPGAMEOBJECT object) { maps[current_map]->Add(object); }

	// This method still exists because a some objects rely on it
	void static TurnPBlockOn() { maps[current_map]->TurnPBlockOn(); }

	bool static IsCourseClear() { return isCourseClear; }
	void static ClearCourse() { isCourseClear = true; }

	void SetGameOver() { isGameOver = true;}
	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

