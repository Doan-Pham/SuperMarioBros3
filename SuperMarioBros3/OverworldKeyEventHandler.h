#pragma once
#include "Scene.h"
class COverworldKeyEventHandler : public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	COverworldKeyEventHandler(LPSCENE s) :CSceneKeyHandler(s) {};
};


