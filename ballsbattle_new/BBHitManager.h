#ifndef BB_HIT_MANAGER_CPP
#define BB_HIT_MANAGER_CPP

#include "BBPlayerNode.h"
#include "BBPlayer.h"

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
	void GetCanEatFood(BBPlayerNode* playerNode, std::vector<int>& vec);
	void GetCanEatFood(BBPlayer* player, std::vector<int>& vec);
	void GetCanEatFood(int playerId, std::vector<int>& vec);

	//找到一个空闲的刺球位置,要求不能和其他刺球,孢子碰撞
	void FindFreeSpikyPos(int& returnMass, int& returnX, int& returnY);
	bool FindFreeFoodPos(int& returnX, int& returnY);
	void FindFreePlayerNodePos(int radius, int& returnX, int& returnY);

	void GetCanEatSpiky(BBPlayerNode* playerNode, std::vector<int>& vec);
	void GetCanEatSpiky(BBPlayer* player, std::vector<int>& vec);
	void GetCanEatSpiky(int playerId, std::vector<int>& vec);

	void GetCanEatNodeOrSpore(BBPlayerNode* playerNode, std::vector<int>& nodeIdxs, std::vector<int>& sporeIdxs);
private:
	void _tryEatNode(BBPlayerNode* playerNode, BBPlayerNode* targetNode, std::vector<int>& nodeIdxs);
	void _tryEatSpore(BBPlayerNode* playerNode, Spore* targetSpore, std::vector<int>& nodeIdxs);
private:
	BBGameManager* gameManager;
	std::vector<int> TempVec;
};

#endif

