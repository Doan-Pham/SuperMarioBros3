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
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;

	// Make this static so other classes can access and add object
	// The classes accessing this must have a  ***CONST***  reference to the playscene
	static vector<LPGAMEOBJECT> objects;

	unordered_map<int, LPMAP> maps;
	int current_map = -1;
	int next_map = -1;
	LPMAP map;

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

	LPGAMEOBJECT GetPlayer() { return player; }
	LPMAP GetMap() { return map; }

	void InitiateSwitchMap(int map_id);
	LPMAP GetCurrentMap() { return maps[current_map]; }
	void static AddObject(LPGAMEOBJECT object) { objects.push_back(object); }

	void Clear();
	void PurgeDeletedObjects();
	void static TurnPBlockOn() { isPBlockTurnedOn = true; }
	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

