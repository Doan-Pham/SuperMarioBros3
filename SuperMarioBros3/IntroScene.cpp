#include <iostream>
#include <fstream>

#include "IntroScene.h"
#include "IntroKeyEventHandler.h"

#include "PlatformTile.h"
#include "Curtain.h"

#include "AssetIDs.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"

#include "Utils.h"
#include "debug.h"
#include <codecvt>

#define MAX_SCENE_LINE 1024

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_SETTINGS 1
#define SCENE_SECTION_SPRITE 2
#define SCENE_SECTION_ANIMATION	3
#define SCENE_SECTION_MAP 4

#define ID_TILE_LAYER_BACKGROUND_2 2
#define ID_TILE_LAYER_BACKGROUND_3 3

CIntroScene::CIntroScene(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	map = NULL;
	mario_1 = NULL;
	mario_2 = NULL;
	key_handler = new CIntroKeyEventHandler(this);

	title = NULL;
	leaf = NULL;
	mushroom = NULL;
	koopa_1 = NULL;
	koopa_2 = NULL;
	koopa_3 = NULL;
	koopa_4 = NULL;
	koopa_5 = NULL;

	goomba = NULL;
	tree = NULL;
	arrow = NULL;

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
	mario_2_actions_time.push_back(MARIO_2_ACTION_4_TIME);
	mario_2_actions_time.push_back(MARIO_2_ACTION_5_TIME);
	mario_2_actions_time.push_back(MARIO_2_ACTION_6_TIME);
	mario_2_actions_time.push_back(MARIO_2_ACTION_7_TIME);
	mario_2_actions_time.push_back(MARIO_2_ACTION_8_TIME);
	mario_2_actions_time.push_back(MARIO_2_ACTION_9_TIME);
	mario_2_actions_time.push_back(MARIO_2_ACTION_10_TIME);
	mario_2_actions_time.push_back(MARIO_2_ACTION_11_TIME);
	mario_2_actions_time.push_back(MARIO_2_ACTION_12_TIME);
	mario_2_actions_time.push_back(MARIO_2_ACTION_13_TIME);
	mario_2_actions_time.push_back(MARIO_2_ACTION_14_TIME);
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
	string stringMapFile = wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(wideStringMapFile);
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
	bool isVisible = true;
	if (xmlElementTileLayer->Attribute("visible") != NULL)
	{
		isVisible = atoi(xmlElementTileLayer->Attribute("visible"));
	};

	//Parse tilelayer's general attributes
	int id = -999;
	int width = atoi(xmlElementTileLayer->Attribute("width"));
	int height = atoi(xmlElementTileLayer->Attribute("height"));

	//Parse tile layer id
	TiXmlElement* xmlProperties = xmlElementTileLayer->FirstChildElement("properties");
	if (xmlProperties == nullptr)
	{
		DebugOut(L"[ERROR] This tile layer doesn't have id: \n");
		return;
	}
	for (TiXmlElement* currentElement = xmlProperties->FirstChildElement()
		; currentElement != nullptr
		; currentElement = currentElement->NextSiblingElement())
	{
		if (currentElement->Attribute("name") == string("id"))
		{
			id = atoi(currentElement->Attribute("value"));
			if (id == -999)
			{
				DebugOut(L"[ERROR] Failed to parse tile layer id: %i\n",
					id);
				return;
			}
		}
	}

	LPTILELAYER tileLayer = new CTileLayer(id, width, height, isVisible);

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

		case OBJECT_TYPE_INTRO_TREE:
		{
			obj = new CTree(x, y);
			obj->Hide();
			tree = (CTree*)obj;
			break;
		}

		case OBJECT_TYPE_INTRO_ARROW:
		{
			obj = new CIntroArrow(x, y);
			obj->Hide();
			arrow = (CIntroArrow*)obj;
			break;
		}

		case OBJECT_TYPE_ENEMY_GOOMBA_BROWN_NORMAL:
		{
			obj = new CGoomba(x, y);
			obj->SetSpeed(0.0f, 0.0f);
			obj->Hide();
			goomba = (CGoomba*)obj;
			break;
		}

		case OBJECT_TYPE_ITEM_MUSHROOM_BIG:
		{
			obj = new CMushroomBig(x, y);
			obj->SetState(MUSHROOM_STATE_HIDING);
			mushroom = (CMushroomBig*)obj;
			break;
		}

		case OBJECT_TYPE_ITEM_LEAF:
		{
			obj = new CLeaf(x, y);
			obj->SetState(LEAF_STATE_HIDING);
			leaf = (CLeaf*)obj;
			break;
		}


		case OBJECT_TYPE_ENEMY_KOOPA_GREEN_NORMAL:
		{
			obj = new CKoopaGreenNormal(x, y, NULL);
			obj->SetState(KOOPA_STATE_SHELL_STILL_DOWNSIDE);
			obj->Hide();
			CKoopaGreenNormal* koopa = (CKoopaGreenNormal*)obj;
			koopa->SetDirection(1);

			if (koopa_1 == NULL) koopa_1 = (CKoopaGreenNormal*)obj;
			else if (koopa_2 == NULL) koopa_2 = (CKoopaGreenNormal*)obj;
			else if (koopa_3 == NULL) koopa_3 = (CKoopaGreenNormal*)obj;
			else if (koopa_4 == NULL) koopa_4 = (CKoopaGreenNormal*)obj;
			else if (koopa_5 == NULL) koopa_5 = (CKoopaGreenNormal*)obj;

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

void CIntroScene::ProcessActions()
{
	ULONGLONG now = GetTickCount64();
	if (mario_1_current_action == -1)
	{
		mario_1_current_action = 0;
		mario_1_last_action_time = now;
	}
	else
	{
		ULONGLONG t = mario_1_actions_time[mario_1_current_action];
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
		ULONGLONG t = mario_2_actions_time[mario_2_current_action];
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

	case 4:
	{
		map->GetTileLayer(ID_TILE_LAYER_BACKGROUND_2)->UnHide();
		title->SetFlashing();

		mushroom->SetState(MUSHROOM_STATE_MOVING);
		leaf->SetState(LEAF_STATE_MOVING);
		goomba->UnHide();

		koopa_1->UnHide();
		koopa_2->UnHide();

		break;
	}
	case 5:
	{
		koopa_1->SetState(KOOPA_STATE_SHELL_STILL_DOWNSIDE);
		koopa_2->SetState(KOOPA_STATE_SHELL_STILL_DOWNSIDE);
		break;
	}
	case 6:
	{
		mario_2->SetState(MARIO_STATE_WALKING_LEFT);
		mario_2->SetState(MARIO_STATE_JUMP);
		koopa_1->SetState(KOOPA_STATE_SHELL_STILL_DOWNSIDE);
		koopa_2->SetState(KOOPA_STATE_SHELL_STILL_DOWNSIDE);
		break;
	}
	case 7:
	{
		mario_2->SetState(MARIO_STATE_IDLE);
		break;
	}
	case 8:
	{
		mario_2->SetState(MARIO_STATE_WALKING_RIGHT);
		break;
	}
	case 9:
	{
		mario_2->SetState(MARIO_STATE_WALKING_LEFT);
		break;
	}
	case 10:
	{
		mario_2->SetState(MARIO_STATE_IDLE);
		tree->UnHide();
		break;
	}
	case 11:
	{
		mario_2->SetState(MARIO_STATE_WALKING_RIGHT);
		break;
	}
	case 12:
	{
		mario_2->SetState(MARIO_STATE_IDLE);
		map->GetTileLayer(ID_TILE_LAYER_BACKGROUND_3)->UnHide();
		arrow->UnHide();
		koopa_3->UnHide();
		break;
	}
	case 13:
	{
		koopa_4->UnHide();
		break;
	}
	case 14:
	{
		koopa_5->UnHide();
		break;
	}
	default:
		break;
	}

	DebugOutTitle(L"now - sequence_start: %d", now - mario_2_last_action_time);
	//DebugOutTitle(L"mario_2_cur_action %i, mario_2_state: %i", mario_2_current_action, mario_2->GetState());
}

void CIntroScene::Update(DWORD dt)
{
	if (mario_2->IsTransforming()) mario_2->StopTransforming();

	ProcessActions();
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
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

	PurgeDeletedObjects();
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
	// Wihtout this, the items and enemies mario kills in intro scene still give him scores, ... etc
	CGame::GetInstance()->ResetAchievements();
	CSprites::GetInstance()->Clear();
	CAnimations::GetInstance()->Clear();
	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CIntroScene::IsGameObjectDeleted(const LPGAMEOBJECT& o)
{
	return o == NULL;
}

void CIntroScene::PurgeDeletedObjects()
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

	// Swap all the deleted objects to the end then erase them automatically
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), IsGameObjectDeleted),
		objects.end());
}
