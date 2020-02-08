#ifndef BB_GAME_MANAGER_CPP
#define BB_GAME_MANAGER_CPP
#include<unordered_map>
#include "BBPlayerNode.h"
#include "BBPlayer.h"
#include"QuadTree.h"
#include "BBHitManager.h"
#include "BBObjectManager.h"
#include "BBPlayerManager.h"
#include<functional>
#include "GridMap.h"
#include "BBFrameCacheManager.h"
#include "BBFoodSpikyManager.h"
#include "BBSporeManager.h"
#include "BBFrameOutManager.h"
#include "BBFrameInManager.h"

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
	void AddNodesCd();
	void FrameUpdate();

	//同步各种操作
	void AddOperatePlayerJoin(int uid);//玩家加入
	void AddPlayerQuit(int uid);
	void AddPlayerCommand(int uid, int angle, int pressure, bool isSplit, bool isShoot, int id, unsigned int checkSum);
	bool CanAddPlayerCommand(int uid);

	SpikyBall* GetSpikyInfo(int idx);
	Spore* GetSporeInfo(int idx);
	BBPlayerNode* GetPlayerNodeInfo(int idx);
	BBPlayer* GetPlayer(int uid);

	inline void SetServerState(bool isServer)
	{
		this->isServer = isServer;
	}
	inline bool IsServer()
	{
		return this->isServer;
	}

	inline void SetUserId(int userId)
	{
		this->userId = userId;
	}

	inline int GetUserId()
	{
		return this->userId;
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

	bool IsPlayerCommandMatch(unsigned int uid);
	unsigned int GetAckCommand(unsigned int uid);
	void SyncServerAckCommand(unsigned int uid,int ackCommand);

	inline void SetCanEatFoodSpiky(bool can)
	{
		canEatFoodSpiky = can;
	}
	inline bool GetCanEatFoodSpiky()
	{
		return canEatFoodSpiky;
	}

	inline void SetCanEatSpore(bool can)
	{
		canEatSpore = can;
	}

	inline bool GetCanEatSpore()
	{
		return canEatSpore;
	}

	inline void SetCanEatNode(bool can)
	{
		canEatNode = can;
	}
	inline bool GetCanEatNode()
	{
		return canEatNode;
	}


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
	
	std::vector<int> GetPlayerNodeIdx(int uid);

	void AddNewFoodFromServer(int posKey);
	void AddNewSpikyFromServer(unsigned int idx, int posKey, int mass);
	void AddNewSporeFromServer(unsigned int idx, unsigned int fromId, unsigned int uid, int mass, int x, int y, int directionX, int directionY, int currentX, int currentY, int curSpeed, int deltaSpeed, int initStopFrame, int cd);
	void CreatePlayerNodeFromServer(unsigned int uid, unsigned int idx, unsigned int fromId, int x, int y, int mass, int cd, int protect, int initStopFrame, int initSpeed, int initDeltaSpeed);
	void RemovePlayerNodeFromServer(unsigned int uid, unsigned int idx);
	void CreatePlayerFromServer(unsigned int uid, unsigned int idx, int directionX, int directionY, int NMass, int nextNodeIdx);

	void ClientSyncServerPlayerInfo(unsigned int uid, int nextNodeIdx, int nMass, int directionX, int directionY, int ackCommand);
	void ClientSyncServerPlayerNode(unsigned int uid, int idx, int fromId, int x, int y, int mass, int cd, int protect, int initStopFrame, int initSpeed, int initDeltaSpeed);
	void CalcBallDelta(unsigned int uid);
	void ClientCatchup(unsigned int uid);

	/***********************client接口***********************/

private:
	
public:
	std::unordered_map<unsigned int, Spore*> mapSpores;//孢子	
	QuadTree NodeTree; //玩家的球

	BBHitManager hitManager;
	BBObjectManager objectManager;
	BBPlayerManager playerManager;
	BBFrameCacheManager frameCacheManager;
	BBFrameOutManager frameOutManager;
	BBFrameInManager frameInManager;
	BBFoodSpikyManager foodSpikyManager;
	BBSporeManager sporeManager;
	BBRect spikyRect; //刺球生成的范围
	BBRect mapRect;  //整个地图的范围
private:
	int gameFrame;
	int currentGetServerKeyFrame;
	bool isServer;
	int userId;

	bool canEatFoodSpiky;
	bool canEatSpore;
	bool canEatNode;
};

#endif