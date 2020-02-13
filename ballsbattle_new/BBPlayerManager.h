#ifndef BB_PLAYER_MANAGER_CPP
#define BB_PLAYER_MANAGER_CPP
#include<unordered_map>
#include "BBPlayerNode.h"
#include "BBPlayer.h"

class BBGameManager;

class BBPlayerManager
{
public:
	BBPlayerManager();
	~BBPlayerManager();

	void SetGameManager(BBGameManager* manager)
	{
		gameManager = manager;
	}

	inline unsigned int GetNextPlayerIdx()
	{
		return nextPlayerIdx++;
	}

	//玩家
	void ServerCreatePlayer(unsigned int uid);
	//给这个玩家加一个球
	void ServerCreatePlayerNode(unsigned int uid);
	BBPlayer* GetPlayer(unsigned int uid);
	void RemovePlayerNode(unsigned int uid, unsigned int nodeIdx);

	void HandleFrameInputCommand();
	void UpdatePlayer();
	void PlayerEat();
	void FinishEat();
	
	
	void DoSpikySplit();
	void ServerDoJoinPlayer();
	void HandlePlayerQuit();

	void CreatePlayerNodeFromServer(unsigned int uid, unsigned int idx, int fromId, int x, int y, int mass, int cd, int protect, int initStopFrame, int initSpeed, int initDeltaSpeed, int speedX, int speedY);
	void CreateSplitPlayerNodeFromServer(unsigned int uid, unsigned int idx, unsigned int fromId, int x, int y, int mass, int speedX, int speedY, int cd, int protect);
	void CreatePlayerFromServer(unsigned int uid, unsigned int playerIdx, int directionX, int directionY, int NMass, int nextNodeIdx, int finalPointX, int finalPointY, bool Stopped);
	void AddPlayerSplitNewBallFromServer(int idx, int fromId, int uid, int mass, int x, int y, int directionX, int directionY, int currentX, int currentY, int curSpeed, int deltaSpeed, int initStopFrame, int cd, int protect);
	
	void OnNewPlayerNodeGenerate(BBPlayer* player, BBPlayerNode* playerNode, bool addNew = true);

private:
	void _finishEatChangeMass();
	void _finishEatReleate();
public:
	std::unordered_map<unsigned int, BBPlayerNode*> mapPlayNodes; //玩家的球
	std::vector<int> playerIds;
	std::unordered_map<unsigned int, BBPlayer*> mapPlayers;//玩家

private:
	BBGameManager * gameManager;
	unsigned int nextPlayerIdx;
};

#endif