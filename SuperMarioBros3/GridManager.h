#pragma once

#include <vector>
#include "GameObject.h"
#include "debug.h"
#define GRID_SIZE_STANDARD	32

class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CGrid;
typedef CGrid* LPGRID;

struct CGrid
{
protected:
	vector<LPGAMEOBJECT> objects;

public:
	CGrid() {};
	void AddObject(LPGAMEOBJECT obj) {
		objects.push_back(obj);
	}
	void GetObjects(vector<LPGAMEOBJECT>::iterator& object_begin, vector<LPGAMEOBJECT>::iterator& object_end)
	{
		if (objects.empty())return;

		object_begin = objects.begin();
		object_end = objects.end();
	}
	vector<LPGAMEOBJECT>::iterator GetObjectsBegin()
	{
		return objects.begin();
	}
	vector<LPGAMEOBJECT>::iterator GetObjectsEnd()
	{
		return objects.end();
	}
	int GetObjectsCount() { return objects.size(); }
	void PurgeDeletedObjects()
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

		objects.erase(
			std::remove_if(objects.begin(), objects.end(),
				[](const LPGAMEOBJECT& o)-> bool {return o == NULL; }),
			objects.end());
	}
	void RemoveObject(LPGAMEOBJECT obj)
	{
		objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
	}
	bool IsEmpty() { return objects.empty(); }
};

class CGridManager
{
private:
	CGrid** gridMatrix;
	int gridSize;
	int gridCountX;
	int gridCountY;

public:
	CGridManager(int gridCountX, int gridCountY, int gridSize = GRID_SIZE_STANDARD) 
	{
		this->gridSize = gridSize;
		this->gridCountX = gridCountX;
		this->gridCountY = gridCountY;
		gridMatrix = new CGrid * [gridCountY];
		for (int i = 0; i < gridCountY; i++) gridMatrix[i] = new CGrid[gridCountX];
	}

	void SetGridCountX(int gridCountX) { this->gridCountX = gridCountX; }
	int GetGridCountX() { return gridCountX; }

	void SetGridCountY(int gridCountY) { this->gridCountY = gridCountY; };
	int GetGridCountY() { return gridCountY; }

	int GetGridSize() { return gridSize; }

	void PutObjectInGrid(LPGAMEOBJECT obj, int old_grid_row_index = -1, int old_grid_col_index = -1)
	{
		float obj_x, obj_y;
		obj->GetPosition(obj_x, obj_y);

		int new_grid_row_index = obj_y / gridSize;
		int new_grid_col_index = obj_x / gridSize;
		if (new_grid_row_index >= gridCountY || new_grid_col_index >= gridCountX)
		{
			DebugOut(L"[INFO] Objects are outside the map, can't add objects to grids");
			return;
		}
		// Object is not in any grid yet
		if (old_grid_row_index == -1 && old_grid_col_index == -1)
		{	
			gridMatrix[new_grid_row_index][new_grid_col_index].AddObject(obj);
			//DebugOut(L"grid_i : %i, grid_j : %i, grid_objects_count: %i \n",
				//grid_row_index, grid_col_index, gridMatrix[grid_row_index][grid_col_index].GetObjectsCount());
		}
		// Object' old grid is different from new grid
		else if(new_grid_row_index != old_grid_row_index || new_grid_col_index != old_grid_col_index)
		{
			gridMatrix[old_grid_row_index][old_grid_col_index].RemoveObject(obj);
			gridMatrix[new_grid_row_index][new_grid_col_index].AddObject(obj);
		}
		obj->SetCurrentGrid(new_grid_row_index, new_grid_col_index);
	}
	void RemoveObjectFromGrid(LPGAMEOBJECT obj, int grid_row_index, int grid_col_index)
	{
		gridMatrix[grid_row_index][grid_col_index].RemoveObject(obj);
	}
	vector<LPGAMEOBJECT>::iterator GetGridObjectsBegin(int grid_row_index, int grid_col_index)
	{
		return gridMatrix[grid_row_index][grid_col_index].GetObjectsBegin();
	}

	vector<LPGAMEOBJECT>::iterator GetGridObjectsEnd(int grid_row_index, int grid_col_index)
	{
		return gridMatrix[grid_row_index][grid_col_index].GetObjectsEnd();
	}
	bool IsGridObjectsEmpty(int grid_row_index, int grid_col_index)
	{
		return gridMatrix[grid_row_index][grid_col_index].IsEmpty();
	}

	void PurgeGridDeletedObjects(int grid_row_index, int grid_col_index)
	{
		gridMatrix[grid_row_index][grid_col_index].PurgeDeletedObjects();
	}
	void Clear()
	{
		for (int i = 0; i < gridCountY; i++) {
			delete gridMatrix[i];
		}
		delete gridMatrix;
	}
};

