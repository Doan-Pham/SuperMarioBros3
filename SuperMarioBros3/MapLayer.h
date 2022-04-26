#pragma once

class CMapLayer
{
protected:
	int id;
public:
	CMapLayer(int id) { this->id = id; };
	virtual void Render() = 0;
}; 

typedef CMapLayer* LPMAPLAYER;
