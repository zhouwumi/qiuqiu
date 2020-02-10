#ifndef BB_FRAME_CACHE_MANAGER_CPP
#define BB_FRAME_CACHE_MANAGER_CPP

#include<unordered_map>
#include<vector>
#include "Command.h"

class BBFrameCacheManager
{
public:
	BBFrameCacheManager();
	~BBFrameCacheManager();

	void OnKeyFrameUpdate();
	//需要返回的一些信息
	void AddFrameRemovePlayerNodeIdxs(int playerNodeIdx, int playerId);
	
	/*void NotifyClientNewFoodCommand(int idx);*/
	void NotifyClientNewSpikyCommad(int idx);

	//记录一些操作
	//void AddOperateMove(int uid, int angle, int percent);
public:
	/*std::unordered_map<int, std::vector<int>> newPlayerNodes;*/
	/*std::vector<int> newFoodIdxs;*/ //c->lua有新生成的食物
	std::vector<int> newSpikyIdxs; //c->lua有新生成的刺球
	std::vector<int> frameRemovePlayerNodeIdxs;
	

	//std::unordered_map<int, SimpleMoveOperate> moveOperates;
};

#endif