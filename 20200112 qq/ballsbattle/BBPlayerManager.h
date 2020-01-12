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
	void ServerCreatePlayer(int uid);
	//给这个玩家加一个球
	void ServerCreatePlayerNode(int uid);
	Player* GetPlayer(int uid);
	void RemovePlayerNode(int uid, int nodeIdx, bool removeFromVec = true);

	void PlayerSplit(int uid);
	PlayerNode* DoPlayerNodeSelfSplit(PlayerNode* sourceNode);

	int DoPlayerNodeSpikySplit(PlayerNode* sourceNode, int maxChildNode, int spikyMass);

	void RemoveMass();
	void MovePlayers();
	void DoEat(std::vector<int>& eatResults);
	void DoEat(PlayerNode* node, std::vector<int>& eatResults);
	void FinishEat();
	void FinishEatChangeMass();
	void FinishEatReleate();
	void KeyFrameUpdatePlayer();

	void DoShoot();
	void DoSpikySplit();
	void DoPlayerSplit();
	void DoJoin();
	void AjustVector();
	void MakePhoto(int keyFrame);
	void ResetPhoto(int keyFrame);
	unsigned int GetAllPlayerCrc();

	void AddNewBallFromServer(int idx, int fromId, int uid, int mass, int x, int y, int directionX, int directionY, int currentX, int currentY, int curSpeed, int deltaSpeed, int Init, int Cd, int Protect);
	void CreatePlayerFromServer(int uid, int directionX, int directionY, int finalX, int finalY, bool isStopped, int NMass);
	void AddPlayerSplitNewBallFromServer(int idx, int fromId, int uid, int mass, int x, int y, int directionX, int directionY, int currentX, int currentY, int curSpeed, int deltaSpeed, int Init, int Cd, int Protect);
	void SetPlayerProperty(int uid, bool isBalance);
	
	void CopyPlayerManager(BBPlayerManager* otherPlayerManager);

private:
	void MovePlayer(Player* player);
	void _KeyFrameUpdatePlayer(Player* player);
	void _onNewPlayerNodeGenerate(Player* player, PlayerNode* playerNode, bool addNew = true);
	void _eat(std::vector<int>& vec, int beEatType, int beEatId, int eatType, int eatId);
	void _keyFrameChangePlayerFinalPoint(Player* player);
	bool _handleNodeHit(Player* player, PlayerNode* node, BBPoint& locVec);
	void _moveNode(Player* player, PlayerNode* node, BBPoint& locVec);
public:
	std::unordered_map<unsigned int, PlayerNode*> mapPlayNodes; //玩家的球
	std::vector<int> playerIds;
	std::unordered_map<unsigned int, Player*> mapPlayers;//玩家

	int timeHit;
	int timeUpdateCirle;
	int timeMovePlayer;
private:
	BBGameManager * gameManager;
	
};

#endif