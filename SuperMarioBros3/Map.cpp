#include "Map.h"
#include "BrickGlass.h"
#include "Coin.h"
#include "Mario.h"
#include "Game.h"

CMap::CMap(int id, LPCWSTR mapFilePath, int width, int height, int 
	tileWidth, int tileHeight, int gridSize)
{
	this->id = id;
	this->mapFilePath = mapFilePath;
	this->width = width;
	this->height = height;
	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;
	gameLoopCount = 0;

	// Adjust the left, top edges to see the cropped tiles
	this->mapLeftEdge = 0 - COORDINATE_ADJUST_SYNC_TILED;
	this->mapTopEdge = 0 - COORDINATE_ADJUST_SYNC_TILED;

	// Adjust the right, bottom edges to avoid seeing empty tiles
	this->mapRightEdge = (float)(width * tileWidth - COORDINATE_ADJUST_SYNC_TILED);
	this->mapBottomEdge = (float)(height * tileHeight - COORDINATE_ADJUST_SYNC_TILED) + BOTTOM_HUD_HEIGHT;

	isPBlockTurnedOn = false;
	isCameraYDefaultValue = true;
	isAddedAcquiredCard = false;

	mapGrid = new CGridManager((mapRightEdge /gridSize) + 1, (mapBottomEdge /gridSize) + 1);
	firstVisibleGridX = -1;
	firstVisibleGridY = -1;
	shownGridsX = -1;
	shownGridsY = -1;

	player = NULL;
	clearCourseCard = NULL;
}

void CMap::Add(LPTILELAYER layer)
{
	for (unsigned int i = 0; i < tileSets.size(); i++)
		layer->AddTileSet(tileSets[i]);
	tileLayers.push_back(layer);
};

void CMap::Update(DWORD dt)
{
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// When mario is transforming, no updates, only render!
	CMario* mario = (CMario*)player;
	if (mario->IsTransforming())
	{
		if (GetTickCount64() - mario->GetTransformStart() > MARIO_TRANSFORM_TIMEOUT)
			mario->StopTransforming();
		else return;
	}


	if (isPBlockTurnedOn)
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (dynamic_cast<CBrickGlass*>(objects[i]))
			{
				objects[i]->SetState(BRICK_STATE_BECOME_COIN);
			}
			if (dynamic_cast<CCoin*>(objects[i]))
			{
				objects[i]->SetState(COIN_STATE_BECOME_BRICK);
			}
		}
		isPBlockTurnedOn = false;
	}

	CGame* game = CGame::GetInstance();
	CPlayScene* currentScene = (CPlayScene*)game->GetCurrentScene();

	float cam_x, cam_y;
	game->GetCamPos(cam_x, cam_y);

	int updateCallsCount = 0;


	firstVisibleGridX = cam_x / mapGrid->GetGridSize();
	firstVisibleGridY = cam_y / mapGrid->GetGridSize();

	shownGridsX = SCREEN_WIDTH / mapGrid->GetGridSize();
	shownGridsY = SCREEN_HEIGHT / mapGrid->GetGridSize();

	vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT>::iterator iterator;

	for (int i = 0; i < mapGrid->GetGridCountY(); i++)
	{
		for (int j = 0; j < mapGrid->GetGridCountX(); j++)
		{
			if (mapGrid->IsGridObjectsEmpty(i, j)) continue;
			for (iterator = mapGrid->GetGridObjectsBegin(i, j);
				iterator != mapGrid->GetGridObjectsEnd(i, j); ++iterator)
			{
				if (!(*iterator)->IsHidden())coObjects.push_back(*iterator);
			}
		}
	}

	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
