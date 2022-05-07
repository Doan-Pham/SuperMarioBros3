#include <iostream>
#include <fstream>
#include "AssetIDs.h"
#include "TinyXml/tinyxml.h"

#include "Map.h"

#include "TileLayer.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"

#include "PlatformTile.h"
#include "PlatformOneLayer.h"
#include "PlatformGhost.h"

#include "SampleKeyEventHandler.h"

using namespace std;

// Definition of static member
vector<LPGAMEOBJECT> CPlayScene::objects;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
	map = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_MAP 3
#define SCENE_SECTION_ASSETSXML 4

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAP_SECTION_UNKNOWN "unknown"
#define MAP_SECTION_TILESET "tileset"
#define MAP_SECTION_TILELAYER "tilelayer"
#define MAP_SECTION_OBJECTLAYER "objectgroup"

#define MAX_SCENE_LINE 1024

//The coordinates parsed from the Tiled software need to be adjusted to be in sync with Mario
//because Tiled uses the top-left corner convention, while this program uses center-center 
#define COORDINATE_ADJUST_SYNC_TILED 8

void CPlayScene::Load()
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
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; };
		if (line == "[ASSETSXMLTEST]") { section = SCENE_SECTION_ASSETSXML; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_ASSETSXML: _ParseSection_ASSETSXML(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ASSETSXML(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	TiXmlDocument doc(tokens[0].c_str());
	bool result = doc.LoadFile();

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
		int r = atoi(currentElement->Attribute("w")) + l -1;
		int b = atoi(currentElement->Attribute("h")) + t - 1;

		CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
	}

}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [MAP]
*/
void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadMap(path.c_str());
}

