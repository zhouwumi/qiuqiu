#ifndef BB_PLAYER_NODE_CPP
#define BB_PLAYER_NODE_CPP
#include"BBMovingBall.h"
#include "BBVector.h"
#include<vector>
#include<unordered_map>
#include "BBFrameData.h"

class BBPlayer;
class BBGameManager;

class BBPlayerNode : public MovingBall
{
public:
	BBPlayerNode();
	~BBPlayerNode();

	void ChangeCd(int timeDelta);

	void InitMove();
	void CalculateInitMoveParams(int radius, int frame, int initSpeed);
	unsigned int GetCrc();
	void Move(BBGameManager* gameManager);
	void Eat(BBGameManager* gameManager);
	void EatFoodSpikySporeChangeMass(BBGameManager* gameManager);

	virtual void SetSpeedVec(double targetX, double targetY);

	int SpikySplit(BBGameManager* gameManager, int maxChildNode, int spikyMass);
	BBPlayerNode* SelfSplit(BBGameManager* gameManager);

	BBFrameBallData GetBallData();
	void ClientSyncBallDataFromServer(BBFrameBallData& ballData);
	void clientCatchup() {};
	void CalcBallDelta();
	void ChangeRenderPosition(double x, double y);

	int GetIntSpeedX();
	int GetIntSpeedY();

	int GetIntLocationX();
	int GetIntLocationY();

	int GetRenderX();
	int GetRenderY();

	//client
	void ClientSyncServerPlayerNode(int fromId, int x, int y, int mass, int cd, int protect, int initStopFrame, int initSpeed, int initDeltaSpeed, int speedX, int speedY);
private:
	BBVector _stepMove();
	bool _handleNodeHit(BBVector& locVec);
public:
	unsigned int uid;
	int cd;//不能融合的CD
	int protect;//刚出生的球不能被吃和不能吃别人的CD

	//初始的一些参数记录
	int initStopFrame; //停下来降为0的帧数
	int initSpeed;
	int initDeltaSpeed;

	bool isSelfSplit;
	BBPlayer* player;
	BBBallDeltaData mDeltaData;
};

#endif