// TO-DO: This is a "dirty" way, need a more organized way 
	//vector<LPGAMEOBJECT> coObjects;
	//for (size_t i = 1; i < objects.size(); i++)
	//{
	//	if (!objects[i]->IsHidden())
	//	{
	//		coObjects.push_back(objects[i]);
	//	}
	//}
	//for (int i = firstVisibleGridY; i < min(mapGrid->GetGridCountY(), firstVisibleGridY + maxGridY + 1); i++)
	//{
	//	for (int j = firstVisibleGridX; j < min(mapGrid->GetGridCountX(), firstVisibleGridX + maxGridX + 1) ; j++)
	//	{
	//		mapGrid->GetGridObjects(i, j, grid_objects_begin, grid_objects_end);
	//		for (iterator = grid_objects_begin; iterator != grid_objects_end;iterator++)
	//		{
	//			(*iterator)->Update(dt, &coObjects);
	//			updateCallsCount++;
	//		}
	//	}
	//}
	gameLoopCount++;
	for (int i = 0; i < mapGrid->GetGridCountY(); i++)
	{
		for (int j = 0; j < mapGrid->GetGridCountX(); j++)
		{
			if (mapGrid->IsGridObjectsEmpty(i, j)) continue;
			vector<LPGAMEOBJECT>::iterator iterator;
			for (iterator = mapGrid->GetGridObjectsBegin(i, j); 
				iterator != mapGrid->GetGridObjectsEnd(i, j); ++iterator)
			{
				(*iterator)->Update(dt, &coObjects);
				//if (iterator == grid_objects_end)
				//{
				//	DebugOut(L"The break statement was called! \n");
				//	break;
				//}
				updateCallsCount++;
			}
		}
	}

	//for (size_t i = 0; i < objects.size(); i++)
	//{
	//	// TODO: A very simple implementation to only update objects near camera
	//	float object_x, object_y;
	//	objects[i]->GetPosition(object_x, object_y);
	//	if (object_x >= cam_x - CAMERA_SURROUNDING_OFFSET &&
	//		object_x <= cam_x + SCREEN_WIDTH + CAMERA_SURROUNDING_OFFSET &&
	//		object_y >= cam_y - CAMERA_SURROUNDING_OFFSET &&
	//		object_y <= cam_y + SCREEN_HEIGHT + CAMERA_SURROUNDING_OFFSET)

	//	{
	//		objects[i]->Update(dt, &coObjects);
	//		updateCallsCount++;
	//	}	
	//}


	float player_x, player_y;
	player->GetPosition(player_x, player_y);

	// When mario clears map by taking the card, he can goes past the map edges
	if (currentScene->IsCourseClear())
	{
		if (player_x > mapRightEdge)
			texts[TEXT_COURSE_CLEAR]->UnHide();

		if (player_x > mapRightEdge + MARIO_DISTANCE_BEFORE_SHOW_NEXT_TEXT && !isAddedAcquiredCard)
		{
			texts[TEXT_YOU_GOT_A_CARD]->UnHide();

			float text_l, text_t, text_r, text_b;
			texts[TEXT_YOU_GOT_A_CARD]->GetBoundingBox(text_l, text_t, text_r, text_b);

			clearCourseCard->SetState(CARD_STATE_TAKEN);
			clearCourseCard->SetPosition(text_r + 3 * CHARACTER_STANDARD_WIDTH, (text_t + text_b) / 2);

			CGame::GetInstance()->AddCardAcquired(clearCourseCard->GetCurrentType());
			isAddedAcquiredCard = true;
		}

	}
	else
	{
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
	}

	player->SetPosition(player_x, player_y);


	// Update camera to follow mario on x-axis
	cam_x = player_x - game->GetBackBufferWidth() / 2;

	// Camera only follows mario on y-axis if he's flying and he's above a certain point
	if (isCameraYDefaultValue)
	{
		if (mario->IsFlying() && player_y < mapBottomEdge - game->GetBackBufferHeight() / 2)
		{
			cam_y = player_y - game->GetBackBufferHeight() / 2;
			isCameraYDefaultValue = false;
		}
		else cam_y = mapBottomEdge - game->GetBackBufferHeight();
	}
	else
	{
		if (mario->IsFlying() || mario->IsTrulyFalling() ||
			player_y < mapBottomEdge - game->GetBackBufferHeight() /2)
			cam_y = player_y - game->GetBackBufferHeight() / 2;

		if (cam_y >= mapBottomEdge - game->GetBackBufferHeight() )
			isCameraYDefaultValue = true;
	}


	// Adjust camera's position so it won't go past the map's edge
	if (cam_x > mapRightEdge - game->GetBackBufferWidth())
		cam_x = mapRightEdge - game->GetBackBufferWidth();
	if (cam_x < mapLeftEdge) cam_x = mapLeftEdge;

	// Need to adjust bottom edge before top edge because sometimes BackBufferHeight is higher than
	// map's bottom edge (like in map_1_1_bonus) causing error, but topEdge is always
	// guaranteed to be smaller than  BackBufferHeight (because it's usually 0)
	if (cam_y > mapBottomEdge - game->GetBackBufferHeight())
		cam_y = mapBottomEdge - game->GetBackBufferHeight();
	if (cam_y < mapTopEdge) cam_y = mapTopEdge;

	//DebugOutTitle(L"cam_x: %0.2f cam_y : %0.2f, top_edge : %0.2f, bot_edge : %0.2f, back_buff_w : %0.2f, back_buff_h : %0.2f",
	//	cam_x, cam_y, mapTopEdge, mapBottomEdge, game->GetBackBufferWidth(),game->GetBackBufferHeight());
	
	//DebugOutTitle(L"mario_x: %0.2f mario_y : %0.2f, top_edge : %0.2f, bot_edge : %0.2f, back_buff_w : %0.2f, back_buff_h : %0.2f",
	//	player_x, player_y, mapTopEdge, mapBottomEdge, game->GetBackBufferWidth(), game->GetBackBufferHeight());

	DebugOut(L"Game loop count: %i \n", gameLoopCount);
	//DebugOut(L"Update() method calls count: %i \n", updateCallsCount);
	CGame::GetInstance()->SetCamPos(cam_x, cam_y);

	PurgeDeletedObjects();
}

