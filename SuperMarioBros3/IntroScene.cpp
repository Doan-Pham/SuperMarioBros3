#include <iostream>
#include <fstream>

#include "IntroScene.h"

#include "PlatformTile.h"
#include "Curtain.h"

#include "Goomba.h"
#include "MushroomBig.h"
#include "Leaf.h"
#include "KoopaGreenNormal.h"

#include "AssetIDs.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"

#include "Utils.h"
#include "debug.h"

#define MAX_SCENE_LINE 1024

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_SETTINGS 1
#define SCENE_SECTION_SPRITE 2
#define SCENE_SECTION_ANIMATION	3
#define SCENE_SECTION_MAP 4

CIntroScene::CIntroScene(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	this->map = NULL;
	mario_1 = NULL;
	mario_2 = NULL;
	title = NULL;

	mario_1_last_action_time = -1;
	mario_1_current_action = -1;
	mario_1_actions_time.push_back(MARIO_1_ACTION_0_TIME);
	mario_1_actions_time.push_back(MARIO_1_ACTION_1_TIME);
	mario_1_actions_time.push_back(MARIO_1_ACTION_2_TIME);
	mario_1_actions_time.push_back(MARIO_1_ACTION_3_TIME);

	mario_2_last_action_time = -1;
	mario_2_current_action = -1;
	mario_2_actions_time.push_back(MARIO_2_ACTION_0_TIME);
	mario_2_actions_time.push_back(MARIO_2_ACTION_1_TIME);
	mario_2_actions_time.push_back(MARIO_2_ACTION_2_TIME);
	mario_2_actions_time.push_back(MARIO_2_ACTION_3_TIME);

}

void CIntroScene::Load()
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

void CIntroScene::_ParseSection_SETTINGS(string line)
{
}

void CIntroScene::_ParseSection_SPRITE(string line)
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

void CIntroScene::_ParseSection_ANIMATION(string line)
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

void CIntroScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadMap(path.c_str());
}

void CIntroScene::LoadMap(LPCWSTR mapFile)
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

void CIntroScene::_ParseSection_TILESET(TiXmlElement* xmlElementTileSet)
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

void CIntroScene::_ParseSection_TILELAYER(TiXmlElement* xmlElementTileLayer)
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

void CIntroScene::_ParseSection_OBJECTGROUP(TiXmlElement* xmlElementObjectGroup)
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
		case OBJECT_TYPE_MARIO:
		{
			obj = new CMario(x, y, NULL);
			if (mario_1 == NULL) mario_1 = (CMario*)obj;
			else
			{
				mario_2 = (CMario*)obj;
				mario_2->SetDirectionX(-1);
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

			default:
				DebugOut(L"[ERROR] Object sub type id does not exist: %i\n", objectSubTypeId);
				return;
				break;
			}
			break;

		}

		case OBJECT_TYPE_INTRO_CURTAIN:
		{
			obj = new CCurtain(x, y, (int)(y / CURTAIN_PART_HEIGHT));
			break;
		}

		case OBJECT_TYPE_INTRO_TITLE:
		{
			obj = new CGameTitle(x, y);
			obj->Hide();
			title = (CGameTitle*)obj;
			break;
		}
		case OBJECT_TYPE_ENEMY_GOOMBA_BROWN_NORMAL:
		{
			obj = new CGoomba(x, y);
			obj->Hide();
			break;
		}

		case OBJECT_TYPE_ITEM_MUSHROOM_BIG:
		{
			obj = new CMushroomBig(x, y);
			obj->SetState(MUSHROOM_STATE_HIDING);
			break;
		}

		case OBJECT_TYPE_ITEM_LEAF:
		{
			obj = new CLeaf(x, y);
			obj->SetState(LEAF_STATE_HIDING);
			break;
		}


		case OBJECT_TYPE_ENEMY_KOOPA_GREEN_NORMAL:
		{
			obj = new CKoopaGreenNormal(x, y, NULL); 
			obj->SetState(KOOPA_STATE_SHELL_STILL_DOWNSIDE);
			obj->Hide();
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

void CIntroScene::ProcessMario()
{
	ULONGLONG now = GetTickCount64();
	if (mario_1_current_action == -1)
	{
		mario_1_current_action = 0;
		mario_1_last_action_time = now;
	}
	else
	{
		DWORD t = mario_1_actions_time[mario_1_current_action];
		if (now - mario_1_last_action_time > t)
		{
			mario_1_current_action++;
			mario_1_last_action_time = now;
			if (mario_1_current_action == mario_1_actions_time.size())
				mario_1_current_action = mario_1_actions_time.size() - 1;
		}

	}

	switch (mario_1_current_action)
	{
	case 0:
	{
		mario_1->SetState(MARIO_STATE_WALKING_RIGHT);
		break;
	}

	case 1:
	{
		mario_1->SetState(MARIO_STATE_JUMP);
		break;
	}

	case 2:
	{
		mario_1->SetState(MARIO_STATE_JUMP);
		break;
	}

	case 3:
	{
		break;
	}
	default:
		break;
	}

	if (mario_2_current_action == -1)
	{
		mario_2_current_action = 0;
		mario_2_last_action_time = now;
	}
	else
	{
		DWORD t = mario_2_actions_time[mario_2_current_action];
		if (now - mario_2_last_action_time > t)
		{
			mario_2_current_action++;
			mario_2_last_action_time = now;
			if (mario_2_current_action == mario_2_actions_time.size())
				mario_2_current_action = mario_2_actions_time.size() - 1;
		}

	}

	switch (mario_2_current_action)
	{
	case 0:
	{
		mario_2->SetState(MARIO_STATE_WALKING_LEFT);
		break;
	}

	case 1:
	{
		mario_2->SetState(MARIO_STATE_SIT);
		mario_2->SetState(MARIO_STATE_IDLE);
		break;
	}

	case 2:
	{
		break;
	}

	case 3:
	{
		mario_2->SetState(MARIO_STATE_SIT_RELEASE);
		title->UnHide();
		break;
	}
	default:
		break;
	}

	//DebugOutTitle(L"now - sequence_start: %d", now - mario_1_last_action_time);
	DebugOutTitle(L"mario_2_cur_action %i, mario_2_state: %i", mario_2_current_action, mario_2->GetState());
}

void CIntroScene::Update(DWORD dt)
{
	ProcessMario();
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
		if (!objects[i]->IsHidden())
		{
			objects[i]->Update(dt, &coObjects);
		}
	}
}

void CIntroScene::Render()
{
	if (map != nullptr) map->Render();

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		if (!objects[i]->IsHidden())
		{
			objects[i]->Render();
		}
	}
}

void CIntroScene::Unload()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	if (map != NULL)
	{
		map->Clear();
		map = NULL;
	}

	CSprites::GetInstance()->Clear();
	CAnimations::GetInstance()->Clear();
	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}
