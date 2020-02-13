#include "BBClientGameManager.h"

BBClientGameManager* BBClientGameManager::Create()
{
	return new BBClientGameManager();
}

BBClientGameManager::BBClientGameManager()
{
}


BBClientGameManager::~BBClientGameManager()
{
}

void BBClientGameManager::SyncServerAckCommand(unsigned int uid, int ackCommand)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		player->mCommandList.clientSyncServerCommandAck(ackCommand);
	}
}

void BBClientGameManager::ClientSyncServerPlayerInfo(unsigned int uid, int nextNodeIdx, int nMass, int directionX, int directionY, int ackCommand, int finalPointX, int finalPointY, bool Stopped)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		player->ClientSyncServerPlayerInfo(nextNodeIdx, nMass, directionX, directionY, ackCommand, finalPointX, finalPointY, Stopped);
	}
}

void BBClientGameManager::CalcBallDelta(unsigned int uid)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		player->CalcBallDelta();
	}
}

void BBClientGameManager::ClientCatchup(unsigned int uid)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		player->clientCatchup();
	}
}

void BBClientGameManager::ClientSyncServerPlayerNode(unsigned int uid, int idx, int fromId, int x, int y, int mass, int cd, int protect, int initStopFrame, int initSpeed, int initDeltaSpeed, int speedX, int speedY)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		BBPlayerNode* node = player->GetPlayerNode(idx);
		if (node)
		{
			node->ClientSyncServerPlayerNode(fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed, speedX,speedY);
		}
	}
}

void BBClientGameManager::CreatePlayerNodeFromServer(unsigned int uid, unsigned int idx, unsigned int fromId, int x, int y, int mass, int cd, int protect, int initStopFrame, int initSpeed, int initDeltaSpeed, int speedX, int speedY)
{
	playerManager.CreatePlayerNodeFromServer(uid, idx, fromId, x, y, mass, cd, protect, initStopFrame, initSpeed, initDeltaSpeed,speedX,speedY);
}

void BBClientGameManager::CreateSplitPlayerNodeFromServer(unsigned int uid, unsigned int idx, unsigned int fromId, int x, int y, int mass, int speedX, int speedY, int cd, int protect) 
{
	playerManager.CreateSplitPlayerNodeFromServer(uid, idx, fromId, x, y, mass, speedX, speedY, cd, protect);
}

void BBClientGameManager::RemovePlayerNodeFromServer(unsigned int uid, unsigned int idx)
{
	playerManager.RemovePlayerNode(uid, idx);
}

void BBClientGameManager::CreatePlayerFromServer(unsigned int uid, unsigned int idx, int directionX, int directionY, int NMass, int nextNodeIdx, int finalPointX, int finalPointY, bool Stopped)
{
	playerManager.CreatePlayerFromServer(uid, idx, directionX, directionY, NMass, nextNodeIdx, finalPointX, finalPointY, Stopped);
}

void BBClientGameManager::AddNewFoodFromServer(int posKey)
{
	foodSpikyManager.ClientAddNewFoodFromServer(posKey);
}

void BBClientGameManager::AddNewSpikyFromServer(unsigned int idx, int posKey, int mass)
{
	foodSpikyManager.ClientAddNewSpikyFromServer(idx, posKey, mass);
}

void BBClientGameManager::AddNewSporeFromServer(unsigned int idx, unsigned int fromId, unsigned int uid, int x, int y, int directionX, int directionY,
	int curSpeed, int deltaSpeed, int initStopFrame, int cd)
{
	sporeManager.AddNewSporeFromServer(idx, fromId, uid, x, y, directionX, directionY, curSpeed, deltaSpeed, initStopFrame, cd);
}

void BBClientGameManager::SyncShootFromServer(int idx, int fromId, unsigned int uid, int speedX, int speedY, int locationX, int locationY)
{
	sporeManager.SyncShootFromServer(idx, fromId, uid, speedX, speedY, locationX, locationY);
}

void BBClientGameManager::RemoveFoodFromServer(int posKey)
{
	foodSpikyManager.RemoveFoodByIdx(posKey);
}

void BBClientGameManager::RemoveSporeFromServer(int sporeIdx)
{
	sporeManager.RemoveSporeByIdx(sporeIdx);
}

bool BBClientGameManager::CanAddPlayerCommand(int uid)
{
	BBPlayer* player = playerManager.GetPlayer(uid);
	if (player)
	{
		return !player->mCommandList.isBacklogged();
	}
	return false;
}

std::vector<int> BBClientGameManager::SimulateEatFoods(std::vector<int> playerIds)
{
	std::vector<int> ret;
	if (!GetCanEatFoodSpiky())
	{
		return ret;
	}
	std::vector<int> tempVec;
	for (int i = 0; i < playerIds.size(); i++)
	{
		unsigned int uid = playerIds[i];
		BBPlayer* player = playerManager.GetPlayer(uid);
		if (player)
		{
			for (auto playerNode : player->vecPlayerNodes)
			{
				tempVec.clear();
				hitManager.GetCanEatFood(playerNode, tempVec);
				for (int j = 0; j < tempVec.size(); j++)
				{
					ret.emplace_back(playerNode->idx);
					ret.emplace_back(tempVec[j]);
				}
			}
		}
	}
	return ret;
}

std::vector<int> BBClientGameManager::SimulateEatSpores(std::vector<int> playerIds)
{
	std::vector<int> ret;
	if (!GetCanEatSpore())
	{
		return ret;
	}
	std::vector<int> tempVec;
	std::vector<int> tempForNodeVec;
	for (int i = 0; i < playerIds.size(); i++)
	{
		unsigned int uid = playerIds[i];
		BBPlayer* player = playerManager.GetPlayer(uid);
		if (player)
		{
			for (auto playerNode : player->vecPlayerNodes)
			{
				tempVec.clear();
				hitManager.GetCanEatNodeOrSpore(playerNode, tempForNodeVec, tempVec);
				for (int j = 0; j < tempVec.size(); j+=2)
				{
					int sporeIdx = tempVec[j];
					Spore* spore = sporeManager.GetSpore(sporeIdx);
					if (spore && spore->initStopFrame == 0)
					{
						ret.emplace_back(playerNode->idx);
						ret.emplace_back(sporeIdx);
					}
					
				}
			}
		}
	}
	return ret;
}

void BBClientGameManager::AutoFillPlayerIdxInRect(int minX, int maxX, int minY, int maxY)
{
	std::vector<int> playerIds = this->GetAllPlayerIdxInRect(minX, maxX, minY, maxY);
	this->SetNeedUpdatePlayers(playerIds);
}
