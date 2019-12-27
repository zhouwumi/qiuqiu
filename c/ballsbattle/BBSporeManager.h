#ifndef BB_SPORE_CPP
#define BB_SPORE_CPP
#include<vector>
#include<unordered_map>
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

public:
	std::unordered_map<unsigned int, Spore*> mapSpores;//孢子
	std::vector<unsigned int> vecSporeIds;
private:
	BBGameManager * gameManager;
};

#endif