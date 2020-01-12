#ifndef BB_PLAYER_NODE_CPP
#define BB_PLAYER_NODE_CPP
#include"BBMovingBall.h"
#include "BBVector.h"
#include<vector>
#include<unordered_map>

struct PlayerNodeData
{
public:
	int Cd;
	int Protect;
	int initSpeed;
	int initDeltaSpeed;

	int Init; //停下来降为0的帧数
	BBPoint Location;
	BBVector Current;//当前的速度
	BBVector Direction; //当前的方向
	int mass;
};

class PlayerNode : public MovingBall
{
public:
	PlayerNode();
	~PlayerNode();

	void ChangeCd(int timeDelta);

	BBVector& InitMove();
	void CalculateInitMoveParams(int radius, int frame, int initSpeed, int finalSpeed);

	void MakePhoto(int keyFrame);
	void ResetPhoto(int keyFrame);

	void CopyPlayerNode(PlayerNode* otherPlayerNode);
	unsigned int GetCrc();
public:
	int Uid;
	int Cd;//不能融合的CD
	int Protect;//刚出生的球不能被吃和不能吃别人的CD
	//初始的一些参数记录		
	int initSpeed;
	int initDeltaSpeed;
	bool isHit;
private:
	std::unordered_map<int, PlayerNodeData> frameCacheDatas;
};

struct PlayerData
{
public:
	bool Stopped;
	BBVector Direction;
	int NMass;
	bool IsBalance;
};

class Player
{
public:
	Player(int id);
	~Player();

	void ResetPoint(int x, int y);
	void UpdateFinalPoint(int x, int y);

	BBRect GetGroupRect();
	void RemoveMass();

	void MakePhoto(int keyFrame);
	void ResetPhoto(int keyFrame);
	unsigned int GetCrc();

	void CopyFromOtherPlayer(Player* otherPlayer);
public:
	std::vector<PlayerNode*> vecPlayerNodes;

	int uid;
	bool Stopped;
	BBPoint FinalPoint;
	BBVector Direction;
	int NMass;
	bool IsBalance;
private:
	std::unordered_map<int, PlayerData> frameCacheDatas;
};

#endif