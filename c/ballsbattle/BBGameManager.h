#ifndef BB_GAME_MANAGER_CPP
#define BB_GAME_MANAGER_CPP
#include<unordered_map>
#include"BBPlayerNode.h"
#include"QuadTree.h"
#include "BBHitManager.h"
#include "BBObjectManager.h"
#include "BBPlayerManager.h"
#include<functional>
#include "GridMap.h"
#include "BBFrameCacheManager.h"
#include "BBFoodSpikyManager.h"
#include "BBSporeManager.h"

class BBGameManager
{
public:
	static BBGameManager* Create();
	static void Destory(BBGameManager* manager);
	BBGameManager();
	~BBGameManager();

	/***********************common接口***********************/

	void InitRoom();

	//每帧更新
	void OnUpdate();
	void EndUpdate();

	//同步各种操作
	void AddOperatePlayerJoin(int uid);//玩家加入
	void AddOperatePlayerSplit(int uid);//玩家分裂
	void AddOperateMove(int uid, int angle, int percent);//改变运动方向
	void AddOperatePlayerQuit(int uid);
	void AddOperatePlayerShoot(int uid);

	int GetGameFrame()
	{
		return gameFrame;
	}
	bool isKeyFrame();
	/***********************common接口***********************/

	/***********************server接口***********************/	

	void InitFoodSpiky();

	/***********************server接口***********************/


	/***********************client接口***********************/

	//客户端刚进来需要创建所有的食物和刺球
	//return:如果不是一开始调用会返回false
	bool ClientFinishGenerateFoodAndSpiky();
	//客户端提供的生成食物的接口,真实环境不用此接口
	std::vector<int> ClientGenerateFood(int num);
	std::vector<int> ClientGenerateSpiky(int num);

	std::vector<int> GetFrameNewPlayer();
	std::vector<int> GetFrameNewPlayerNodeIdxs();
	std::vector<int> GetFrameRemovedPlayerNodeIdxs();
	std::vector<int> GetFrameNewFood();
	std::vector<int> GetFrameNewSpiky();
	std::vector<int> GetFrameNewSpore();
	std::vector<int> GetFrameEatResult()
	{
		return eatResults;
	}
	//信息获取
	std::vector<int> GetAllFoodInfos();
	std::vector<int> GetAllSpikyInfos();
	int GetFoodIdxByPos(int pos);
	int GetFoodPosByIdx(int idx);
	Spore* GetSporeInfo(int idx);
	SpikyBall* GetSpikyInfo(int idx);
	PlayerNode* GetPlayerNodeInfo(int idx);
	std::vector<int> GetPlayerNodeIdx(int uid);

	void AddOperateNewFood(int idx, int posKey);
	void AddOperateNewSpiky(int idx, int posKey, int mass);

	/***********************client接口***********************/

private:
	void _AddNodesCd();

public:
	std::unordered_map<unsigned int, Spore*> mapSpores;//孢子	
	QuadTree NodeTree; //玩家的球

	BBHitManager hitManager;
	BBObjectManager objectManager;
	BBPlayerManager playerManager;
	BBFrameCacheManager frameCacheManager;
	BBFoodSpikyManager foodSpikyManager;
	BBSporeManager sporeManager;
	BBRect spikyRect; //刺球生成的范围
	BBRect mapRect;  //整个地图的范围
private:
	std::vector<int> eatResults;
	int gameFrame;
};

#endif