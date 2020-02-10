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
	static void Destory(BBGameManager* manager);

	BBGameManager();
	~BBGameManager();

	/***********************common接口***********************/

	void InitRoom();

	void AddNodesCd();
	void FrameUpdate();
	
	
	void AddPlayerCommand(int uid, int angle, int pressure, bool isSplit, bool isShoot, int id, unsigned int checkSum);

	SpikyBall* GetSpikyInfo(int idx);
	Spore* GetSporeInfo(int idx);
	BBPlayerNode* GetPlayerNode(int idx);
	BBPlayer* GetPlayer(int uid);

	std::vector<int> GetAllPlayerNodeIdxs(int playerId);


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


	inline int GetGameFrame()
	{
		return mGameFrame;
	}

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

	//客户端刚进来需要创建所有的食物和刺球
	//return:如果不是一开始调用会返回false
	//客户端提供的生成食物的接口,真实环境不用此接口
	std::vector<int> GetPlayerNodeIdx(int uid);

	
	/***********************client接口***********************/

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
	int mGameFrame;
	int currentGetServerKeyFrame;
	bool isServer;
	int userId;

	bool canEatFoodSpiky;
	bool canEatSpore;
	bool canEatNode;
};

#endif