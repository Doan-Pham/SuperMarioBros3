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
	static unordered_map<int, COverworldNode*> nodes;
	LPGAMEOBJECT player;
	LPTILEMAP map;
	CHUD* bottomHUD;
	CPMeter* pMeter;

	int current_node;
	int destination_node;

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
	void Load();
	void Update(DWORD dt);
	void Render();
	void Unload();
	LPGAMEOBJECT GetPlayer() { return this->player; }
	static COverworldNode* GetNode(int id) { return nodes[id]; }

	COverworldNode* GetCurrentNode() { return nodes[current_node]; }
	COverworldNode* GetDestinationNode() { return nodes[destination_node]; }

	void SetDestinationNode(int destination_node) { this->destination_node = destination_node; }
	void SetCurrentNode(int current_node) { this->current_node = current_node; }
};

