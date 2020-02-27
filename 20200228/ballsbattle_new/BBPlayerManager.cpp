#include "BBPlayerManager.h"
#include "BBObjectManager.h"
#include "BBGameManager.h"
#include "BBConfigManager.h"

#include<algorithm>
#include "BBMathUtils.h"
#include<math.h>
#if defined(_WIN32) && defined(_WINDOWS)
#include "cocos2d.h"
#else
#include<stdarg.h>
#include<sys/time.h>
#endif

void BB_PlayerManager_gettimeofday(timeval& tm)
{
#if defined(_WIN32) && defined(_WINDOWS)
	cocos2d::gettimeofday(&tm, NULL);
#else
	gettimeofday(&tm, NULL);
#endif
}

int BB_PlayerManager_getCost(timeval time1, timeval time2)
{
	return (time1.tv_sec - time2.tv_sec) * 1000000 + (time1.tv_usec - time2.tv_usec);
}

BBPlayerManager::BBPlayerManager()
{
}


BBPlayerManager::~BBPlayerManager()
{
	for (auto iter : mapPlayNodes)
	{
		delete iter.second;
		iter.second = NULL;
	}
	mapPlayNodes.clear();

	for (auto iter : mapPlayers)
	{
		delete iter.second;
		iter.second = NULL;
	}
	mapPlayers.clear();
}

void BBPlayerManager::OnNewPlayerNodeGenerate(BBPlayer* player, BBPlayerNode* playerNode, bool addNew)
{
	player->AddPlayerNode(playerNode);
	mapPlayNodes.emplace(playerNode->idx, playerNode);
	gameManager->NodeTree.AddCircleNode(playerNode);

	if (addNew)
	{
		gameManager->frameOutManager.AddNewPlayerNode(player->uid, playerNode->idx);
	}
}

/**玩家部分**/
//server interface
void BBPlayerManager::ServerCreatePlayer(unsigned int uid)
{
	if (!gameManager->IsServer())
	{
		return;
	}
	BBPlayer* player = NULL;
	auto iter = mapPlayers.find(uid);
	if (iter == mapPlayers.end())
	{
		player = new BBPlayer(uid);
		player->SetGameManager(gameManager);
		mapPlayers.emplace(uid, player);
	}
	else {
		player = iter->second;
	}
	player->ResetCommands();
	if (player->vecPlayerNodes.size() == 0)
	{
		ServerCreatePlayerNode(uid);
	}
	gameManager->frameOutManager.AddNewPlayer(uid);
	for (int i = 0; i < playerIds.size(); i++)
	{
		if (playerIds[i] == uid)
		{
			return;
		}
	}
	playerIds.emplace_back(uid);
	std::sort(playerIds.begin(), playerIds.end());
}

//client interface
void BBPlayerManager::CreatePlayerFromServer(unsigned int uid, int directionX, int directionY, int NMass, int finalPointX, int finalPointY, bool Stopped)
{
	if (gameManager->IsServer())
	{
		return;
	}
	BBPlayer* player = new BBPlayer(uid);
	player->SetGameManager(gameManager);
	player->NMass = NMass;
	player->direction.x = directionX;
	player->direction.y = directionY;
	player->FinalPoint.x = BBMathUtils::bb_int_to_float(finalPointX);
	player->FinalPoint.y = BBMathUtils::bb_int_to_float(finalPointY);
	player->Stopped = Stopped;
	mapPlayers.emplace(uid, player);
	//gameManager->frameOutManager.AddNewPlayer(uid);
	for (int i = 0; i < playerIds.size(); i++)
	{
		if (playerIds[i] == uid)
		{
			return;
		}
	}
	playerIds.emplace_back(uid);
	std::sort(playerIds.begin(), playerIds.end());
}

void BBPlayerManager::ServerCreatePlayerNode(unsigned int uid)
{
	if (!gameManager->IsServer())
	{
		return;
	}
	BBPlayer* player = GetPlayer(uid);
	if (!player)
	{
		return;
	}
	BBPlayerNode* newPlayerNode = new BBPlayerNode();
	newPlayerNode->uid = uid;
	newPlayerNode->protect = BBConfigManager::protectTime;
	if (uid == gameManager->GetUserId())
	{
		if (BBConfigManager::debugMyBallMass > 0)
		{
			newPlayerNode->SetBallMass(BBConfigManager::debugMyBallMass);
		}
		else {
			newPlayerNode->SetBallMass(BBConfigManager::initBallMass);
		}
	}
	else
	{
		if (BBConfigManager::debugOtherBallMass > 0)
		{
			newPlayerNode->SetBallMass(BBConfigManager::debugOtherBallMass);
		}
		else {
			newPlayerNode->SetBallMass(BBConfigManager::initBallMass);
		}
	}
	newPlayerNode->idx = gameManager->objectManager.GetNextObjIdx();

	int radius = newPlayerNode->GetRadius();
	int x, y;
	gameManager->hitManager.FindFreePlayerNodePos(radius, x, y);

	if (BBConfigManager::clientDebug)
	{
		if (uid == gameManager->GetUserId())
		{
			x = 600;
			y = 600;
		}
		else
		{
			x = 500;
			y = 500;
		}
	}
	
	newPlayerNode->ChangePosition(x, y);
	newPlayerNode->ChangeRenderPosition(x, y);
	OnNewPlayerNodeGenerate(player, newPlayerNode);
}

