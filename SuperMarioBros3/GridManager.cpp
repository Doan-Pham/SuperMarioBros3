#include "GridManager.h"

void CGridManager::PutObjectInGrid(LPGAMEOBJECT obj, int old_grid_row_index, int old_grid_col_index)
{
	float obj_x, obj_y;
	obj->GetPosition(obj_x, obj_y);

	int new_grid_row_index = obj_y / gridSize;
	int new_grid_col_index = obj_x / gridSize;
	if (new_grid_row_index >= gridCountY || new_grid_col_index >= gridCountX ||
		new_grid_row_index < 0 || new_grid_col_index < 0)
	{
		DebugOut(L"[INFO] Objects are outside the map, can't add objects to grids : row_index : %i, col_index : %i\n",
			new_grid_row_index, new_grid_col_index);
		return;
	}
	float obj_left = -1.0f, obj_top = -1.0f, obj_right = -1.0f, obj_bottom = -1.0f;
	obj->GetBoundingBox(obj_left, obj_top, obj_right, obj_bottom);

	int grid_left = new_grid_col_index, grid_right = new_grid_col_index;
	int grid_top = new_grid_row_index, grid_bottom = new_grid_row_index;

	// If object's GetBoundingBox() makes some changes to parameter
	if (obj_left != -1.0f)
	{
		grid_left = obj_left / gridSize;
		grid_top = obj_top / gridSize;
		grid_right = obj_right / gridSize;
		grid_bottom = obj_bottom / gridSize;
	}
	float cam_x, cam_y;
 	CGame::GetInstance()->GetCamPos(cam_x, cam_y);
	if (grid_left != new_grid_col_index || grid_right != new_grid_col_index)
	{
		if (obj_left > cam_x && obj_left < cam_x + SCREEN_WIDTH) new_grid_col_index = grid_left;
		else if (obj_right >= cam_x && obj_right <= cam_x + SCREEN_WIDTH) new_grid_col_index = grid_right;
		else if (obj_left > cam_x) new_grid_col_index = grid_left;
		else if (obj_right < cam_x) new_grid_col_index = grid_right;
		if (obj_left - SMOOTH_GRID_CHANGE_OFFSET < cam_x && obj_right + SMOOTH_GRID_CHANGE_OFFSET > cam_x) 
			new_grid_col_index =(cam_x + SCREEN_WIDTH/2) / gridSize;
	}

	if (grid_top != new_grid_row_index || grid_bottom != new_grid_row_index)
	{
		if (obj_top > cam_y && obj_top < cam_y + SCREEN_HEIGHT) new_grid_row_index = grid_top;
		else if (obj_bottom >= cam_y && obj_bottom <= cam_y + SCREEN_HEIGHT) new_grid_row_index = grid_bottom;
		else if (obj_top > cam_y) new_grid_row_index = grid_top;
		else if (obj_bottom < cam_y) new_grid_row_index = grid_bottom;
		if ((obj_top - SMOOTH_GRID_CHANGE_OFFSET*2) < cam_y && (obj_bottom + SMOOTH_GRID_CHANGE_OFFSET*2 )> cam_y) 
			new_grid_row_index = (cam_y + SCREEN_HEIGHT / 2) / gridSize;
	}
	// Object is not in any grid yet
	if (old_grid_row_index == -1 && old_grid_col_index == -1)
	{
		gridMatrix[new_grid_row_index][new_grid_col_index].AddObject(obj);
		//DebugOut(L"grid_i : %i, grid_j : %i, grid_objects_count: %i \n",
			//grid_row_index, grid_col_index, gridMatrix[grid_row_index][grid_col_index].GetObjectsCount());
	}
	// Object' old grid is different from new grid
	else if (new_grid_row_index != old_grid_row_index || new_grid_col_index != old_grid_col_index)
	{
		gridMatrix[old_grid_row_index][old_grid_col_index].RemoveObject(obj);
		gridMatrix[new_grid_row_index][new_grid_col_index].AddObject(obj);
	}
	obj->SetCurrentGrid(new_grid_row_index, new_grid_col_index);
}

void CGridManager::RemoveObjectFromGrid(LPGAMEOBJECT obj, int grid_row_index, int grid_col_index)
{
	gridMatrix[grid_row_index][grid_col_index].RemoveObject(obj);
}

vector<LPGAMEOBJECT>::iterator CGridManager::GetGridObjectsBegin(int grid_row_index, int grid_col_index)
{
	return gridMatrix[grid_row_index][grid_col_index].GetObjectsBegin();
}

vector<LPGAMEOBJECT>::iterator CGridManager::GetGridObjectsEnd(int grid_row_index, int grid_col_index)
{
	return gridMatrix[grid_row_index][grid_col_index].GetObjectsEnd();
}

bool CGridManager::IsGridObjectsEmpty(int grid_row_index, int grid_col_index)
{
	return gridMatrix[grid_row_index][grid_col_index].IsEmpty();
}

void CGridManager::PurgeGridDeletedObjects(int grid_row_index, int grid_col_index)
{
	gridMatrix[grid_row_index][grid_col_index].PurgeDeletedObjects();
}

void CGridManager::Clear()
{
	for (int i = 0; i < gridCountY; i++)
	{
		for (int j = 0; j < gridCountX; j++)
		{
			if (!(gridMatrix[i][j].IsEmpty()))
			gridMatrix[i][j].Clear();
		}
	}
	for (int i = 0; i < gridCountY; i++)
	{
		delete[] gridMatrix[i];
	}
	delete[] gridMatrix;
}