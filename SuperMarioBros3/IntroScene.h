#pragma once

#include "TinyXml/tinyxml.h"

#include "Scene.h"
#include "GameObject.h"
#include "TileMap.h"
#include "Mario.h"

#define MARIO_1_ACTION_0_TIME	1000
#define MARIO_1_ACTION_1_TIME	1000
#define MARIO_1_ACTION_2_TIME	200
#define MARIO_1_ACTION_3_TIME	3000

#define MARIO_2_ACTION_0_TIME	1000
#define MARIO_2_ACTION_1_TIME	2000
#define MARIO_2_ACTION_2_TIME	200
#define MARIO_2_ACTION_3_TIME	3000

class CIntroScene : public CScene
{
protected:
	vector<LPGAMEOBJECT> objects;
	LPTILEMAP map;

	CMario* mario_1;
	vector<ULONGLONG> mario_1_actions_time;
	int mario_1_current_action;
	ULONGLONG mario_1_last_action_time;

	CMario* mario_2;
	vector<ULONGLONG> mario_2_actions_time;
	int mario_2_current_action;
	ULONGLONG mario_2_last_action_time;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SPRITE(string line);
	void _ParseSection_ANIMATION(string line);
	void _ParseSection_MAP(string line);
	void LoadMap(LPCWSTR mapFile);
	void _ParseSection_TILESET(TiXmlElement* xmlElementTileSet);
	void _ParseSection_TILELAYER(TiXmlElement* xmlElementTileLayer);
	void _ParseSection_OBJECTGROUP(TiXmlElement* xmlElementObjectGroup);

public:
	CIntroScene(int id, LPCWSTR filePath);
	void Load();
	void ProcessMario();
	void Update(DWORD dt);
	void Render();
	void Unload();
};

