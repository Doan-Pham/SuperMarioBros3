#include <iostream>
#include <fstream>

#include "OverworldScene.h"
#include "OverworldKeyEventHandler.h"
#include "Utils.h"

#include "AssetIDs.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"

#include "OverworldMario.h"

#define MAX_SCENE_LINE 1024

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_SETTINGS 1
#define SCENE_SECTION_SPRITE 2
#define SCENE_SECTION_ANIMATION	3
#define SCENE_SECTION_MAP 4

#define BOTTOM_HUD_HEIGHT 50

unordered_map<int, COverworldNode*> COverworldScene::nodes;

COverworldScene::COverworldScene(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	player = NULL;
	map = NULL;
	key_handler = new COverworldKeyEventHandler(this);
	float back_buffer_width = CGame::GetInstance()->GetBackBufferWidth();
	float back_buffer_height = CGame::GetInstance()->GetBackBufferHeight();

	pMeter = new CPMeter();

	bottomHUD = new CHUD(back_buffer_width / 2, back_buffer_height - BOTTOM_HUD_HEIGHT / 2,
		back_buffer_width, BOTTOM_HUD_HEIGHT);

	bottomHUD->SetPMeter(pMeter);

	current_node = -1;
	destination_node = -1;
}

void COverworldScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#' || line == "") continue;	// skip comment lines and empty lines
		if (line == "[SPRITE]") { section = SCENE_SECTION_SPRITE; continue; }
		if (line == "[ANIMATION]") { section = SCENE_SECTION_ANIMATION; continue; };
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case SCENE_SECTION_SPRITE: _ParseSection_SPRITE(line); break;
		case SCENE_SECTION_ANIMATION: _ParseSection_ANIMATION(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void COverworldScene::Update(DWORD dt)
{
	if (player == NULL) return;

	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
// TO-DO: This is a "dirty" way, need a more organized way 
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		if (!objects[i]->IsHidden())
		{
			coObjects.push_back(objects[i]);
		}

	}

	for (size_t i = 0; i < objects.size(); i++)
	{
			objects[i]->Update(dt, &coObjects);
	}
	bottomHUD->Update(dt);
	DebugOutTitle(L"Current node: %i, destination node: %i", current_node, destination_node);
}

void COverworldScene::Render()
{
	if (map != nullptr) map->Render();

	for (unsigned int i = 0; i < objects.size(); i++)
	{
			objects[i]->Render();
	}
	bottomHUD->Render();
}

void COverworldScene::Unload()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	player = NULL;

	if (map != NULL)
	{
		map->Clear();
		map = NULL;
	}

	CSprites::GetInstance()->Clear();
	CAnimations::GetInstance()->Clear();
	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

void COverworldScene::_ParseSection_SETTINGS(string line)
{
}

void COverworldScene::_ParseSection_SPRITE(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	TiXmlDocument doc(tokens[0].c_str());
	bool result = doc.LoadFile();

	if (!result)
	{
		DebugOut(L"[ERROR] Failed to load sprites from %s\n", path.c_str());
		return;
	}

	DebugOut(L"[INFO] Start loading sprites from : %s \n", path.c_str());

	TiXmlElement* root = doc.FirstChildElement();
	int textureId = atoi(root->Attribute("textureId"));

	LPTEXTURE tex = CTextures::GetInstance()->Get(textureId);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", textureId);
		return;
	}

	for (TiXmlElement* currentElement = root->FirstChildElement()
		; currentElement != nullptr
		; currentElement = currentElement->NextSiblingElement())
	{
		int ID = atoi(currentElement->Attribute("n"));
		int l = atoi(currentElement->Attribute("x"));
		int t = atoi(currentElement->Attribute("y"));
		int width = atoi(currentElement->Attribute("w"));
		int height = atoi(currentElement->Attribute("h"));

		// Sprites with odd width/height don't need adjustments
		int r = l + width;
		int b = t + height;

		// Sprites with even width/height need to subtract right/bottom by 1
		if (width % 2 == 0) r = l + width - 1;
		if (height % 2 == 0) b = t + height - 1;

		CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
	}

	DebugOut(L"[INFO] Done loading sprites from : %s \n", path.c_str());
}

void COverworldScene::_ParseSection_ANIMATION(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	DebugOut(L"[INFO] Start loading animations from : %s \n", path.c_str());

	ifstream f;
	f.open(path.c_str());

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#' || line[0] == '\0') continue;	// skip comment lines and empty lines

		vector<string> tokens = split(line);

		if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

		//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

		LPANIMATION ani = new CAnimation();

		int ani_id = atoi(tokens[0].c_str());
		for (unsigned int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
		{
			int sprite_id = atoi(tokens[i].c_str());
			int frame_time = atoi(tokens[i + 1].c_str());
			ani->Add(sprite_id, frame_time);
		}

		CAnimations::GetInstance()->Add(ani_id, ani);
	}

	f.close();

	DebugOut(L"[INFO] Done loading animations from %s\n", path.c_str());
}

void COverworldScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadMap(path.c_str());
}