BBPlayer* BBPlayerManager::GetPlayer(unsigned int uid)
{
	auto iter = mapPlayers.find(uid);
	if (iter == mapPlayers.end())
	{
		return NULL;
	}
	return iter->second;
}

void BBPlayerManager::RemovePlayerNode(unsigned int uid, unsigned int nodeIdx)
{
	if (mapPlayNodes.find(nodeIdx) == mapPlayNodes.end())
	{
		return;
	}
	BBPlayerNode* node = mapPlayNodes[nodeIdx];
	mapPlayNodes.erase(nodeIdx);
	gameManager->NodeTree.RemoveCircleNode(node);

	BBPlayer* player = mapPlayers[uid];
	if (player)
	{
		for (int i = player->vecPlayerNodes.size() - 1; i >= 0; i--)
		{
			if (player->vecPlayerNodes[i] == node)
			{
				player->vecPlayerNodes.erase(player->vecPlayerNodes.begin() + i);
				break;
			}
		}
	}
	
	delete node;
}

void BBPlayerManager::CreatePlayerNodeFromServer(unsigned int uid, unsigned int idx, int fromId, int x, int y, int mass, int cd, int protect, int initStopFrame, int initSpeed, int initDeltaSpeed, int speedX, int speedY)
{
	BBPlayerNode* newPlayerNode = new BBPlayerNode();
	newPlayerNode->uid = uid;
	newPlayerNode->idx = idx;
	newPlayerNode->cd = cd;
	newPlayerNode->fromId = fromId;
    newPlayerNode->SetBallMass(mass);
	newPlayerNode->protect = protect;
	newPlayerNode->currentSpeedVec.SetPoint(BBMathUtils::bb_int_to_float(speedX), BBMathUtils::bb_int_to_float(speedY));
	newPlayerNode->ChangePosition(BBMathUtils::bb_int_to_float(x), BBMathUtils::bb_int_to_float(y));
	newPlayerNode->ChangeRenderPosition(BBMathUtils::bb_int_to_float(x), BBMathUtils::bb_int_to_float(y));
	newPlayerNode->initStopFrame = initStopFrame;
	newPlayerNode->initSpeed = initSpeed;
	newPlayerNode->initDeltaSpeed = initDeltaSpeed;
	BBPlayer* player = GetPlayer(uid);
	//从lua层来主动创建的node不放入到新的玩家节点列表中
	OnNewPlayerNodeGenerate(player, newPlayerNode, false);
}

void BBPlayerManager::CreateSplitPlayerNodeFromServer(unsigned int uid, unsigned int idx, unsigned int fromId, int x, int y, int mass,int speedX, int speedY, int cd, int protect)
{
	BBPlayer* player = GetPlayer(uid);
	if (!player)
	{
		return;
	}
	BBPlayerNode* sourceNode = player->GetPlayerNode(fromId);
	if (!sourceNode)
	{
		return;
	}
	BBPlayerNode* newPlayerNode = new BBPlayerNode();
	newPlayerNode->fromId = fromId;
	newPlayerNode->uid = uid;
	newPlayerNode->player = player;
	newPlayerNode->idx = idx;
	newPlayerNode->ChangePosition(BBMathUtils::bb_int_to_float(x), BBMathUtils::bb_int_to_float(y));
	newPlayerNode->currentSpeedVec.SetPoint(BBMathUtils::bb_int_to_float(speedX), BBMathUtils::bb_int_to_float(speedY));
	newPlayerNode->SetBallMass(mass);
	newPlayerNode->cd = cd;
	newPlayerNode->protect = protect;
	BBVector moveVec = BBVector::GetFixedVetor2(newPlayerNode->currentSpeedVec, sourceNode->GetRadius());
	newPlayerNode->ChangeRenderPosition(sourceNode->location.x + moveVec.x, sourceNode->location.y + moveVec.y);
	//newPlayerNode->ChangeRenderPosition(sourceNode->realX +moveVec.x, sourceNode->realY + moveVec.y);
	newPlayerNode->CalcBallDelta();
	newPlayerNode->CalculateInitMoveParams(newPlayerNode->GetRadius(), BBConfigManager::splitStopFrame, BBConfigManager::splitInitSpeed);
	OnNewPlayerNodeGenerate(player, newPlayerNode, false);
}


void BBPlayerManager::HandleFrameInputCommand()
{
	auto& playerCommands = gameManager->frameInManager.playerCommands;
	for(auto& iter : playerCommands)
	{
		unsigned int uid = iter.first;
		auto& commands = iter.second;
		auto playerIter = mapPlayers.find(uid);
		if (playerIter != mapPlayers.end())
		{
			for (auto& command : commands)
			{
				playerIter->second->mCommandList.pushCommand(command);
			}
		}
	}
}

