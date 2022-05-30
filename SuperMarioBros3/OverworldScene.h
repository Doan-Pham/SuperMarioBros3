#pragma once

#include "TinyXml/tinyxml.h"

#include "GameObject.h"
#include "Scene.h"
#include "HUD.h"
#include "PMeter.h"
#include "TileMap.h"
#include "OverworldNode.h"

class COverworldScene : public CScene
{
protected:
	vector<LPGAMEOBJECT> objects;
	unordered_map<int, COverworldNode*> nodes;
	LPGAMEOBJECT player;

	LPTILEMAP map;
	CHUD* bottomHUD;
	CPMeter* pMeter;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SPRITE(string line);
	void _ParseSection_ANIMATION(string line);
	void _ParseSection_MAP(string line);

	void LoadMap(LPCWSTR mapFile);
	void _ParseSection_TILESET(TiXmlElement* xmlElementTileSet);
	void _ParseSection_TILELAYER(TiXmlElement* xmlElementTileLayer);
	void _ParseSection_OBJECTGROUP(TiXmlElement* xmlElementObjectGroup);

public:
	COverworldScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

};

