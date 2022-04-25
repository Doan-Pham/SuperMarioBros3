#pragma once

#include <Windows.h>
#include "MapLayer.h"

class CMap
{
protected:
	int id;
	LPCWSTR mapFilePath;
	vector<LPMAPLAYER> layers;

public:
};

typedef CMap* LPMAP;