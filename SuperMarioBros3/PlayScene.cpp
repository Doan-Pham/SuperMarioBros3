#include <iostream>
#include <fstream>

#include "PlayScene.h"

#include "AssetIDs.h"
#include "Textures.h"
#include "Sprites.h"

#include "Map.h"
#include "TileLayer.h"
#include "Utils.h"
#include "SampleKeyEventHandler.h"

#include "Mario.h"

#include "Leaf.h"
#include "MushroomBig.h"
#include "Coin.h"
#include "Card.h"

#include "BrickQuestionMark.h"
#include "BrickGlass.h"

#include "Goomba.h"
#include "GoombaRedWing.h"
#include "PlantRedFire.h"
#include "PlantGreenNormal.h"
#include "PlantGreenFire.h"
#include "KoopaRedNormal.h"
#include "KoopaGreenWing.h"
#include "KoopaGreenNormal.h"

#include "PlatformTile.h"
#include "PlatformOneLayer.h"
#include "PlatformGhost.h"
#include "Pipe.h"
#include "DeadZone.h"

#include "Portal.h"
#include "Text.h"

using namespace std;
#define BOTTOM_HUD_HEIGHT 50

int CPlayScene::current_map;
unordered_map<int, LPMAP> CPlayScene::maps;
bool CPlayScene::isCourseClear;
ULONGLONG  CPlayScene::clear_course_start;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	overworld_scene_id = -1;

	key_handler = new CSampleKeyHandler(this);

	reduce_time_start = -1;

	float back_buffer_width = CGame::GetInstance()->GetBackBufferWidth();
	float back_buffer_height = CGame::GetInstance()->GetBackBufferHeight();

	pMeter = new CPMeter();

	bottomHUD = new CHUD(back_buffer_width / 2, back_buffer_height - BOTTOM_HUD_HEIGHT / 2,
		back_buffer_width, BOTTOM_HUD_HEIGHT);

	bottomHUD->SetPMeter(pMeter);

	isCourseClear = false;
	clear_course_start = -1;
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_SETTINGS 1
#define SCENE_SECTION_SPRITE 2
#define SCENE_SECTION_ANIMATION	3
#define SCENE_SECTION_MAP 4


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
		if (line == "[SETTINGS]") { section = SCENE_SECTION_SETTINGS; continue; }
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

void CPlayScene::SwitchMap()
{
	if (next_map < 0 || next_map == current_map) return;

	DebugOut(L"[INFO] Switching to map %d\n", next_map);
	CMario* mario_cur = (CMario*)maps[current_map]->GetPlayer();
	CMario* mario_next = (CMario*)maps[next_map]->GetPlayer();
	mario_next->SetLevel(mario_cur->GetLevel());
	current_map = next_map;
}

void CPlayScene::InitiateSwitchMap(int map_id)
{
	next_map = map_id;
}

void CPlayScene::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	if (tokens[0] == "start")
	{
		next_map = atoi(tokens[1].c_str());

		current_map = next_map;
	}
	if (tokens[0] == "overworld_scene")
	{
		overworld_scene_id = atoi(tokens[1].c_str());
	}
	else
		DebugOut(L"[ERROR] Unknown game setting: %s\n", ToWSTR(tokens[0]).c_str());
}

void CPlayScene::_ParseSection_SPRITE(string line)
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

void CPlayScene::_ParseSection_ANIMATION(string line)
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
			if (mapId == -999)
			{
				DebugOut(L"[ERROR] Map id not found: %i\n", mapId);
				return;
			}

			if (maps[mapId] != nullptr)
			{
				DebugOut(L"[INFO] Map has already been loaded: %i\n", mapId);
				return;
			}
			maps[mapId] = new CMap(mapId, mapFile, width, height, tileWidth, tileHeight);
			continue;
		}

		if (currentElement->Value() == string("tileset"))
		{
			_ParseSection_TILESET(currentElement, mapId);
			continue;
		}

		if (currentElement->Value() == string("layer"))
		{
			_ParseSection_TILELAYER(currentElement, mapId);
			continue;
		}

		if (currentElement->Value() == string("objectgroup"))
		{
			_ParseSection_OBJECTGROUP(currentElement, mapId);
			continue;
		}
	}

	DebugOut(L"[INFO] Done loading map from %s\n", mapFile);
}

