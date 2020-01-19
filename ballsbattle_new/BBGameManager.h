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
	void OnFrameUpdate();
	void OnKeyFrameUpdate();
	void ClientKeyFrameUpdateBefore();
	void ClientKeyFrameUpdate();
	void DoPlayerSplit();
	void ClientBeginSyncServerOperate(int serverKeyFrame);
	void StartSimulate(int serverKeyFrame);
	void OnSimulate(bool isSimulateKeyFrame);
	void StopSimulate(int serverKeyFrame);
	void EndUpdate();
	void AddNodesCd();

	//同步各种操作
	void AddOperatePlayerJoin(int uid);//玩家加入
	void AddOperatePlayerSplit(int uid);//玩家分裂
	void AddOperateMove(int uid, int angle, int percent);//改变运动方向
	void AddOperatePlayerQuit(int uid);
	void AddOperatePlayerShoot(int uid);

	int GetFoodIdxByPos(int pos);
	int GetFoodPosByIdx(int idx);
	SpikyBall* GetSpikyInfo(int idx);
	Spore* GetSporeInfo(int idx);
	PlayerNode* GetPlayerNodeInfo(int idx);
	Player* GetPlayer(int uid);

	void SetServerState(bool isServer)
	{
		this->isServer = isServer;
	}
	bool IsServer()
	{
		return this->isServer;
	}

	int GetGameFrame()
	{
		return gameFrame;
	}

	void SetGameFrame(int serverGameFrame)
	{
		gameFrame = serverGameFrame;
	}

	void SetNextObjectIdx(int nextIdx)
	{
		objectManager.SetServerNextObjIndex(nextIdx);
	}

	int GetNextObjectIdx()
	{
		return objectManager.GetNextObjIdx();
	}

	int GetTimeMove()
	{
		return timeMove;
	}

	int GetTimeEat()
	{
		return timeEat;
	}

	int GetTimeHit()
	{
		return timeHit;
	}

	int GetUpdateCircle()
	{
		return timeUpdateCircle;
	}
	
	int GetMovePlayer()
	{
		return timeMovePlayer;
	}

	int GetCrc(int playerId);
	unsigned int GetAllPlayerCrc();
	unsigned int GetAllFoodCrc();
	unsigned int GetAllSpikyCrc();
	unsigned int GetAllSporeCrc();

	/***********************common接口***********************/

	/***********************server接口***********************/	

	void InitFoodSpiky();
	std::vector<int> GetAllFoodInfos();
	std::vector<int> GetAllSpikyInfos();
	std::vector<int>& GetAllSporeInfos();
	std::vector<int>& GetAllPlayerIdxs();
	std::vector<int> GetAllPlayerNodeIdxs(int playerId);

	std::vector < std::string> GetServerLog();
	void ClearServerLog();

	/***********************server接口***********************/


	/***********************client接口***********************/

	//客户端刚进来需要创建所有的食物和刺球
	//return:如果不是一开始调用会返回false
	//客户端提供的生成食物的接口,真实环境不用此接口
	std::vector<int> ClientGenerateFood(int num);
	std::vector<int> ClientGenerateSpiky(int num);

	std::vector<int>& GetFrameNewPlayer();
	std::vector<int> GetFrameNewPlayerNodeIdxs();
	std::vector<int>& GetFrameRemovedPlayerNodeIdxs();
	std::vector<int>& GetFrameNewFood();
	std::vector<int>& GetFrameNewSpiky();
	std::vector<int>& GetFrameNewSpore();
	std::vector<int>& GetFrameEatResult()
	{
		return eatResults;
	}
	std::vector<int> GetPlayerNodeIdx(int uid);

	void AddNewFoodFromServer(int posKey);
	void AddNewSpikyFromServer(int idx, int posKey, int mass);
	void AddNewSporeFromServer(int idx, int fromId, int uid, int mass, int x, int y, int directionX, int directionY, int currentX, int currentY, int curSpeed, int deltaSpeed, int Init, int Cd);
	void AddNewBallFromServer(int idx, int fromId, int uid, int mass, int x, int y, int directionX, int directionY, int currentX, int currentY, int curSpeed, int deltaSpeed, int Init, int Cd, int Protect);
	void CreatePlayerFromServer(int uid, int directionX, int directionY, int finalX, int finalY, bool isStopped, int NMass);
	/***********************client接口***********************/

private:
	
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
	int currentGetServerKeyFrame;
	bool isServer;
	int timeMove;
	int timeEat;
	int timeHit;
	int timeUpdateCircle;
	int timeMovePlayer;
};

#endif