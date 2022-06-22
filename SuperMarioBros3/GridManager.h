#pragma once

#include <vector>
#include "GameObject.h"
#include "debug.h"

#define GRID_SIZE_STANDARD	64

#define SMOOTH_GRID_CHANGE_OFFSET	50

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

	void Clear()
	{
		vector<LPGAMEOBJECT>::iterator it;
		for (it = objects.begin(); it != objects.end(); it++)
		{
			LPGAMEOBJECT o = *it;
			delete o;
			*it = NULL;
		}
		objects.clear();
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

	void PutObjectInGrid(LPGAMEOBJECT obj, int old_grid_row_index = -1, int old_grid_col_index = -1);

	void RemoveObjectFromGrid(LPGAMEOBJECT obj, int grid_row_index, int grid_col_index);

	vector<LPGAMEOBJECT>::iterator GetGridObjectsBegin(int grid_row_index, int grid_col_index);


	vector<LPGAMEOBJECT>::iterator GetGridObjectsEnd(int grid_row_index, int grid_col_index);

	bool IsGridObjectsEmpty(int grid_row_index, int grid_col_index);


	void PurgeGridDeletedObjects(int grid_row_index, int grid_col_index);

	void Clear();

};

