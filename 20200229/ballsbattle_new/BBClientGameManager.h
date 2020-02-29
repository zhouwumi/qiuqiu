#ifndef BB_CLIENT_GAME_MANAGER_CPP
#define BB_CLIENT_GAME_MANAGER_CPP
#include "BBGameManager.h"

class BBClientGameManager : public BBGameManager
{
public:
	static BBClientGameManager* Create();
	BBClientGameManager();
	~BBClientGameManager();

	virtual void FrameUpdate();
	void UpdatePlayer(unsigned int uid);
	bool CanAddPlayerCommand(unsigned int uid);
	int GetPlayerDelay(unsigned int uid);
	int GetOtherPlayerDelay(unsigned int uid);

	void CreatePlayerNodeFromServer(unsigned int uid, unsigned int idx, unsigned int fromId, int x, int y, int mass, int cd, int protect, int initStopFrame, int initSpeed, int initDeltaSpeed, int speedX, int speedY);
	void CreateSplitPlayerNodeFromServer(unsigned int uid, unsigned int idx, unsigned int fromId, int x, int y, int mass, int speedX, int speedY, int cd, int protect, int initStopFrame, int initSpeed, int initDeltaSpeed);
	void RemovePlayerNodeFromServer(unsigned int uid, unsigned int idx);
	void CreatePlayerFromServer(unsigned int uid, int directionX, int directionY, int NMass, int finalPointX, int finalPointY, bool Stopped);

	void AddNewFoodFromServer(int posKey);
	void AddNewSpikyFromServer(unsigned int idx, int posKey, int mass);
	void AddNewSporeFromServer(unsigned int idx, unsigned int fromId, unsigned int uid, int x, int y, int directionX, int directionY, int curSpeed, int deltaSpeed, int initStopFrame, int cd);

	void SyncShootFromServer(int idx, int fromId, unsigned int uid, int speedX, int speedY, int locationX, int locationY);

	void RemoveFoodFromServer(int posKey);
	void RemoveSporeFromServer(int sporeIdx);

	void SyncServerAckCommand(unsigned int uid, int ackCommand);
	void ClientSyncServerPlayerInfo(unsigned int uid, int nMass, int directionX, int directionY, int ackCommand, int finalPointX, int finalPointY, bool Stopped);
	void ClientSyncServerPlayerNode(unsigned int uid, int idx, int fromId, int x, int y, int mass, int cd, int protect, int initStopFrame, int initSpeed, int initDeltaSpeed, int speedX, int speedY);
	void ClientSyncRenderToLocation(int idx);
	void SetRealClientRenderXY(int idx, double x, double y);
	void CalcBallDelta(unsigned int uid);
	void ClientCatchup(unsigned int uid);

	std::vector<int> SimulateEatFoods(std::vector<int> playerIds);
	std::vector<int> SimulateEatSpores(std::vector<int> playerIds);

	std::vector<int> GetNodeInRound(int nodeIdx, int expandCircle);
	void OnNodeEatFood(int nodeIdx, int massChange);
};
#endif
