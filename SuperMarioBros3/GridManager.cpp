#include "GridManager.h"

void CGridManager::PutObjectInGrid(
	LPGAMEOBJECT obj,
	int old_first_grid_row_index,
	int old_first_grid_col_index,
	int old_last_grid_row_index,
	int old_last_grid_col_index)
{
	float obj_x, obj_y;
	obj->GetPosition(obj_x, obj_y);

	//int new_first_grid_row_index;
	//int new_last_grid_row_index;

	int new_first_grid_row_index = (int)obj_y / gridSize;
	int new_first_grid_col_index = (int)obj_x / gridSize;
	int new_last_grid_row_index = new_first_grid_row_index;
	int new_last_grid_col_index = new_first_grid_col_index;

	float obj_left = -1.0f, obj_top = -1.0f, obj_right = -1.0f, obj_bottom = -1.0f;
	obj->GetBoundingBox(obj_left, obj_top, obj_right, obj_bottom);

	// If object's GetBoundingBox() is not empty
	if (obj_left != -1.0f)
	{
		new_first_grid_col_index = (int)obj_left / gridSize;
		new_first_grid_row_index = (int)obj_top / gridSize;
		new_last_grid_col_index = (int)obj_right / gridSize;
		new_last_grid_row_index = (int)obj_bottom / gridSize;
	}

	if (new_last_grid_row_index >= gridCountY || new_last_grid_col_index >= gridCountX ||
		new_first_grid_row_index < 0 || new_first_grid_col_index < 0)
	{
		//DebugOut(L"[INFO] Objects are outside the map, can't add objects to grids : row_index : %i, col_index : %i\n",
		//	new_first_grid_row_index, new_first_grid_col_index);
		return;
	}



	// Object is not in any grid yet
	if (old_first_grid_row_index == -1 && old_last_grid_row_index == -1 && 
		old_first_grid_col_index == -1 && old_last_grid_col_index == -1)
	{
		for (int i = new_first_grid_row_index; i <= new_last_grid_row_index; i++)
		{
			for (int j = new_first_grid_col_index; j <= new_last_grid_col_index; j++)
			{
				gridMatrix[i][j].AddObject(obj);
			}
		}
		//DebugOut(L"grid_i : %i, grid_j : %i, grid_objects_count: %i \n",
			//grid_row_index, grid_col_index, gridMatrix[i][j].GetObjectsCount());
	}
	// Object' old grid is different from new grid
	else if (
		new_first_grid_row_index != old_first_grid_row_index || 
		new_first_grid_col_index != old_first_grid_col_index || 
		new_last_grid_row_index != old_last_grid_row_index || 
		new_last_grid_col_index != old_last_grid_col_index){
		for (int i = old_first_grid_row_index; i <= old_last_grid_row_index; i++)
		{
			for (int j = old_first_grid_col_index; j <= old_last_grid_col_index; j++)
			{
				gridMatrix[i][j].RemoveObject(obj);
			}
		}
		for (int i = new_first_grid_row_index; i <= new_last_grid_row_index; i++)
		{
			for (int j = new_first_grid_col_index; j <= new_last_grid_col_index; j++)
			{
				gridMatrix[i][j].AddObject(obj);
			}
		}
	}

	obj->SetCurrentGrid(
		new_first_grid_row_index, new_first_grid_col_index, 
		new_last_grid_row_index, new_last_grid_col_index);
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

bool CGridManager::IsGameObjectDeleted(const LPGAMEOBJECT& o)
{
	return o == NULL;
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