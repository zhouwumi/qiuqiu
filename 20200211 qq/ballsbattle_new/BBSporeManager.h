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

	Spore* CreateSpore(BBPlayerNode* node);
	void RemoveSporeByIdx(int idx);
	void ClearAllSpore();

	void FixSpore(Spore* spore);
	Spore* GetSporeInfo(int idx);
	void MoveSpores();

	unsigned int GetAllSporeCrc();
	void AddNewSporeFromServer(int idx, int fromId, unsigned int uid, int x, int y, int directionX, int directionY, int initSpeed, int initDeltaSpeed, int initStopFrame, int cd);
	void SyncShootFromServer(int idx, int fromId, unsigned int uid, int speedX, int speedY, int locationX, int locationY);
public:
	std::unordered_map<unsigned int, Spore*> mapSpores;//孢子
	std::vector<int> vecSporeIds;
private:
	BBGameManager * gameManager;
};

#endif