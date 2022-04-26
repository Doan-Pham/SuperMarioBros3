#pragma once
#include <Windows.h>
#include <vector>

#include "MapLayer.h"

class CMap
{
protected:
	int id;
	LPCWSTR mapFilePath;
	std::vector<LPMAPLAYER> layers;

public:
	CMap(int id, LPCWSTR mapFilePath)
	{
		this->id = id;
		this->mapFilePath = mapFilePath;
	}
	void Add(LPMAPLAYER layer) { layers.push_back(layer); };

};

typedef CMap* LPMAP;