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

using namespace std;

// Have to define this vector since it's static (it has to be static to be used in the static method:
// AddObjects()
vector<LPGAMEOBJECT> CPlayScene::objects;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
	map = NULL;
	key_handler = new CSampleKeyHandler(this);
	isCameraYDefaultValue = true;
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_SPRITE 1
#define SCENE_SECTION_ANIMATION	2
#define SCENE_SECTION_MAP 3


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
		if (line == "[SPRITE]") { section = SCENE_SECTION_SPRITE; continue; }
		if (line == "[ANIMATION]") { section = SCENE_SECTION_ANIMATION; continue; };
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_SPRITE: _ParseSection_SPRITE(line); break;
		case SCENE_SECTION_ANIMATION: _ParseSection_ANIMATION(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
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

void CPlayScene::_ParseSection_TILELAYER(TiXmlElement* xmlElementTileLayer)
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

// TODO: Find some way to make this smaller, probably by moving some of the case
// to their separate methods and group them inside another utility class
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
		x = (float)atof(currentElementObject->Attribute("x"));
		y = (float)atof(currentElementObject->Attribute("y"));

		switch (objectType)
		{
		case OBJECT_TYPE_MARIO:
		{

			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}
			obj = new CMario(x, y, this);
			player = (CMario*)obj;

			DebugOut(L"[INFO] Player object has been created!\n");
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
			int direction = -1;

			TiXmlElement* xmlElementProperties = currentElementObject->FirstChildElement("properties");

			for (TiXmlElement* currentProprety = xmlElementProperties->FirstChildElement()
				; currentProprety != nullptr
				; currentProprety = currentProprety->NextSiblingElement())
			{
				if (currentProprety->Attribute("name") == string("direction"))
				{
					direction = atoi(currentProprety->Attribute("value"));
					if (direction == -1)
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
			}


			obj = new CPipe(
				x + width / 2 - COORDINATE_ADJUST_SYNC_TILED,
				y + cellHeight / 2 - COORDINATE_ADJUST_SYNC_TILED,
				(int)width / cellWidth, (int)height / cellHeight,
				cellWidth, cellHeight,
				direction);

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
	for (size_t i = 1; i < objects.size(); i++)
	{
		if (!objects[i]->IsHidden())
		{
			coObjects.push_back(objects[i]);
		}

	}

	float cam_test_x, cam_test_y;
	CGame::GetInstance()->GetCamPos(cam_test_x, cam_test_y);
	for (size_t i = 0; i < objects.size(); i++)
	{

		// TODO: A very simple implementation to only update objects near camera
		float object_x, object_y;
		objects[i]->GetPosition(object_x, object_y);
		if (object_x >= cam_test_x - 100 && object_x <= cam_test_x + SCREEN_WIDTH + 100 &&
			object_y >= cam_test_y - 100 && object_y <= cam_test_y + SCREEN_HEIGHT + 100)

			objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	CGame* game = CGame::GetInstance();

	int mapWidth, mapHeight, mapTileWidth, mapTileHeight;
	map->GetSize(mapWidth, mapHeight);
	map->GetTileSize(mapTileWidth, mapTileHeight);

	float mapLeftEdge = 0;
	float mapTopEdge = 0;

	// Adjust the right, bottom edges to avoid seeing empty tiles
	float mapRightEdge = (float)(mapWidth * mapTileWidth - COORDINATE_ADJUST_SYNC_TILED);
	float mapBottomEdge = (float)(mapHeight * mapTileHeight - COORDINATE_ADJUST_SYNC_TILED);


	float cam_x, cam_y;
	game->GetCamPos(cam_x, cam_y);

	float player_x, player_y;
	player->GetPosition(player_x, player_y);
	// Adjust mario's position to prevent him from going beyond the map's edges
	// If we don't add/substract COORDINATE_ADJUST_SYNC_TILED and simply use the map's edges,
	// mario will get split in half when he comes to the edges.

	if (player_x < mapLeftEdge + COORDINATE_ADJUST_SYNC_TILED)
		player_x = mapLeftEdge + COORDINATE_ADJUST_SYNC_TILED;

	if (player_x > mapRightEdge - COORDINATE_ADJUST_SYNC_TILED)
		player_x = mapRightEdge - COORDINATE_ADJUST_SYNC_TILED;

	if (player_y < mapTopEdge + COORDINATE_ADJUST_SYNC_TILED)
		player_y = mapTopEdge + COORDINATE_ADJUST_SYNC_TILED;

	if (player_y > mapBottomEdge - COORDINATE_ADJUST_SYNC_TILED)
		player_y = mapBottomEdge - COORDINATE_ADJUST_SYNC_TILED;

	player->SetPosition(player_x, player_y);

	// Adjust camera's position so it won't go past the map's edge
	// Update camera to follow mario
	cam_x = player_x - game->GetBackBufferWidth() / 2;

	// Camera only follows mario if he's flying and he's above a certain point
	CMario* mario = (CMario*)player;

	if (isCameraYDefaultValue)
	{
		if (mario->IsFlying() && player_y < mapBottomEdge - game->GetBackBufferHeight() * 1.425)
		{
			cam_y = player_y - game->GetBackBufferHeight() / 2;
			isCameraYDefaultValue = false;
		}
		else cam_y = mapBottomEdge - game->GetBackBufferHeight() * 1.9;
	}
	else
	{
		if (mario->IsFlying() || mario->IsTrulyFalling() ||
			player_y < mapBottomEdge - game->GetBackBufferHeight() * 1.425)
			cam_y = player_y - game->GetBackBufferHeight() / 2;

		if (cam_y >= mapBottomEdge - game->GetBackBufferHeight() * 1.9)
			isCameraYDefaultValue = true;
	}

	if (cam_x < mapLeftEdge) cam_x = mapLeftEdge;
	if (cam_x > mapRightEdge - game->GetBackBufferWidth())
		cam_x = mapRightEdge - game->GetBackBufferWidth();

	if (cam_y < mapTopEdge) cam_y = mapTopEdge;
	if (cam_y > mapBottomEdge - game->GetBackBufferHeight())
		cam_y = mapBottomEdge - game->GetBackBufferHeight();

	CGame::GetInstance()->SetCamPos(cam_x, cam_y);
	
	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	if (map != nullptr)
	{
		map->Render();
	}

	// A lambda expression to sort vector objects according to the object's render priority
	// Objects with higher priority will be rendered first and can be covered by other objects
	sort(objects.begin(), objects.end(),
		[](const LPGAMEOBJECT& firstObject, const LPGAMEOBJECT& secondObject) -> bool
		{
			return firstObject->GetRenderPriority() > secondObject->GetRenderPriority();
		});

	float cam_test_x, cam_test_y;
	CGame::GetInstance()->GetCamPos(cam_test_x, cam_test_y);
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		// TODO: A very simple implementation to only update objects near camera
		float object_x, object_y;
		objects[i]->GetPosition(object_x, object_y);
		if (object_x >= cam_test_x - 100 && object_x <= cam_test_x + SCREEN_WIDTH + 100 &&
			object_y >= cam_test_y - 100 && object_y <= cam_test_y + SCREEN_HEIGHT + 100)
			objects[i]->Render();
	}
		
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