#ifndef BB_PLAYER_CPP
#define BB_PLAYER_CPP

#include<vector>
#include "BBVector.h"
#include "BBRect.h"
#include "BBSyncObject.h"

class BBPlayerNode;
class BBGameManager;

class BBPlayer:public BBSyncObject
{
public:
	BBPlayer(unsigned int id, unsigned int idx);
	~BBPlayer();
	bool isMe();
	unsigned int GetNextPlayerNodeIdx(bool increase = true);
	inline void ResetLastCommand()
	{
		lastMoveCommand = -1;
	}
	bool IsNeedSyncState();
	bool CanSkipSyncState();

	void AddPlayerNode(BBPlayerNode* node);
	BBPlayerNode* GetPlayerNode(int idx);
	void UpdateFinalPoint(double x, double y);
	int GetFinalPointX();
	int GetFinalPointY();

	BBRect GetGroupRect();
	void RemoveMass();

	unsigned int GetCrc();

	void SetGameManager(BBGameManager* gameManager);

	void Update();
	void Move();
	void Eat();

	void SelfSplit(BBGameManager* gameManager);
	void ServerSelfShoot(BBGameManager* gameManager);

	//client
	void ClientSyncServerPlayerInfo(int nextNodeIdx, int nMass, int directionX, int directionY, int ackCommand, int finalPointX, int finalPointY, bool Stopped);
	void CalcBallDelta();

	//----extend---//
	virtual void processTick(BBMoveCommand& command);
	virtual BBFrameData serverGetFrameData();
	virtual void clientReadFrameData(BBFrameData frameData);
	virtual void clientCatchup();
	virtual BBFrameData packUpdate();
	virtual void unPackUpdate(BBFrameData frameData);
private:
	void _tryUpdateFinalPoint();
	void _changeInitEndNodeSpeed();
	void _updateNodeTime();
public:
	BBGameManager* gameManager;
	std::vector<BBPlayerNode*> vecPlayerNodes;

	unsigned int uid;
	bool Stopped;
	BBVector FinalPoint;
	BBPoint direction;
	int NMass;
	unsigned int playerIdx;
	int nextNodeIdx;
	int lastMoveCommand;
	BBPredictionData predictionData;
	bool isCatchingUp;

	bool isShoot;
	bool isSplit;
};

#endif