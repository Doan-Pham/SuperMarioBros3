#pragma once

#include <vector>
#include "GameObject.h"

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
	void AddObject(LPGAMEOBJECT obj) { objects.push_back(obj); }
	void GetObjects(vector<LPGAMEOBJECT>::iterator object_begin, vector<LPGAMEOBJECT>::iterator object_end)
	{
		object_begin = objects.begin();
		object_end = objects.end();
	}
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

	void SetGridCountY(int gridCountY) { this->gridCountY = gridCountY; };

	void PutObjectInGrid(LPGAMEOBJECT obj, int grid_row_index = -1, int grid_col_index = -1)
	{
		// Object is not in any grid yet
		if (grid_row_index == -1 && grid_col_index == -1)
		{
			float obj_x, obj_y;
			obj->GetPosition(obj_x, obj_y);

			int grid_row_index = obj_y / gridSize;
			int grid_col_index = obj_x / gridSize;
			
			gridMatrix[grid_row_index][grid_col_index].AddObject(obj);
		}
		else
		{

		}
	}

	void GetGridObjects(int grid_row_index, int grid_col_index, 
		vector<LPGAMEOBJECT>::iterator iterator_begin, vector<LPGAMEOBJECT>::iterator iterator_end)
	{
		gridMatrix[grid_row_index][grid_col_index].GetObjects(iterator_begin, iterator_end);
	}

	void Clear()
	{
		for (int i = 0; i < gridCountY; i++) {
			delete gridMatrix[i];
		}
		delete gridMatrix;
	}
};

