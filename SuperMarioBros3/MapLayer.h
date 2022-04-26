#pragma once

class CMapLayer
{
protected:
	int id;
public:
	CMapLayer(int id) { this->id = id; };
}; 

typedef CMapLayer* LPMAPLAYER;
