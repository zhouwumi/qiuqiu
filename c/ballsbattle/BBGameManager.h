#pragma once
#include<unordered_map>
#include"BBPlayerNode.h"
#include"QuadTree.h"

class BBGameManager
{
public:
	//��ʼ������
	void InitRoom();
	void InitFood();

	//foodʳ��
	void AddFoodByKey(int key, int x, int y);


	//���
	void AddPlayer(int uid);
	//�������Ҽ�һ����
	void AddPlayerNode(int uid);

	PlayerNode* SpawnPlayerNode();

	static BBGameManager* getInstance();
	void destroyInstance();
protected:
	BBGameManager();
	~BBGameManager();
	static BBGameManager* s_sharedGameManager;
private:
	unsigned int _getNextObjIdx();

	//��Ҳ���
	void _updatePlayerCenter(int uid);
public:
	std::unordered_map<unsigned int, PlayerNode*> mapPlayNodes; //��ҵ���
	std::unordered_map<unsigned int, Player*> mapPlayers;//���
	std::unordered_map<unsigned int, Spore*> mapSpores;//����
	std::unordered_map<unsigned int, Food*> mapFoods;//ʳ��
	std::unordered_map<unsigned int, SpikyBall*> mapSpikyBalls;//����

	QuadTree ObjectTree; //����,����
	QuadTree FoodTree; //ʳ��
	QuadTree NodeTree; //��ҵ���

private:
	BBRect spikyRect; //�������ɵķ�Χ
	BBRect mapRect;  //������ͼ�ķ�Χ
	unsigned int _curObjIdx;
};