void COverworldScene::LoadMap(LPCWSTR mapFile)
{
	//Convert mapFile (type - wchar*) to char* because TinyXml doesn't support wchar
	wstring wideStringMapFile(mapFile);
	string stringMapFile(wideStringMapFile.begin(), wideStringMapFile.end());
	const char* charMapFile = stringMapFile.c_str();

	DebugOut(L"[INFO] Start loading map from : %s \n", wideStringMapFile.c_str());

	TiXmlDocument doc(charMapFile);
	bool result = doc.LoadFile();

	if (!result)
	{
		DebugOut(L"[ERROR] Failed to load map from %s\n", wideStringMapFile.c_str());
		return;
	}

	TiXmlElement* root = doc.FirstChildElement();
	int mapId = -999;
	int width = atoi(root->Attribute("width"));
	int height = atoi(root->Attribute("height"));
	int tileWidth = atoi(root->Attribute("tilewidth"));
	int tileHeight = atoi(root->Attribute("tileheight"));

	map = new CTileMap(mapId, mapFile, width, height, tileWidth, tileHeight);

	for (TiXmlElement* currentElement = root->FirstChildElement()
		; currentElement != nullptr
		; currentElement = currentElement->NextSiblingElement())
	{
		if (currentElement->Value() == string("properties") &&
			currentElement->FirstChildElement()->Attribute("name") == string("id"))
		{
			mapId = atoi(currentElement->FirstChildElement()->Attribute("value"));
			if (mapId == -999) DebugOut(L"[ERROR] Map id not found: %i\n", mapId);

			continue;
		}

		if (currentElement->Value() == string("tileset"))
		{
			_ParseSection_TILESET(currentElement);
			continue;
		}

		if (currentElement->Value() == string("layer"))
		{
			_ParseSection_TILELAYER(currentElement);
			continue;
		}

		if (currentElement->Value() == string("objectgroup"))
		{
			_ParseSection_OBJECTGROUP(currentElement);
			continue;
		}
	}

	DebugOut(L"[INFO] Done loading map from %s\n", mapFile);
}

void COverworldScene::_ParseSection_TILESET(TiXmlElement* xmlElementTileSet)
{
	//Parse tileset's general attributes
	int firstGid = atoi(xmlElementTileSet->Attribute("firstgid"));
	int tileWidth = atoi(xmlElementTileSet->Attribute("tilewidth"));
	int tileHeight = atoi(xmlElementTileSet->Attribute("tileheight"));
	int tileCount = atoi(xmlElementTileSet->Attribute("tilecount"));
	int columnsCount = atoi(xmlElementTileSet->Attribute("columns"));
	LPCWSTR imageSourcePath;
	int textureId = -999;

	if (tileWidth != TILE_WIDTH_STANDARD || tileHeight != TILE_HEIGHT_STANDARD)
	{
		DebugOut(L"[ERROR] Tileset's tilewidth (%d) and tileheight (%d) not compatible"
			" with game's standard tilewidth (%d) and tileheight (%d)\n",
			tileWidth,
			tileHeight,
			TILE_WIDTH_STANDARD,
			TILE_HEIGHT_STANDARD);
		return;
	}

	TiXmlElement* xmlImage = xmlElementTileSet->FirstChildElement("image");
	imageSourcePath = ToLPCWSTR(xmlImage->Attribute("source"));

	//Parse tileset's id and textureId
	TiXmlElement* xmlProperties = xmlElementTileSet->FirstChildElement("properties");

	for (TiXmlElement* currentElement = xmlProperties->FirstChildElement()
		; currentElement != nullptr
		; currentElement = currentElement->NextSiblingElement())
	{
		if (currentElement->Attribute("name") == string("textureId"))
		{
			textureId = atoi(currentElement->Attribute("value"));
			if (textureId == -999)
				DebugOut(L"[ERROR] Failed to parse texture id for tileset: %i\n", textureId);

			continue;
		}
	}

	CTextures::GetInstance()->Add(textureId, imageSourcePath);
	LPTEXTURE tileSetTexture = CTextures::GetInstance()->Get(textureId);

	LPTILESET tileSet = new CTileSet(
		firstGid, tileWidth, tileHeight,
		tileCount, columnsCount, tileSetTexture);

	map->Add(tileSet);

	DebugOut(L"[INFO] Done loading tileset from: %s \n", imageSourcePath);
}

void COverworldScene::_ParseSection_TILELAYER(TiXmlElement* xmlElementTileLayer)
{
	if (xmlElementTileLayer->Attribute("visible") != NULL)
	{
		int visible = atoi(xmlElementTileLayer->Attribute("visible"));
		if (!visible) return;
	};

	//Parse tilelayer's general attributes
	int id = atoi(xmlElementTileLayer->Attribute("id"));
	int width = atoi(xmlElementTileLayer->Attribute("width"));
	int height = atoi(xmlElementTileLayer->Attribute("height"));

	LPTILELAYER tileLayer = new CTileLayer(id, width, height);

	//Parse tilelayer's tile matrix
	vector<string> tokens;
	TiXmlElement* xmlElementTileCoorData = xmlElementTileLayer->FirstChildElement("data");
	tokens = split(xmlElementTileCoorData->GetText(), ",");

	int** tileMatrix;
	tileLayer->GetTileMatrix(tileMatrix);

	int tokenIndex = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			tileMatrix[i][j] = atoi(tokens[tokenIndex].c_str());
			tokenIndex++;

			//DebugOut(L"i = %i, j = %i, tokenIndex = %i, tileMatrix[i][j] = %i\n"
			//	, i, j,tokenIndex, tileMatrix[i][j]);
		}
	}

	map->Add(tileLayer);

	DebugOut(L"[INFO] Done loading tilelayer with id: %i \n", id);
}