void CMap::Render()
{
	for (unsigned int i = 0; i < tileLayers.size(); i++)
		tileLayers[i]->Render();

	// A lambda expression to sort vector objects according to the object's render priority
	// Objects with higher priority will be rendered first and can be covered by other objects
	//sort(objects.begin(), objects.end(),
	//	[](const LPGAMEOBJECT& firstObject, const LPGAMEOBJECT& secondObject) -> bool
	//	{
	//		return firstObject->GetRenderPriority() > secondObject->GetRenderPriority();
	//	});

	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);

	int renderCallsCount = 0;
	vector<LPGAMEOBJECT>::iterator iterator;
	for (int i = 0; i < mapGrid->GetGridCountY(); i++)
	{
		for (int j = 0; j < mapGrid->GetGridCountX(); j++)
		{
			if (mapGrid->IsGridObjectsEmpty(i, j)) continue;
			for (iterator = mapGrid->GetGridObjectsBegin(i, j);
				iterator != mapGrid->GetGridObjectsEnd(i, j); ++iterator)
			{
				if (!(*iterator)->IsHidden())
				{
					(*iterator)->Render();
					renderCallsCount++;
				}
			}
		}
	}
	//for (unsigned int i = 0; i < objects.size(); i++)
	//{
	//	// TODO: A very simple implementation to only render objects near camera
	//	float object_x, object_y;
	//	objects[i]->GetPosition(object_x, object_y);

	//	if (object_x >= cam_x - CAMERA_SURROUNDING_OFFSET &&
	//		object_x <= cam_x + SCREEN_WIDTH + CAMERA_SURROUNDING_OFFSET &&
	//		object_y >= cam_y - CAMERA_SURROUNDING_OFFSET &&
	//		object_y <= cam_y + SCREEN_HEIGHT + CAMERA_SURROUNDING_OFFSET &&
	//		!objects[i]->IsHidden())
	//	{
	//		objects[i]->Render();
	//		renderCallsCount++;
	//	}	
	//}
	//DebugOut(L"Render() method calls count: %i \n", renderCallsCount);
};

// Clear all map's objects, tilelayers and tilesets
void CMap::Clear()
{
	for (unsigned int i = 0; i < tileLayers.size(); i++)
	{
		tileLayers[i]->Clear();
	}
	tileLayers.clear();

	for (unsigned int i = 0; i < tileSets.size(); i++)
	{
		if (tileSets[i] != nullptr) delete tileSets[i];
	}
	tileSets.clear();

	if (mapFilePath != nullptr)
	{
		mapFilePath = NULL;
	}
	for (unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	mapGrid->Clear();
}

bool CMap::IsGameObjectDeleted(const LPGAMEOBJECT& o)
{
	return o == NULL;
}

void CMap::PurgeDeletedObjects()
{
	for (int i = 0; i < mapGrid->GetGridCountY(); i++)
	{
		for (int j = 0; j < mapGrid->GetGridCountX(); j++)
		{
			if (mapGrid->IsGridObjectsEmpty(i, j)) continue;
			mapGrid->PurgeGridDeletedObjects(i, j);
		}
	}
	//vector<LPGAMEOBJECT>::iterator it;
	//for (it = objects.begin(); it != objects.end(); it++)
	//{
	//	LPGAMEOBJECT o = *it;
	//	if (o->IsDeleted() && o != NULL)
	//	{
	//		delete o;
	//		*it = NULL;
	//	}
	//}

	//// Swap all the deleted objects to the end then erase them automatically
	//objects.erase(
	//	std::remove_if(objects.begin(), objects.end(), IsGameObjectDeleted),
	//	objects.end());
}
