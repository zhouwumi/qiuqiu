#pragma once
#include "BBObjects.h"
#include "BBPlayerNode.h"
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
	Player* CreatePlayer(int uid);
	PlayerNode* CreatePlayerNode(int uid, int mass, int cd, const BBPoint& FromLocation, const BBVector& Current, const BBVector& Delta, const BBVector& Final, const BBVector& Direction);
	PlayerNode*  CreatePlayerNode(int uid, int mass);


	unsigned int _getNextObjIdx();
private:
	

private:
	BBGameManager* gameManager;
	unsigned int _curObjIdx;
};