void COverworldScene::_ParseSection_OBJECTGROUP(TiXmlElement* xmlElementObjectGroup)
{
	CGameObject* obj = NULL;
	int objectType = -999;
	float x = -999;
	float y = -999;

	//Parse object's type id
	TiXmlElement* xmlProperties = xmlElementObjectGroup->FirstChildElement("properties");
	if (xmlProperties == nullptr)
	{
		DebugOut(L"[ERROR] This object group doesn't have object type: \n");
		return;
	}
	for (TiXmlElement* currentElement = xmlProperties->FirstChildElement()
		; currentElement != nullptr
		; currentElement = currentElement->NextSiblingElement())
	{
		if (currentElement->Attribute("name") == string("objectTypeId"))
		{
			objectType = atoi(currentElement->Attribute("value"));
			if (objectType == -999)
			{
				DebugOut(L"[ERROR] Failed to parse object's type id: %i\n",
					objectType);
				return;
			}
		}
	}

	//Parse all objects of a objectgroup
	for (TiXmlElement* currentElementObject = xmlElementObjectGroup->FirstChildElement("object")
		; currentElementObject != nullptr
		; currentElementObject = currentElementObject->NextSiblingElement())
	{
		x = (float)atof(currentElementObject->Attribute("x"));
		y = (float)atof(currentElementObject->Attribute("y"));

		switch (objectType)
		{
		case OBJECT_TYPE_OVERWORLD_MARIO:
		{
			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}
			obj = new COverworldMario(x, y);
			player = (COverworldMario*)obj;

			DebugOut(L"[INFO] Player object has been created!\n");
			break;
		}

		case OBJECT_TYPE_OVERWORLD_NODE:
		{
			int id = atoi(currentElementObject->Attribute("id"));
			int connectedNodeLeft = -1;
			int connectedNodeTop = -1;
			int connectedNodeRight = -1;
			int connectedNodeBottom = -1;
			int playSceneId = -1;
			bool isStartingNode = false;
			int objectSubTypeId = -999;

			if (currentElementObject->FirstChildElement("properties") == NULL)
			{
				DebugOut(L"[ERROR] This overworld node doesn't have objectSubTypeId or connected nodes id");
				break;
			}

			TiXmlElement* xmlNodeProperties =
				currentElementObject->FirstChildElement("properties");

			for (TiXmlElement* currentNodeProperty = xmlNodeProperties->FirstChildElement()
				; currentNodeProperty != nullptr
				; currentNodeProperty = currentNodeProperty->NextSiblingElement())
			{
				if (currentNodeProperty->Attribute("name") == string("connectedNodeLeft"))
				{
					connectedNodeLeft = atoi(currentNodeProperty->Attribute("value"));
				}
				if (currentNodeProperty->Attribute("name") == string("connectedNodeTop"))
				{
					connectedNodeTop = atoi(currentNodeProperty->Attribute("value"));
				}
				if (currentNodeProperty->Attribute("name") == string("connectedNodeRight"))
				{
					connectedNodeRight = atoi(currentNodeProperty->Attribute("value"));
				}
				if (currentNodeProperty->Attribute("name") == string("connectedNodeBottom"))
				{
					connectedNodeBottom = atoi(currentNodeProperty->Attribute("value"));
				}
				if (currentNodeProperty->Attribute("name") == string("isStartingNode"))
				{
					isStartingNode = atoi(currentNodeProperty->Attribute("value"));
				}

				if (currentNodeProperty->Attribute("name") == string("playSceneId"))
				{
					playSceneId = atoi(currentNodeProperty->Attribute("value"));
				}

				if (currentNodeProperty->Attribute("name") == string("objectSubTypeId"))
				{
					objectSubTypeId = atoi(currentNodeProperty->Attribute("value"));
					if (objectType == -999)
					{
						DebugOut(L"[ERROR] Failed to parse block's sub type id: %i\n",
							objectType);
						return;
					}
				}

				obj = new COverworldNode(
					id, objectSubTypeId, x, y,
					connectedNodeLeft, connectedNodeTop, connectedNodeRight, connectedNodeBottom,
					playSceneId);

				this->nodes[id] = (COverworldNode*)obj;

				if (isStartingNode && current_node == -1)
					current_node = id;
			}
			break;
		}

		default:
		{
			DebugOut(L"[ERROR] Object type id does not exist: %i\n", objectType);
			return;
		}
		}

		objects.push_back(obj);
	}
}
