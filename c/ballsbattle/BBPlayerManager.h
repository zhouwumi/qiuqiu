#ifndef BB_PLAYER_MANAGER_CPP
#define BB_PLAYER_MANAGER_CPP
#include<unordered_map>
#include"BBPlayerNode.h"

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

	//玩家
	void CreatePlayer(int uid);
	//给这个玩家加一个球
	void CreatePlayerNode(int uid);
	void RemovePlayer(int uid);
	void RemovePlayerNode(int uid, int nodeIdx, bool removeFromVec = true);

	void PlayerSplit(int uid);
	PlayerNode* DoPlayerNodeSelfSplit(PlayerNode* sourceNode);

	int DoPlayerNodeSpikySplit(PlayerNode* sourceNode, int maxChildNode, int spikyMass);

	void RemoveMass();
	void MovePlayers(bool isKeyFrame);
	void DoEat(std::vector<int>& eatResults);
	void DoEat(PlayerNode* node, std::vector<int>& eatResults);
	void FinishEat();
	void FinishEatChangeMass();
	void FinishEatReleate();

	void DoShoot();
	void DoSplit();
	void DoJoin();
	void AjustVector();
private:
	void MovePlayer(Player* player, bool isKeyFrame);
	void _onNewPlayerNodeGenerate(Player* player, PlayerNode* playerNode);
	void _eat(std::vector<int>& vec, int beEatType, int beEatId, int eatType, int eatId);

public:
	std::unordered_map<unsigned int, PlayerNode*> mapPlayNodes; //玩家的球
	std::vector<int> playerIds;
	std::unordered_map<unsigned int, Player*> mapPlayers;//玩家
private:
	BBGameManager * gameManager;
};

#endif