#pragma once

#include "TinyXml/tinyxml.h"

#include "Scene.h"
#include "GameObject.h"
#include "TileMap.h"
#include "Mario.h"

class CIntroScene : public CScene
{
protected:
	vector<LPGAMEOBJECT> objects;
	LPTILEMAP map;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SPRITE(string line);
	void _ParseSection_ANIMATION(string line);
	void _ParseSection_MAP(string line);
	void LoadMap(LPCWSTR mapFile);
	void _ParseSection_TILESET(TiXmlElement* xmlElementTileSet);
	void _ParseSection_TILELAYER(TiXmlElement* xmlElementTileLayer);
	void _ParseSection_OBJECTGROUP(TiXmlElement* xmlElementObjectGroup);

	CMario* mario_1;
	CMario* mario_2;

public:
	CIntroScene(int id, LPCWSTR filePath);
	void Load();
	void Update(DWORD dt);
	void Render();
	void Unload();
};

