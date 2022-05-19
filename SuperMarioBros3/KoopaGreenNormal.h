#include "Koopa.h"

#define ID_ANI_KOOPA_GREEN_NORMAL_WALKING_LEFT					5221
#define ID_ANI_KOOPA_GREEN_NORMAL_WALKING_RIGHT					5222

#define ID_ANI_KOOPA_GREEN_NORMAL_SHELL_MOVING_UPSIDE			5223
#define ID_ANI_KOOPA_GREEN_NORMAL_SHELL_MOVING_DOWNSIDE			5224

#define ID_ANI_KOOPA_GREEN_NORMAL_SHELL_STILL_UPSIDE			5227
#define ID_ANI_KOOPA_GREEN_NORMAL_SHELL_STILL_DOWNSIDE			5228

class CKoopaGreenNormal : public CKoopa
{
protected:

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int GetAniId();
	virtual void Render();

public:
	CKoopaGreenNormal(float x, float y, const LPPLAYSCENE& currentScene) : CKoopa(x, y, currentScene) {};
};
