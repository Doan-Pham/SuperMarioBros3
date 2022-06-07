#pragma once

#include "TinyXml/tinyxml.h"

#include "Scene.h"
#include "GameObject.h"
#include "TileMap.h"
#include "Mario.h"

#include "MushroomBig.h"
#include "Leaf.h"
#include "KoopaGreenNormal.h"
#include "Goomba.h"

#include "GameTitle.h"
#include "Tree.h"
#include "IntroArrow.h"

// These numbers mean that action "x" will last for "y"time
#define MARIO_1_ACTION_0_TIME	1000
#define MARIO_1_ACTION_1_TIME	1000
#define MARIO_1_ACTION_2_TIME	200
#define MARIO_1_ACTION_3_TIME	3000

#define MARIO_2_ACTION_0_TIME	1000
#define MARIO_2_ACTION_1_TIME	10
#define MARIO_2_ACTION_2_TIME	2000
#define MARIO_2_ACTION_3_TIME	2000
#define MARIO_2_ACTION_4_TIME	0
#define MARIO_2_ACTION_5_TIME	3000
#define MARIO_2_ACTION_6_TIME	1000
#define MARIO_2_ACTION_7_TIME	1000
#define MARIO_2_ACTION_8_TIME	1500
#define MARIO_2_ACTION_9_TIME	1000
#define MARIO_2_ACTION_10_TIME	1000
#define MARIO_2_ACTION_11_TIME	2000
#define MARIO_2_ACTION_12_TIME	2000

class CIntroScene : public CScene
{
protected:
	vector<LPGAMEOBJECT> objects;
	LPTILEMAP map;
	CGameTitle* title;
	CLeaf* leaf;
	CMushroomBig* mushroom;
	CKoopaGreenNormal* koopa_1;
	CKoopaGreenNormal* koopa_2;
	CGoomba* goomba;
	CTree* tree;
	CIntroArrow* arrow;

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
	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
	void PurgeDeletedObjects();
	LPGAMEOBJECT GetArrow() { return arrow; }
};

