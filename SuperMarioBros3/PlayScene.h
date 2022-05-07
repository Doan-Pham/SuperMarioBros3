#pragma once
#include "Game.h"
#include "Map.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "BrickQuestionMark.h"
#include "Leaf.h"
#include "MushroomBig.h"
#include "Mario.h"
#include "Goomba.h"
#include "TinyXml/tinyxml.h"
//#include "Koopas.h"


class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					

	// Make this static so other classes can access and add object
	// The classes accessing this must have a  ***CONST***  reference to the playscene
	static vector<LPGAMEOBJECT> objects;

	LPMAP map;

	void _ParseSection_ASSETS(string line);
	void _ParseSection_ASSETSXML(string line);
	void LoadAssets(LPCWSTR assetFile);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_MAP(string line);
	void LoadMap(LPCWSTR mapFile);
	void _ParseSection_TILESET(TiXmlElement* xmlElementTileSet);
	void _ParseSection_TILELAYER(TiXmlElement* xmlElementTileLayer);
	void _ParseSection_OBJECTGROUP(TiXmlElement* xmlElementObjectGroup);


public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void static AddObject(LPGAMEOBJECT object) { objects.push_back(object); }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

