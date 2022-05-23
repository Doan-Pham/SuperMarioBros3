#pragma once

#include "TinyXml/tinyxml.h"

#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "Map.h"
#include "GameObject.h"


class CPlayScene : public CScene
{
protected:
	// A play scene has to have player, right? - well now playscene has maps and maps have their own
	// player
	static vector<LPGAMEOBJECT> objects;
	static unordered_map<int, LPMAP> maps;
	static int current_map;
	int next_map = -1;

	void SwitchMap();
	
	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SPRITE(string line);
	void _ParseSection_ANIMATION(string line);

	void _ParseSection_MAP(string line);
	void LoadMap(LPCWSTR mapFile);
	void _ParseSection_TILESET(TiXmlElement* xmlElementTileSet, int mapId);
	void _ParseSection_TILELAYER(TiXmlElement* xmlElementTileLayer, int mapId);
	void _ParseSection_OBJECTGROUP(TiXmlElement* xmlElementObjectGroup, int mapId);

	// This flag shows if camera's y coord has been modified (ex: when mario flies past a certain 
	// point) and adjust the camera accordingly
	bool isCameraYDefaultValue;
	bool static isPBlockTurnedOn;

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	// This method still exists because some objects rely on it
	LPGAMEOBJECT GetPlayer() { return maps[current_map]->GetPlayer(); }

	void InitiateSwitchMap(int map_id);
	LPMAP GetCurrentMap() { return maps[current_map]; }

	// This method still exists because some objects rely on it
	void static AddObject(LPGAMEOBJECT object) { maps[current_map]->Add(object); }

	// This method still exists because a some objects rely on it
	void static TurnPBlockOn() { maps[current_map]->TurnPBlockOn(); }
	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

