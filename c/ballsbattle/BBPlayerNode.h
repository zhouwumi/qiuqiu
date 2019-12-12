#pragma once
#include"BBMovingBall.h"
#include<vector>

class PlayerNode : public MovingBall
{
public:
	PlayerNode();
	~PlayerNode();

	void ChangeCd(int timeDelta);

	//�������
	void SpikySplit(int maxChildNode, int spikyMass, std::vector<PlayerNode*>& newPlayerNodes);
	PlayerNode* SelfSplit();
	BBVector& InitMove();
	void CalculateInitMoveParams(int radius, int frame, float initSpeed, int finalSpeed);

public:
	int Uid;
	int Cd;//�����ںϵ�CD
	int Protect;//�ճ��������ܱ��ԺͲ��ܳԱ��˵�CD
	//��ʼ��һЩ������¼		
	float initSpeed;
	float initDeltaSpeed;
};

class Player
{
public:
	Player(int id);
	~Player();

	void ResetPoint(int x, int y);
	void UpdateFinalPoint(int x, int y);
public:
	std::vector<PlayerNode*> vecPlayerNodes;
	int uid;
	BBPoint FromPoint;
	BBPoint FinalPoint;
};
