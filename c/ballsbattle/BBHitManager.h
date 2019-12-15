#pragma once

#include "BBPlayerNode.h"

class BBGameManager;

class BBHitManager
{
public:
	BBHitManager();
	~BBHitManager();

	void SetGameManager(BBGameManager* manager)
	{
		gameManager = manager;
	}

	//玩家吃食物
	void GetCanEatFood(PlayerNode* playerNode, std::vector<int>& vec);
	void GetCanEatFood(Player* player, std::vector<int>& vec);
	void GetCanEatFood(int playerId, std::vector<int>& vec);

	//找到一个空闲的刺球位置,要求不能和其他刺球,孢子碰撞
	void FindFreeSpikyPos(int& returnMass, int& returnX, int& returnY);
	bool FindFreeFoodPos(int& returnX, int& returnY);
	void FindFreePlayerNodePos(int radius, int& returnX, int& returnY);

	void GetCanEatSpiky(PlayerNode* playerNode, std::vector<int>& vec);
	void GetCanEatSpiky(Player* player, std::vector<int>& vec);
	void GetCanEatSpiky(int playerId, std::vector<int>& vec);

	void CheckHitSpiky(SpikyBall* ball1, SpikyBall* ball2);
private:
	BBGameManager* gameManager;
	std::vector<int> TempVec;
};

