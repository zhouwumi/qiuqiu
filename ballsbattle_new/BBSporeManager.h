#ifndef BB_SPORE_CPP
#define BB_SPORE_CPP
#include<vector>
#include<unordered_map>
#include <algorithm>
#include "BBObjects.h"
#include "BBMovingBall.h"
#include "BBPlayerNode.h"

class BBGameManager;

class BBSporeManager
{
public:
	BBSporeManager();
	~BBSporeManager();
	void SetGameManager(BBGameManager* manager)
	{
		gameManager = manager;
	}

	Spore* CreateSpore(PlayerNode* node);
	void RemoveSporeByIdx(int idx);
	void ClearAllSpore();

	void FixSpore(Spore* spore);
	Spore* GetSporeInfo(int idx);
	void MoveSpores();

	unsigned int GetAllSporeCrc();
	void AddNewSporeFromServer(int idx, int fromId, int uid, int mass, int x, int y, int directionX, int directionY, int currentX, int currentY, int curSpeed, int deltaSpeed, int Init, int Cd);
public:
	std::unordered_map<unsigned int, Spore*> mapSpores;//孢子
	std::vector<int> vecSporeIds;
private:
	BBGameManager * gameManager;
};

#endif