void CPlayScene::LoadMap(LPCWSTR mapFile)
{
	DebugOut(L"[INFO] Start loading map from : %s \n", mapFile);

	//Convert mapFile (type - wchar*) to char* because TinyXml doesn't support wchar
	wstring wideStringMapFile(mapFile);
	string stringMapFile(wideStringMapFile.begin(), wideStringMapFile.end());
	const char* charMapFile = stringMapFile.c_str();

	TiXmlDocument doc(charMapFile);
	bool result = doc.LoadFile();

	if (!result)
	{
		DebugOut(L"[ERROR] Failed to load map from %s\n", mapFile);
		return;
	}

	TiXmlElement* root = doc.FirstChildElement();
	int mapId = -999;
	int width = atoi(root->Attribute("width"));
	int height = atoi(root->Attribute("height"));
	int tileWidth = atoi(root->Attribute("tilewidth"));
	int tileHeight = atoi(root->Attribute("tileheight"));


	for (TiXmlElement* currentElement = root->FirstChildElement()
		; currentElement != nullptr
		; currentElement = currentElement->NextSiblingElement())
	{
		if (currentElement->Value() == string("properties") &&
			currentElement->FirstChildElement()->Attribute("name") == string("id"))
		{
			mapId = atoi(currentElement->FirstChildElement()->Attribute("value"));
			if (mapId == -999) DebugOut(L"[ERROR] Map id not found: %i\n", mapId);

			map = new CMap(mapId, mapFile, width, height, tileWidth, tileHeight);
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

void CPlayScene::_ParseSection_TILESET(TiXmlElement* xmlElementTileSet)
{
	//Parse tileset's general attributes
	int tilesetId = -999;
	int firstGid = -999;
	int tileWidth = -999;
	int tileHeight = -999;
	int tileCount = -999;
	int columnsCount = -999;
	LPCWSTR imageSourcePath;
	int textureId = -999;

	xmlElementTileSet->Attribute("firstgid", &firstGid);
	xmlElementTileSet->Attribute("tilewidth", &tileWidth);
	xmlElementTileSet->Attribute("tileheight", &tileHeight);
	xmlElementTileSet->Attribute("tilecount", &tileCount);
	xmlElementTileSet->Attribute("columns", &columnsCount);

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
		if (currentElement->Attribute("name") == string("id"))
		{
			tilesetId = atoi(currentElement->Attribute("value"));
			if (tilesetId == -999) 
				DebugOut(L"[ERROR] Failed to parse tileset id: %i\n", tilesetId);

			continue;
		}

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

void CPlayScene::_ParseSection_TILELAYER(TiXmlElement* xmlElementTileLayer)
{
	
	if (xmlElementTileLayer->Attribute("visible") != NULL)
	{
		int visible = atoi(xmlElementTileLayer->Attribute("visible"));
		if (!visible) return;
	};

	//Parse tilelayer's general attributes
	int id = -999;
	int width = -999;
	int height = -999;

	xmlElementTileLayer->Attribute("id", &id);
	xmlElementTileLayer->Attribute("width", &width);
	xmlElementTileLayer->Attribute("height", &height);

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

void CPlayScene::_ParseSection_OBJECTGROUP(TiXmlElement* xmlElementObjectGroup)
{
	CGameObject* obj = NULL;
	int objectType = -999;
	float x = -999;
	float y = -999;

	//Parse object's type id
	TiXmlElement* xmlProperties = xmlElementObjectGroup->FirstChildElement("properties");

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
		x = atof(currentElementObject->Attribute("x"));
		y = atof(currentElementObject->Attribute("y"));

		switch (objectType)
		{
		case OBJECT_TYPE_MARIO:
		{

			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}
			obj = new CMario(x, y,this);
			player = (CMario*)obj;

			DebugOut(L"[INFO] Player object has been created!\n");
			break;
		}

		case OBJECT_TYPE_BLOCK:
		{
			bool isHidingItem = currentElementObject->FirstChildElement("properties") != NULL;
			CBrickQuestionMark* newBrick = new CBrickQuestionMark(x, y, isHidingItem);


			obj = newBrick;
			break;
		}

		case OBJECT_TYPE_ITEM:
		{
			int objectSubTypeId = atoi(currentElementObject->FirstChildElement("properties")
				->FirstChildElement("property")->Attribute("value"));
			switch (objectSubTypeId)
			{
			case OBJECT_TYPE_ITEM_MUSHROOM_BIG:
			{
				obj = new CMushroomBig(x, y);
				break;
			}
			case OBJECT_TYPE_ITEM_LEAF:
			{
				obj = new CLeaf(x, y);
				break;
			}
			case OBJECT_TYPE_ITEM_COIN:
			{
				obj = new CCoin(x, y);
				break;
			}
			default:
				DebugOut(L"[ERROR] Object sub type id does not exist: %i\n", objectSubTypeId);
				return;
				break;
			}
			break;
		}

		case OBJECT_TYPE_PLATFORM:
		{
			if (currentElementObject->FirstChildElement("properties") == NULL)
			{
				DebugOut(L"[ERROR] This object doesn't have subtype id \n");
				return;
			}

			int objectSubTypeId = atoi(currentElementObject->FirstChildElement("properties")
				->FirstChildElement("property")->Attribute("value"));
			switch (objectSubTypeId)
			{
			case OBJECT_TYPE_PLATFORM_TILE:
			{
				int height = atof(currentElementObject->Attribute("height"));
				int width = atof(currentElementObject->Attribute("width"));

				//The Tiled software's coordinate system uses the top-left corner convention, but
				//our program uses the center-center one, therefore we need to adjust the input
				//coordinates
				obj = new CPlatformTile(
					x + width / 2 - COORDINATE_ADJUST_SYNC_TILED,
					y + height / 2 - COORDINATE_ADJUST_SYNC_TILED,
					height,
					width);

				break;
			}
			case OBJECT_TYPE_PLATFORM_ONE_LAYER:
			{
				obj = new CCoin(x, y);
				break;
			}
			case OBJECT_TYPE_PLATFORM_GHOST:
			{
				float height = atof(currentElementObject->Attribute("height"));
				float width = atof(currentElementObject->Attribute("width"));

				//The Tiled software's coordinate system uses the top-left corner convention, but
				//our program uses the center-center one, therefore we need to adjust the input
				//coordinates
				obj = new CPlatformGhost(
					x + width / 2 - COORDINATE_ADJUST_SYNC_TILED,
					y + height / 2 - COORDINATE_ADJUST_SYNC_TILED,
					height,
					width);

				break;
			}
			default:
				DebugOut(L"[ERROR] Object sub type id does not exist: %i\n", objectSubTypeId);
				return;
				break;
			}
			break;

		}


		case OBJECT_TYPE_ENEMY: obj = new CGoomba(x, y); break;

		case OBJECT_TYPE_PORTAL:
		{
			float r = x + (float)atof(currentElementObject->Attribute("width"));
			float b = y + (float)atof(currentElementObject->Attribute("height"));
			int scene_id = atoi(currentElementObject->FirstChildElement("properties")
				->FirstChildElement("property")->Attribute("value"));

			obj = new CPortal(x, y, r, b, scene_id);
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


void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (!objects[i]->IsHidden())
			coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;


	CGame* game = CGame::GetInstance();

	int mapWidth, mapHeight, mapTileWidth, mapTileHeight;
	map->GetSize(mapWidth, mapHeight);
	map->GetTileSize(mapTileWidth, mapTileHeight);

	float mapLeftEdge = 0 - COORDINATE_ADJUST_SYNC_TILED;
	float mapTopEdge = 0 - COORDINATE_ADJUST_SYNC_TILED;
	float mapRightEdge = mapWidth * mapTileWidth - COORDINATE_ADJUST_SYNC_TILED;
	float mapBottomEdge = mapHeight * mapTileHeight - COORDINATE_ADJUST_SYNC_TILED;


	float cx, cy;
	player->GetPosition(cx, cy);

	// Adjust mario's position to prevent him from going beyond the map's edges
	// If we don't add/substract COORDINATE_ADJUST_SYNC_TILED and simply use the map's edges,
	// mario will get split in half when he comes to the edges.
	if (cx < mapLeftEdge + COORDINATE_ADJUST_SYNC_TILED) 
		player->SetPosition(mapLeftEdge + COORDINATE_ADJUST_SYNC_TILED, cy);
	if (cx > mapRightEdge - COORDINATE_ADJUST_SYNC_TILED)
		player->SetPosition(mapRightEdge - COORDINATE_ADJUST_SYNC_TILED, cy);

	if (cy < mapTopEdge + COORDINATE_ADJUST_SYNC_TILED) 
		player->SetPosition(cx, mapTopEdge + COORDINATE_ADJUST_SYNC_TILED);
	if (cy > mapBottomEdge - COORDINATE_ADJUST_SYNC_TILED)
		player->SetPosition(cx, mapBottomEdge - COORDINATE_ADJUST_SYNC_TILED);


	// Adjust camera's position so it won't go past the map's edge
	// Update camera to follow mario
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;

	if (cx < mapLeftEdge) cx = mapLeftEdge;
	if (cx > mapRightEdge - game->GetBackBufferWidth()) 
		cx = mapRightEdge - game->GetBackBufferWidth();

	if (cy < mapTopEdge) cy = mapTopEdge;
	if (cy > mapBottomEdge - game->GetBackBufferHeight())
		cy = mapBottomEdge - game->GetBackBufferHeight();

	CGame::GetInstance()->SetCamPos(cx, cy);;

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	if (map != nullptr)
	{
		map->Render();
	}

	// A lambda expression to sort vector objects according to the object's render priority
	// 
	sort(objects.begin(), objects.end(),
		[](const LPGAMEOBJECT& firstObject, const LPGAMEOBJECT& secondObject) -> bool
		{
			return firstObject->GetRenderPriority() > secondObject->GetRenderPriority();
		});

	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
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
	//CTextures::GetInstance()->Clear();
	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}