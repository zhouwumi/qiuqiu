#ifndef BB_OBJECT_MANAGER_CPP
#define BB_OBJECT_MANAGER_CPP
#include "BBObjects.h"
#include "BBPlayerNode.h"
#include "BBPlayer.h"
#include "BBRect.h"
#include "BBVector.h"
#include "BBPoint.h"

class BBGameManager;

class BBObjectManager
{
public:
	BBObjectManager();
	~BBObjectManager();
	void SetGameManager(BBGameManager* manager)
	{
		gameManager = manager;
	}

	Food* CreateFood(int x, int y);
	SpikyBall* CreateSpikyBall(int x, int y, int mass);
	SpikyBall* CreateSpikyBall(int x, int y, int mass, int idx);

	Spore* CreateSpore(BBPlayerNode* sourceNode);
	Spore* DebugCreateSpore();
	inline unsigned int GetNextObjIdx()
	{
		return _curObjIdx++;
	}
private:
	BBGameManager* gameManager;
	unsigned int _curObjIdx;
};

#endif