void CPlayScene::_ParseSection_TILESET(TiXmlElement* xmlElementTileSet, int mapId)
{
	if (mapId == -999)
	{
		DebugOut(L"[ERROR] Map id not found: %i\n", mapId);
		return;
	}

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

	maps[mapId]->Add(tileSet);
	DebugOut(L"[INFO] Done loading tileset from: %s \n", imageSourcePath);
}

void CPlayScene::_ParseSection_TILELAYER(TiXmlElement* xmlElementTileLayer, int mapId)
{
	if (mapId == -999)
	{
		DebugOut(L"[ERROR] Map id not found: %i\n", mapId);
		return;
	}

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

	maps[mapId]->Add(tileLayer);
	DebugOut(L"[INFO] Done loading tilelayer with id: %i \n", id);
}

// TODO: Find some way to make this smaller, probably by moving some of the case
// to their separate methods and group them inside another utility class
void CPlayScene::_ParseSection_OBJECTGROUP(TiXmlElement* xmlElementObjectGroup, int mapId)
{
	if (mapId == -999)
	{
		DebugOut(L"[ERROR] Map id not found: %i\n", mapId);
		return;
	}

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
		x = (float)atof(currentElementObject->Attribute("x"));
		y = (float)atof(currentElementObject->Attribute("y"));

		switch (objectType)
		{
		case OBJECT_TYPE_MARIO:
		{
			obj = new CMario(x, y, this);

			if (maps[mapId]->GetPlayer() != NULL)
			{
				DebugOut(L"[INFO] MARIO object was created before for this map: %i!\n", mapId);
				return;
			}

			maps[mapId]->SetPlayer(obj);

			DebugOut(L"[INFO] Player object has been created for map : %i!\n", mapId);
			break;
		}

		case OBJECT_TYPE_BLOCK:
		{

			bool isHidingItem = false;
			bool isHidingUpMushroom = false;
			bool isHidingPBlock = false;
			int objectSubTypeId = -999;
			TiXmlElement* xmlBlockProperties =
				currentElementObject->FirstChildElement("properties");

			for (TiXmlElement* currentBlockProperty = xmlBlockProperties->FirstChildElement()
				; currentBlockProperty != nullptr
				; currentBlockProperty = currentBlockProperty->NextSiblingElement())
			{
				if (currentBlockProperty->Attribute("name") == string("isHidingItem"))
				{
					isHidingItem = atoi(currentBlockProperty->Attribute("value"));
				}
				if (currentBlockProperty->Attribute("name") == string("isHidingUpMushroom"))
				{
					isHidingUpMushroom = atoi(currentBlockProperty->Attribute("value"));
				}
				if (currentBlockProperty->Attribute("name") == string("isHidingPBlock"))
				{
					isHidingPBlock = atoi(currentBlockProperty->Attribute("value"));
				}
				if (currentBlockProperty->Attribute("name") == string("objectSubTypeId"))
				{
					objectSubTypeId = atoi(currentBlockProperty->Attribute("value"));
					if (objectType == -999)
					{
						DebugOut(L"[ERROR] Failed to parse block's sub type id: %i\n",
							objectType);
						return;
					}
				}
			}

			switch (objectSubTypeId)
			{
			case OBJECT_TYPE_BLOCK_BRICK_QUESTIONMARK:
			{
				obj = new CBrickQuestionMark(x, y, isHidingItem);
				break;
			}

			case OBJECT_TYPE_BLOCK_BRICK_GLASS:
			{
				obj = new CBrickGlass(x, y, isHidingUpMushroom, isHidingPBlock);
				break;
			}
			default:
			{
				DebugOut(L"[ERROR] Object sub type id does not exist: %i\n", objectSubTypeId);
				return;
				break;
			}
			}
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

			case OBJECT_TYPE_ITEM_CARD:
			{
				obj = new CCard(x, y);
				maps[mapId]->AddClearCourseCard((CCard*)obj);
				break;
			}

			default:
			{
				DebugOut(L"[ERROR] Object sub type id does not exist: %i\n", objectSubTypeId);
				return;
			}

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
				float height = (float)atof(currentElementObject->Attribute("height"));
				float width = (float)atof(currentElementObject->Attribute("width"));

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
				float height = (float)atof(currentElementObject->Attribute("height"));
				float width = (float)atof(currentElementObject->Attribute("width"));

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

		case OBJECT_TYPE_ENEMY:
		{
			int objectSubTypeId = -999;

			TiXmlElement* xmlElementProperties = currentElementObject->FirstChildElement("properties");

			for (TiXmlElement* currentProprety = xmlElementProperties->FirstChildElement()
				; currentProprety != nullptr
				; currentProprety = currentProprety->NextSiblingElement())
			{
				if (currentProprety->Attribute("name") == string("objectSubTypeId"))
				{
					objectSubTypeId = atoi(currentProprety->Attribute("value"));
					if (objectSubTypeId == -999)
					{
						DebugOut(L"[ERROR] Failed to parse enemy's sub type id: %i\n",
							objectSubTypeId);
						return;
					}
				}
			}

			switch (objectSubTypeId)
			{

			case OBJECT_TYPE_ENEMY_GOOMBA_BROWN_NORMAL:
			{
				obj = new CGoomba(x, y); break;
				break;
			}

			case OBJECT_TYPE_ENEMY_GOOMBA_RED_WING:
			{
				obj = new CGoombaRedWing(x, y); break;
				break;
			}

			case OBJECT_TYPE_ENEMY_KOOPA_RED_NORMAL:
			{
				obj = new CKoopaRedNormal(x, y, this);
				break;
			}

			case OBJECT_TYPE_ENEMY_KOOPA_GREEN_WING:
			{
				obj = new CKoopaGreenWing(x, y, this);
				break;
			}

			case OBJECT_TYPE_ENEMY_KOOPA_GREEN_NORMAL:
			{
				obj = new CKoopaGreenNormal(x, y, this);
				break;
			}

			case OBJECT_TYPE_ENEMY_PLANT_RED_FIRE:
			{
				obj = new CPlantRedFire(x, y); break;
				break;
			}

			case OBJECT_TYPE_ENEMY_PLANT_GREEN_NORMAL:
			{
				obj = new CPlantGreenNormal(x, y); break;
				break;
			}

			case OBJECT_TYPE_ENEMY_PLANT_GREEN_FIRE:
			{
				obj = new CPlantGreenFire(x, y); break;
				break;
			}
			default:
			{
				DebugOut(L"[ERROR] Object sub type id does not exist: %i\n", objectSubTypeId);
				return;
			}
			}

			break;
		}

		case OBJECT_TYPE_PIPE:
		{

			float height = (float)atof(currentElementObject->Attribute("height"));
			float width = (float)atof(currentElementObject->Attribute("width"));

			float cellWidth = -1.0f;
			float cellHeight = -1.0f;

			// -1: vertical upside, 1: vertical downside, 2: horizontal
			int direction = -999;
			bool isMarioSpawnLocation = 0;
			TiXmlElement* xmlElementProperties = currentElementObject->FirstChildElement("properties");

			for (TiXmlElement* currentProprety = xmlElementProperties->FirstChildElement()
				; currentProprety != nullptr
				; currentProprety = currentProprety->NextSiblingElement())
			{
				if (currentProprety->Attribute("name") == string("direction"))
				{
					direction = atoi(currentProprety->Attribute("value"));
					if (direction == -999)
					{
						DebugOut(L"[ERROR] Pipe's direction unknown: %i\n", direction);
						return;
					}
				}

				if (currentProprety->Attribute("name") == string("cellWidth"))
				{
					cellWidth = (float)atof(currentProprety->Attribute("value"));
					if (cellWidth == -1.0f)
					{
						DebugOut(L"[ERROR] Pipe's cell width unknown: %i\n", cellWidth);
						return;
					}
				}

				if (currentProprety->Attribute("name") == string("cellHeight"))
				{
					cellHeight = (float)atof(currentProprety->Attribute("value"));
					if (cellHeight == -1.0f)
					{
						DebugOut(L"[ERROR] Pipe's cell height unknown: %i\n", cellHeight);
						return;
					}
				}

				if (currentProprety->Attribute("name") == string("isMarioSpawnLocation"))
				{
					isMarioSpawnLocation = (float)atof(currentProprety->Attribute("value"));
				}
			}

			obj = new CPipe(
				x + width / 2 - COORDINATE_ADJUST_SYNC_TILED,
				y + cellHeight / 2 - COORDINATE_ADJUST_SYNC_TILED,
				(int)width / cellWidth, (int)height / cellHeight,
				cellWidth, cellHeight,
				direction, isMarioSpawnLocation);
			if (isMarioSpawnLocation)
			{
				CMario* mario = (CMario*)maps[mapId]->GetPlayer();
				mario->SetSpawnPipeLocation((CPipe*)obj);
			}

			break;
		}

		case OBJECT_TYPE_DEAD_ZONE:
		{
			float height = (float)atof(currentElementObject->Attribute("height"));
			float width = (float)atof(currentElementObject->Attribute("width"));

			//The Tiled software's coordinate system uses the top-left corner convention, but
			//our program uses the center-center one, therefore we need to adjust the input
			//coordinates
			obj = new CDeadZone(
				x + width / 2 - COORDINATE_ADJUST_SYNC_TILED,
				y + height / 2 - COORDINATE_ADJUST_SYNC_TILED,
				height,
				width);

			break;
		}

		case OBJECT_TYPE_PORTAL:
		{
			float width = (float)atof(currentElementObject->Attribute("width"));
			float height = (float)atof(currentElementObject->Attribute("height"));
			int map_id = -1;
			int scene_id = -1;

			TiXmlElement* xmlElementProperties = currentElementObject->FirstChildElement("properties");

			for (TiXmlElement* currentProprety = xmlElementProperties->FirstChildElement()
				; currentProprety != nullptr
				; currentProprety = currentProprety->NextSiblingElement())
			{
				if (currentProprety->Attribute("name") == string("mapId"))
				{
					map_id = atoi(currentProprety->Attribute("value"));
				}
				if (currentProprety->Attribute("name") == string("sceneId"))
				{
					scene_id = atoi(currentProprety->Attribute("value"));
				}
			}
			obj = new CPortal(
				x + width / 2 - COORDINATE_ADJUST_SYNC_TILED,
				y + height / 2 - COORDINATE_ADJUST_SYNC_TILED,
				width, height, scene_id, map_id);
			break;
		}

		case OBJECT_TYPE_TEXT:
		{
			string textContent;
			TiXmlElement* xmlElementProperties = currentElementObject->FirstChildElement("properties");

			for (TiXmlElement* currentProprety = xmlElementProperties->FirstChildElement()
				; currentProprety != nullptr
				; currentProprety = currentProprety->NextSiblingElement())
			{
				if (currentProprety->Attribute("name") == string("textContent"))
				{
					textContent = string(currentProprety->Attribute("value"));
				}

			}
			obj = new CText(x, y, vector<char>(textContent.begin(), textContent.end()));

			maps[mapId]->AddText(textContent, (CText*)obj);
			break;
		}

		default:
		{
			DebugOut(L"[ERROR] Object type id does not exist: %i\n", objectType);
			return;
		}
		}

		//objects.push_back(obj);
		maps[mapId]->Add(obj);
	}
}

