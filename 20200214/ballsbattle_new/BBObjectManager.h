#ifndef BB_OBJECT_MANAGER_CPP
#define BB_OBJECT_MANAGER_CPP
#include "BBObjects.h"
#include "BBPlayerNode.h"
#include "BBPlayer.h"
#include "BBRect.h"
#include "BBVector.h"
#include "BBPoint.h"

#define IDX_START_SPIKY 10000
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
	inline unsigned int GetNextPlayerIdx()
	{
		return _nextPlayerIdx++;
	}

	//同步服务器的下一个对象idx
	void SetServerNextObjIndex(int nextIdx)
	{
		_curObjIdx = nextIdx;
	}

	//1 * 10000
	inline unsigned int GetNextSpikyIdx()
	{
		return _nextSpikyIdx++ + IDX_START_SPIKY;
	}

private:
	BBGameManager* gameManager;
	unsigned int _curObjIdx;
	unsigned int _nextPlayerIdx;
	unsigned int _nextSpikyIdx;
};

#endif