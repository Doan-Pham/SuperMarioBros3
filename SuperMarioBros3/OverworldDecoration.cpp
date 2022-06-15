#include "OverworldDecoration.h"
#include "Game.h"
#include "debug.h"
#include "Animations.h"

void COverworldDecoration::Render()
{
	int currentWorld = CGame::GetInstance()->GetCurrentWorld();
	int animationId = -1;
	if (currentWorld >= ID_WORLD_1 && currentWorld <= ID_WORLD_2)
	{
		animationId = ID_ANI_OVERWORLD_DECORATION_WORLD_1 + (currentWorld - ID_WORLD_1);
	}
	else
	{
		DebugOut(L"[ERROR] Can't find world id to render overworld decorations: %i", currentWorld);
		return;
	}
	CAnimations::GetInstance()->Get(animationId)->Render(x,y);
}