void CPlayScene::Update(DWORD dt)
{

	if (isCourseClear)
	{
		int timeLeftBefore = CGame::GetInstance()->GetPlaysceneTimeLeft();
		CGame::GetInstance()->UpdatePlaysceneTimeLeft(-TIME_REDUCE_AMOUNT_COURSE_CLEAR);
		int timeLeftAfter = CGame::GetInstance()->GetPlaysceneTimeLeft();

		CGame::GetInstance()->UpdateScores
		((timeLeftBefore - timeLeftAfter) * SCORE_PER_SECOND_AFTER_COURSE_CLEAR);
		if (GetTickCount64() - clear_course_start > CLEAR_COURSE_SWITCH_SCENE_TIMEOUT)
			CGame::GetInstance()->InitiateSwitchScene(overworld_scene_id);
	}
	else
	{
		if (GetTickCount64() - reduce_time_start > REDUCE_TIME_TIMEOUT)
		{
			CGame::GetInstance()->UpdatePlaysceneTimeLeft(-TIME_REDUCE_AMOUNT_DEFAULT);
			reduce_time_start = GetTickCount64();
		}
		if (CGame::GetInstance()->GetPlaysceneTimeLeft() == 0)
		{
			CMario* mario = (CMario*)GetPlayer();
			mario->SetState(MARIO_STATE_DIE);
		}
	}
	SwitchMap();
	maps[current_map]->Update(dt);
	bottomHUD->Update(dt);
}

void CPlayScene::Render()
{
	if (maps[current_map] != nullptr)
		maps[current_map]->Render();
	bottomHUD->Render();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	if (maps[current_map] != NULL)
	{
		maps[current_map]->Clear();
		maps[current_map] = NULL;
	}
	isCourseClear = false;
	CSprites::GetInstance()->Clear();
	CAnimations::GetInstance()->Clear();
	//CTextures::GetInstance()->Clear();
	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }