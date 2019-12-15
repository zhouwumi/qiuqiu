#pragma once
#include<unordered_map>
#include"BBPlayerNode.h"
#include"QuadTree.h"
#include "BBHitManager.h"
#include "BBObjectManager.h"
#include<functional>

class BBGameManager
{
public:
	//��ʼ������
	void InitRoom();
	void InitFood();

	//foodʳ��
	void CreateFood();
	void RemoveFoodByIdx(int idx);
	void RemoveFoodByPos(int posKey);

	//����
	void CreateSpiky();
	void RemoveSpikyByIdx(int idx);

	//���
	void CreatePlayer(int uid);
	//�������Ҽ�һ����
	void CreatePlayerNode(int uid);
	void RemovePlayer(int uid);
	void RemovePlayerNode(int uid, int nodeIdx, bool removeFromVec = true);

	//ÿ֡����
	void OnUpdate();
	
	PlayerNode* SpawnPlayerNode();
	static BBGameManager* getInstance();
	void destroyInstance();
protected:
	BBGameManager();
	~BBGameManager();
	static BBGameManager* s_sharedGameManager;
public:
	std::vector<int> GetAllFoodInfos();
	std::vector<int> GetAllSpikyInfos();
	int GetFoodIdxByPos(int pos);
	int GetFoodPosByIdx(int idx);
	SpikyBall* GetSpikyInfo(int idx);
	std::vector<int> GetPlayerNodeIdx(int uid);
	PlayerNode* GetPlayerNodeInfo(int idx);
	std::vector<int> CheckPlayerHit(int uid);
	void ChangePlayerNodePos(int idx, int x, int y);

	void CheckSpikyHit(int idx1, int idx2);

	void SetEatCbFunction(std::function<void(int, int, int, int)> eatFunc)
	{
		eatCallback = eatFunc;
	}
	void RemoveEatCbFunction()
	{
		eatCallback = NULL;
	}

	void DebugPrintLog();

	void StartMovePlayer(int uid);

private:
	//��Ҳ���
	void _updatePlayerCenter(int uid);
	//����ƶ�
	void MovePlayer(Player* player);
public:
	std::unordered_map<unsigned int, PlayerNode*> mapPlayNodes; //��ҵ���
	std::unordered_map<unsigned int, Player*> mapPlayers;//���
	std::unordered_map<unsigned int, Spore*> mapSpores;//����
	std::unordered_map<unsigned int, Food*> mapFoodPos;//ʳ��λ��key
	std::unordered_map<unsigned int, Food*> mapFoodIdxs;//ʳ��idx key
	std::unordered_map<unsigned int, SpikyBall*> mapSpikyBalls;//����

	QuadTree ObjectTree; //����,����
	QuadTree FoodTree; //ʳ��
	QuadTree NodeTree; //��ҵ���

	BBHitManager hitManager;
	BBObjectManager objectManager;

	BBRect spikyRect; //�������ɵķ�Χ
	BBRect mapRect;  //������ͼ�ķ�Χ
private:
	//obj_type, obj_idx, eat_obj_type, eat_obj_idx
	std::function<void(int, int, int, int)> eatCallback;
};