//update state
void BBPlayerManager::UpdatePlayer()
{
	for (int i = 0; i < playerIds.size(); i++)
	{
		int playerId = playerIds[i];
		auto iter = mapPlayers.find(playerId);
		if (iter != mapPlayers.end())
		{
			iter->second->Update();
		}
	}
}

void BBPlayerManager::PlayerEat()
{
	for (int i = 0; i < playerIds.size(); i++)
	{
		auto iter = mapPlayers.find(playerIds[i]);
		if (iter != mapPlayers.end())
		{
			BBPlayer* player = (*iter).second;
			player->Eat();
		}
	}
}

void BBPlayerManager::FinishEat()
{
	_finishEatChangeMass();
	_finishEatReleate();
}

void BBPlayerManager::_finishEatChangeMass()
{
	for (int i = 0; i < playerIds.size(); i++)
	{
		auto iter = mapPlayers.find(playerIds[i]);
		if (iter != mapPlayers.end())
		{
			BBPlayer* player = (*iter).second;
			for (BBPlayerNode* node : player->vecPlayerNodes)
			{
				node->EatFoodSpikySporeChangeMass(gameManager);
			}
		}
	}
}

void BBPlayerManager::_finishEatReleate()
{
	auto beEatNodeId2EatInfos = gameManager->frameOutManager.beEatNodeId2EatInfos;
	std::unordered_map<int, int> fillMap;
	for(auto iter : beEatNodeId2EatInfos)
	{
		int beEatNodeIdx = iter.first;
		int eatNodeIdx = iter.second.nodeId;
		int finalIdx = eatNodeIdx;
		while (beEatNodeId2EatInfos.find(finalIdx) != beEatNodeId2EatInfos.end())
		{
			finalIdx = beEatNodeId2EatInfos[finalIdx].nodeId;
		}
		fillMap[beEatNodeIdx] = finalIdx;
	}
	auto nodeId2BeEatNodes = gameManager->frameOutManager.nodeId2BeEatNodes;
	std::unordered_map<int, int> massMap;
	for (auto iter : nodeId2BeEatNodes)
	{
		int key = iter.first;
		if (fillMap.find(key) != fillMap.end())
		{
			key = fillMap[key];
		}
		if (massMap.find(key) == massMap.end())
		{
			massMap[key] = 0;
		}
		for(auto info : iter.second)
		{
			BBPlayerNode* node = mapPlayNodes[info.beEatNodeId];
			if (node)
			{
				massMap[key] += node->GetBallMass();
			}
		}
	}

	for (auto iter : massMap)
	{
		int eatNodeIdx = iter.first;
		int changeMass = iter.second;
		BBPlayerNode* node = mapPlayNodes[eatNodeIdx];
		if (node->GetBallMass() + changeMass < BBConfigManager::initBallMass)
		{
			changeMass = BBConfigManager::initBallMass - node->GetBallMass();
		}
		node->ChangeDeltaMass(changeMass);
	}

	for (auto iter : beEatNodeId2EatInfos)
	{
		int beEatId = iter.first;
		NodeEatInfo& info = iter.second;
		RemovePlayerNode(info.beEatUid, info.beEatNodeId);
	}
	//....
}

void BBPlayerManager::DoSpikySplit()
{
	int size = gameManager->frameOutManager.spikyEatInfos.size();
	if (size > 0)
	{
		for (int i = 0; i < size; i++)
		{
			SpikyEatInfo& info = gameManager->frameOutManager.spikyEatInfos[i];
			int nodeId = info.nodeId;
			int spikyBallMass = info.spikyBallMass;
			BBPlayerNode* playerNode = mapPlayNodes[nodeId];
			int totalNum = playerNode->player->vecPlayerNodes.size();
			int maxCanSplit = BBConfigManager::maxBallCount - totalNum;
			if (maxCanSplit > BBConfigManager::maxSpikySplitChildCount)
			{
				maxCanSplit = BBConfigManager::maxSpikySplitChildCount;
			}
			playerNode->SpikySplit(gameManager, maxCanSplit, spikyBallMass);
		}
	}
}

void BBPlayerManager::ServerDoJoinPlayer()
{
	if (!gameManager->IsServer())
	{
		return;
	}
	auto& playerJoinIds = gameManager->frameInManager.playerJoinIds;
	for (int uid : playerJoinIds)
	{
		ServerCreatePlayer(uid);
	}
}

void BBPlayerManager::HandlePlayerQuit()
{
	auto& playerQuitIds = gameManager->frameInManager.playerQuitIds;
	for (int i = 0; i < playerQuitIds.size(); i++)
	{
		int uid = playerQuitIds[i];
		if (mapPlayers.find(uid) != mapPlayers.end())
		{
			BBPlayer* player = mapPlayers[uid];
			player->direction.SetPoint(0, 0);
		}
	}
}