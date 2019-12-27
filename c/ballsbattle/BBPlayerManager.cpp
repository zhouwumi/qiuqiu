#include "BBPlayerManager.h"
#include "BBObjectManager.h"
#include "BBGameManager.h"
#include "BBConst.h"
#include<algorithm>
#include "BBMathUtils.h"
#include "BBFrameCacheManager.h"

bool ComparePlayerNodes(PlayerNode* nodeA, PlayerNode* nodeB)
{
	return nodeA->Idx < nodeB->Idx;
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



void BBPlayerManager::_onNewPlayerNodeGenerate(Player* player, PlayerNode* playerNode)
{
	player->vecPlayerNodes.emplace_back(playerNode);
	std::sort(player->vecPlayerNodes.begin(), player->vecPlayerNodes.end(), ComparePlayerNodes);
	mapPlayNodes.emplace(playerNode->Idx, playerNode);
	gameManager->NodeTree.AddCircleNode(playerNode);

	gameManager->frameCacheManager.AddNewPlayerNode(player->uid, playerNode->Idx);
}

/**玩家部分**/
void BBPlayerManager::CreatePlayer(int uid)
{
	if (mapPlayers.find(uid) == mapPlayers.end())
	{
		Player* player = gameManager->objectManager.CreatePlayer(uid);
		mapPlayers.emplace(uid, player);
		CreatePlayerNode(uid);
		PlayerNode* node = player->vecPlayerNodes[0];
		player->ResetPoint(node->Location.x, node->Location.y);
	}
	else
	{
		Player* player = mapPlayers[uid];
		if (player->vecPlayerNodes.size() == 0)
		{
			player->Respawn = 0;
			CreatePlayerNode(uid);
			PlayerNode* node = player->vecPlayerNodes[0];
			player->ResetPoint(node->Location.x, node->Location.y);
		}
	}
	gameManager->frameCacheManager.AddNewPlayer(uid);
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


void BBPlayerManager::CreatePlayerNode(int uid)
{
	Player* player = mapPlayers[uid];
	if (!player)
	{
		//assert("add player node but player is not : map");
		return;
	}
	PlayerNode* newPlayerNode = gameManager->objectManager.CreatePlayerNode(uid, BBConst::InitMass);
	int radius = newPlayerNode->radius;
	int x, y;
	gameManager->hitManager.FindFreePlayerNodePos(radius, x, y);
	newPlayerNode->ChangePosition(x, y);
	newPlayerNode->FromLocation = newPlayerNode->Location;

	_onNewPlayerNodeGenerate(player, newPlayerNode);
}

void BBPlayerManager::RemovePlayer(int uid)
{
	Player* player = mapPlayers[uid];
	if (!player)
	{
		return;
	}

	for (int i = player->vecPlayerNodes.size() - 1; i >= 0; i--)
	{
		RemovePlayerNode(uid, player->vecPlayerNodes[i]->Idx);
	}
	for (int i = 0; i < playerIds.size(); i++)
	{
		if (playerIds[i] == uid)
		{
			int last = playerIds.size() - 1;
			int temp = playerIds[last];
			playerIds[last] = uid;
			playerIds[i] = temp;
			playerIds.pop_back();
			break;
		}
	}
	std::sort(playerIds.begin(), playerIds.end());

	player->vecPlayerNodes.clear();
	mapPlayers.erase(uid);
	delete player;
}

void BBPlayerManager::RemovePlayerNode(int uid, int nodeIdx, bool removeFromVec)
{
	if (mapPlayNodes.find(nodeIdx) == mapPlayNodes.end())
	{
		//assert("remove player node but not : map");
		return;
	}
	PlayerNode* node = mapPlayNodes[nodeIdx];
	mapPlayNodes.erase(nodeIdx);
	gameManager->NodeTree.RemoveCircleNode(node);

	if (removeFromVec)
	{
		Player* player = mapPlayers[uid];
		if (!player)
		{
			return;
		}
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

//玩家分裂
void BBPlayerManager::PlayerSplit(int uid)
{
	if (mapPlayers.find(uid) == mapPlayers.end())
	{
		//assert("player split but not : map");
		return;
	}
	Player* player = mapPlayers[uid];
	if (player->vecPlayerNodes.size() >= BBConst::MaxCell)
	{
		return;
	}
	int curentCount = player->vecPlayerNodes.size();
	std::vector<PlayerNode*> splitPlayerNodes;
	for (auto playerNode : player->vecPlayerNodes)
	{
		if (playerNode->Mass() > BBConst::MinSplitMass)
		{
			splitPlayerNodes.emplace_back(playerNode);
		}
	}
	std::sort(splitPlayerNodes.begin(), splitPlayerNodes.end(), ComparePlayerNodes);
	std::vector<int> selectedNodes;
	for (int i = 0; i < splitPlayerNodes.size(); i++)
	{
		selectedNodes.emplace_back(i);
		curentCount++;
		if (curentCount == BBConst::MaxCell)
		{
			break;
		}
	}
	for (int idx : selectedNodes)
	{
		PlayerNode* child = splitPlayerNodes[idx];
		PlayerNode* newNode = DoPlayerNodeSelfSplit(child);
		gameManager->frameCacheManager.AddCd(newNode->Idx, newNode->Uid);
		gameManager->frameCacheManager.AddCd(child->Idx, newNode->Uid);
		_onNewPlayerNodeGenerate(player, newNode);
	}
}

PlayerNode* BBPlayerManager::DoPlayerNodeSelfSplit(PlayerNode* sourceNode)
{
	float childMas = sourceNode->mass / 2.0f;
	sourceNode->ChangeMass(-childMas);
	PlayerNode* newPlayerNode = gameManager->objectManager.CreateSimplePlayerNode();
	newPlayerNode->Uid = sourceNode->Uid;
	if (sourceNode->Direction == BBVector::ZERO) {
		newPlayerNode->Direction = BBVector::GetFixedVetor2(BBVector(1, 0), BBMathUtils::Mass2Speed(childMas));
	}
	else {
		newPlayerNode->Direction = sourceNode->Direction;
	}
	newPlayerNode->SetMass(childMas);
	BBVector moveVec = BBVector::GetFixedVetor2(newPlayerNode->Direction, sourceNode->radius);
	newPlayerNode->FromLocation.x = sourceNode->Location.x + moveVec.x;
	newPlayerNode->FromLocation.y = sourceNode->Location.y + moveVec.y;
	newPlayerNode->ChangePosition(newPlayerNode->FromLocation.x, newPlayerNode->FromLocation.y);
	newPlayerNode->FromId = sourceNode->Idx;
	newPlayerNode->Init = BBConst::SplitFrame;
	newPlayerNode->Current = newPlayerNode->Direction;
	newPlayerNode->CalculateInitMoveParams(newPlayerNode->GetRadius(), BBConst::SplitFrame, BBConst::SplitInitSpeed, BBConst::SplitFinalSpeed);
	return newPlayerNode;
}

int BBPlayerManager::DoPlayerNodeSpikySplit(PlayerNode* sourceNode, int maxChildNode, int spikyMass)
{
	if (sourceNode->mass < spikyMass)
	{
		return 0;
	}
	if (maxChildNode == 0)
	{
		sourceNode->ChangeMass(spikyMass);
		return 0;
	}
	int uid = sourceNode->Uid;
	if (mapPlayers.find(uid) == mapPlayers.end())
	{
		return 0;
	}
	Player* player = mapPlayers[uid];
	int assignMass = 2 * spikyMass;
	int childMass = assignMass / (maxChildNode + 1);
	if (childMass > 40)
	{
		childMass = 40;
	}
	int centerMass = sourceNode->mass + spikyMass - childMass * maxChildNode;
	sourceNode->ChangeMass(centerMass - sourceNode->mass);
	int splitAngle = ceilf(360.0f / maxChildNode);
	int directionAngle = sourceNode->Direction.GetAngle();
	for (int i = 0; i < maxChildNode; i++)
	{
		PlayerNode* newPlayerNode = gameManager->objectManager.CreateSimplePlayerNode();
		newPlayerNode->Uid = sourceNode->Uid;
		newPlayerNode->Cd = 0;
		newPlayerNode->SetMass(childMass);
		newPlayerNode->Init = BBConst::SplitFrame;
		newPlayerNode->Direction = BBMathUtils::AngleToFixedVector(directionAngle, BBMathUtils::Mass2Speed(childMass));

		BBVector moveVec = BBVector::GetFixedVetor2(newPlayerNode->Direction, sourceNode->radius);
		newPlayerNode->FromLocation.x = sourceNode->Location.x + moveVec.x;
		newPlayerNode->FromLocation.y = sourceNode->Location.y + moveVec.y;
		newPlayerNode->ChangePosition(newPlayerNode->FromLocation.x, newPlayerNode->FromLocation.y);
		newPlayerNode->FromId = sourceNode->Idx;
		newPlayerNode->CalculateInitMoveParams(newPlayerNode->GetRadius(), BBConst::SplitFrame, BBConst::SplitInitSpeed, BBConst::SplitFinalSpeed);

		gameManager->frameCacheManager.AddCd(newPlayerNode->Idx, newPlayerNode->Uid);
		_onNewPlayerNodeGenerate(player, newPlayerNode);
	}
	return maxChildNode;
}

void BBPlayerManager::RemoveMass()
{
	for (int i = 0; i < playerIds.size(); i++)
	{
		int playerId = playerIds[i];
		auto iter = mapPlayers.find(playerId);
		if (iter != mapPlayers.end())
		{
			Player* player = (*iter).second;
			if (player->vecPlayerNodes.size() > 0)
			{
				player->RemoveMass();
			}
		}
	}
}

//玩家移动
void BBPlayerManager::MovePlayer(Player* player, bool isKeyFrame)
{

	std::vector<PlayerNode*>& allPlayerNodes = player->vecPlayerNodes;
	if (allPlayerNodes.size() == 0)
	{
		player->Direction = BBVector::ZERO;
		player->Stopped = true;
		return;
	}
	if (isKeyFrame)
	{
		if (!player->Stopped) {
			BBRect rect = player->GetGroupRect();
			int w = rect.maxX - rect.minX;
			int h = rect.maxY - rect.minY;
			int halfLen = (sqrtf(pow(w, 2) + pow(h, 2)) / 2.0f);
			player->RectCenter.SetPoint(rect.centerX, rect.centerY);
			int baseLen = halfLen * BBConst::MoveRate / BBConst::MoveBase;
			if (baseLen < BBConst::FixLength) {
				baseLen = BBConst::FixLength;
			}
			int extLen = baseLen * BBMathUtils::PressureToPercent(player->Direction.y);
			BBVector targetVec = BBMathUtils::AngleToFixedVector(int(player->Direction.x / 10.0f), extLen);
			if (targetVec == BBVector::ZERO && player->vecPlayerNodes.size() == 1)
			{
				for (int i = 0; i < player->vecPlayerNodes.size(); i++)
				{
					PlayerNode* node = player->vecPlayerNodes[i];
					node->Current = BBVector::ZERO;
					node->FromLocation = node->Location;
					player->UpdateFinalPoint(node->Location.x, node->Location.y);
				}
			}
			else
			{
				BBPoint point;
				point.SetPoint(int(rect.centerX + targetVec.x), int(rect.centerY + targetVec.y));
				player->UpdateFinalPoint(point.x, point.y);
				for (int i = 0; i < player->vecPlayerNodes.size(); i++)
				{
					PlayerNode* node = player->vecPlayerNodes[i];
					if (node->Init == 0)
					{
						node->SetSpeedVec(player->FinalPoint.x - node->positionX, player->FinalPoint.y - node->positionY);
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < player->vecPlayerNodes.size(); i++)
			{
				PlayerNode* node = player->vecPlayerNodes[i];
				if (node->Init == 0)
				{
					node->SetSpeedVec(player->FinalPoint.x - node->positionX, player->FinalPoint.y - node->positionY);
				}
			}
		}
		if (player->Direction.y == 0)
		{
			player->Stopped = true;
		}
	}
	else
	{
		player->UpdateFinalPoint(player->FinalPoint.x, player->FinalPoint.y);
	}

	//player->UpdateFinalPoint(player->FinalPoint.x, player->FinalPoint.y);
	BBPoint& finalPoint = player->FinalPoint;

	for (int i = 0; i < allPlayerNodes.size(); i++)
	{
		PlayerNode* node = allPlayerNodes[i];
		BBPoint locVec = node->Location;
		node->FromLocation = node->Location;
		int oldX = locVec.x;
		int oldY = locVec.y;
		if (node->Init > 0)
		{
			BBVector moveVec = node->InitMove();
			locVec.x += moveVec.x;
			locVec.y += moveVec.y;
		}
		else
		{
			if (node->Current != BBVector::ZERO)
			{
				locVec.x += node->Current.x;
				locVec.y += node->Current.y;
				if (node->FromLocation.x <= finalPoint.x && locVec.x >= finalPoint.x)
				{
					locVec.x = finalPoint.x;
				}
				if (node->FromLocation.x >= finalPoint.x && locVec.x <= finalPoint.x)
				{
					locVec.x = finalPoint.x;
				}

				if (node->FromLocation.y <= finalPoint.y && locVec.y >= finalPoint.y)
				{
					locVec.y = finalPoint.y;
				}
				if (node->FromLocation.y >= finalPoint.y && locVec.y <= finalPoint.y)
				{
					locVec.y = finalPoint.y;
				}
			}
		}

		if (node->Init <= 0 && allPlayerNodes.size() > 1)
		{
			for (int j = 0; j < allPlayerNodes.size(); j++)
			{
				PlayerNode* ballB = allPlayerNodes[j];
				if (ballB->Init <= 0 && ballB != node)
				{
					int deltaX = ballB->positionX - locVec.x;
					int deltaY = ballB->positionY - locVec.y;

					int distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
					int totalCircle = ballB->radius + node->radius;
					if (totalCircle > distance)
					{
						int length = totalCircle - distance;
						float deltaLen = (float)BBConst::Delta / BBConst::DeltaBase;
						if (node->Cd > 0 || ballB->Cd > 0 || BBMathUtils::NeedRollback(*node, *ballB, deltaLen))
						{
							BBVector fixVec(locVec.x - ballB->positionX, locVec.y - ballB->positionY);
							BBVector vec = BBVector::GetFixedVetor2(fixVec, length);
							BBPoint oldVec = locVec;
							if (locVec.x <= ballB->Location.x)
							{
								locVec.x = locVec.x - abs(vec.x);
							}
							else if (locVec.x > ballB->Location.x)
							{
								locVec.x = locVec.x + abs(vec.x);
							}

							if (locVec.y <= ballB->Location.y)
							{
								locVec.y = locVec.y - abs(vec.y);
							}
							else if (locVec.y > ballB->Location.y)
							{
								locVec.y = locVec.y + abs(vec.y);
							}
						}
					}
				}
			}
		}
		

		bool isFixedX, isFixedY;
		int fixedX, fixedY;
		BBMathUtils::FixCircle(gameManager->mapRect, locVec.x, locVec.y, node->radius, fixedX, fixedY, isFixedX, isFixedY);
		node->ChangePosition(fixedX, fixedY);

		gameManager->NodeTree.UpdateCircleNode(node);
	}

	for (int i = 0; i < player->vecPlayerNodes.size(); i++)
	{
		PlayerNode* node = player->vecPlayerNodes[i];
		if (node->Init > 0)
		{
			node->Init -= 1;
			if (node->Init == 0)
			{
				node->SetSpeedVec(player->FinalPoint.x - node->Location.x, player->FinalPoint.y - node->Location.y);
				node->Delta = BBVector::ZERO;
				node->Final = node->Current;
			}
		}
		if (node->Cd > 0)
		{
			node->Cd -= 1;
		}
		if (node->Protect > 0)
		{
			node->Protect -= 1;
		}
	}
}

void BBPlayerManager::MovePlayers(bool isKeyFrame)
{
	for (int i = 0; i < playerIds.size(); i++)
	{
		auto iter = mapPlayers.find(playerIds[i]);
		if (iter != mapPlayers.end())
		{
			MovePlayer((*iter).second, isKeyFrame);
		}
	}
}

void BBPlayerManager::DoShoot()
{
	if (gameManager->frameCacheManager.playerShootOperates.size() <= 0)
	{
		return;
	}
	for (auto iter : gameManager->frameCacheManager.playerShootOperates)
	{
		int uid = iter.first;
		if (mapPlayers.find(uid) == mapPlayers.end())
		{
			continue;
		}
		Player* player = mapPlayers[uid];
		if (player->vecPlayerNodes.size() == 0)
		{
			continue;
		}
		for (int i = 0; i < player->vecPlayerNodes.size(); i++)
		{
			PlayerNode* playerNode = player->vecPlayerNodes[i];
			if (playerNode->mass >= BBConst::ShootMinMass)
			{
				playerNode->ChangeMass(-1 * BBConst::SporeMass);
				gameManager->sporeManager.CreateSpore(playerNode);
				
			}
		}
	}
}

void BBPlayerManager::DoEat(std::vector<int>& eatResults)
{
	for (int i = 0; i < playerIds.size(); i++)
	{
		auto iter = mapPlayers.find(playerIds[i]);
		if (iter != mapPlayers.end())
		{
			Player* player = (*iter).second;
			for (PlayerNode* node : player->vecPlayerNodes)
			{
				DoEat(node, eatResults);
			}
		}
	}
}

void BBPlayerManager::DoEat(PlayerNode* node, std::vector<int>& eatResults)
{
	std::vector<int> vec;
	gameManager->hitManager.GetCanEatFood(node, vec);
	if (vec.size() > 0)
	{
		for (int i = 0; i < vec.size(); i += 2)
		{
			gameManager->frameCacheManager.AddEatenFood(vec[i]);
			gameManager->foodSpikyManager.RemoveFoodByIdx(vec[i + 1]);
			_eat(eatResults, Type_Food, vec[i + 1], Type_PlayerNode, vec[i]);
		}
		vec.clear();
	}
	gameManager->hitManager.GetCanEatSpiky(node, vec);
	if (vec.size() > 0)
	{
		for (int i = 0; i < vec.size(); i += 3)
		{
			_eat(eatResults, Type_Spiky, vec[i + 1], Type_PlayerNode, vec[i]);
			gameManager->frameCacheManager.AddEatenSpiKy(vec[i], vec[i + 2]);
			gameManager->foodSpikyManager.SpikyBeEat(vec[i + 1]);
		}
		vec.clear();
	}
	
	std::vector<int> sporeVec;
	gameManager->hitManager.GetCanEatNodeOrSpore(node, vec, sporeVec);
	if (sporeVec.size() > 0)
	{
		for (int i = 0; i < sporeVec.size(); i += 3)
		{
			_eat(eatResults, Type_Spore, sporeVec[i + 1], Type_PlayerNode, sporeVec[i]);

			gameManager->frameCacheManager.AddEatenSpores(sporeVec[i], sporeVec[i + 1]);
			gameManager->sporeManager.RemoveSporeByIdx(sporeVec[i + 1]);
		}
		sporeVec.clear();
	}
	if (vec.size() > 0 || sporeVec.size()> 0)
	{
		int temp = 1;
		//for (int i = 0; i < vec.size(); i += 3)
		//{
		//	//_eat(eatResults, Type_Spiky, vec[i + 1], Type_PlayerNode, vec[i]);
		//	/*gameManager->frameCacheManager.AddEatenSpiKy(vec[i], vec[i + 2]);
		//	gameManager->foodSpikyManager.SpikyBeEat(vec[i + 1]);*/
		//}
		//vec.clear();
	}
}

void BBPlayerManager::FinishEat()
{
	FinishEatChangeMass();
	FinishEatReleate();
}

void BBPlayerManager::FinishEatChangeMass()
{
	auto allFoodEatInfos = gameManager->frameCacheManager.FoodEeaten;
	auto allSporeEatInfos = gameManager->frameCacheManager.SporesEaten;

	for (int i = 0; i < playerIds.size(); i++)
	{
		auto iter = mapPlayers.find(playerIds[i]);
		if (iter != mapPlayers.end())
		{
			Player* player = (*iter).second;
			for (PlayerNode* node : player->vecPlayerNodes)
			{
				int deltaMass = 0;
				
				if (allFoodEatInfos.find(node->Idx) != allFoodEatInfos.end())
				{
					deltaMass += allFoodEatInfos[node->Idx] * BBConst::FoodMass;
				}

				if (allSporeEatInfos.find(node->Idx) != allSporeEatInfos.end())
				{
					deltaMass += allSporeEatInfos[node->Idx].size() * BBConst::SporeMass;
				}

				if (deltaMass != 0)
				{
					if (node->mass + deltaMass < BBConst::InitMass)
					{
						deltaMass = BBConst::InitMass - node->mass;
					}
					node->ChangeMass(deltaMass);
				}
			}
		}
	}
}

void BBPlayerManager::FinishEatReleate()
{
	auto& removeNodes = gameManager->frameCacheManager.RemovedNodes;
	std::unordered_map<int, int> fillMap;
	for(auto& iter : removeNodes)
	{
		int beEatNodeIdx = iter.first;
		int eatNodeIdx = iter.second;
		int finalIdx = eatNodeIdx;
		while (removeNodes.find(finalIdx) != removeNodes.end())
		{
			finalIdx = removeNodes[finalIdx];
		}
		fillMap[beEatNodeIdx] = finalIdx;
	}
	auto& nodesEaten = gameManager->frameCacheManager.NodesEaten;
	std::unordered_map<int, int> massMap;
	for (auto& iter : nodesEaten)
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
		for(auto beEatId : iter.second)
		{
			PlayerNode* node = mapPlayNodes[beEatId];
			if (node)
			{
				massMap[key] += node->mass;
			}
		}
	}

	for (auto iter : massMap)
	{
		int eatNodeIdx = iter.first;
		int changeMass = iter.second;
		PlayerNode* node = mapPlayNodes[eatNodeIdx];
		if (node->mass + changeMass < BBConst::InitMass)
		{
			changeMass = BBConst::InitMass - node->mass;
		}
		node->ChangeMass(changeMass);
	}

	for (auto& iter : nodesEaten)
	{
		for(auto beEatId : iter.second)
		{
			PlayerNode* node = mapPlayNodes[beEatId];
			if (node)
			{
				gameManager->frameCacheManager.AddFrameRemovePlayerNodeIdxs(node->Idx, node->Uid);
				RemovePlayerNode(node->Uid, node->Idx);
			}
		}
	}

	//....
}

void BBPlayerManager::DoSplit()
{
	auto& splitOperates = gameManager->frameCacheManager.splitOperates;
	if (splitOperates.size() > 0)
	{
		for (int i = 0; i < splitOperates.size(); i++)
		{
			int uid = splitOperates[i].uid;
			PlayerSplit(uid);
		}
	}
	if (gameManager->frameCacheManager.SpikysEaten.size() > 0)
	{
		for (int i = 0; i < playerIds.size(); i++)
		{
			auto iter = mapPlayers.find(playerIds[i]);
			if (iter != mapPlayers.end())
			{
				Player* player = (*iter).second;
				int totalNum = player->vecPlayerNodes.size();
				int curNum = totalNum;
				for (int i = 0; i < curNum; i++)
				{
					PlayerNode* playerNode = player->vecPlayerNodes[i];
					auto iter = gameManager->frameCacheManager.SpikysEaten.find(playerNode->Idx);
					if (iter != gameManager->frameCacheManager.SpikysEaten.end())
					{
						int canSplit = BBConst::MaxCell - totalNum;
						if (canSplit > BBConst::SpikyChild)
						{
							canSplit = BBConst::SpikyChild;
						}
						
						int splitNum = DoPlayerNodeSpikySplit(playerNode, canSplit, iter->second);
						totalNum += splitNum;
						if (splitNum > 0)
						{
							gameManager->frameCacheManager.AddCd(playerNode->Idx, playerNode->Uid);
						}
					}
				}
			}
		}
	}
}

void BBPlayerManager::_eat(std::vector<int>& vec, int beEatType, int beEatId, int eatType, int eatId)
{
	vec.emplace_back(beEatType);
	vec.emplace_back(beEatId);
}

void BBPlayerManager::DoJoin()
{
	auto& joinOperates = gameManager->frameCacheManager.joinOperates;
	for (int i = 0; i < joinOperates.size(); i++)
	{
		int uid = joinOperates[i].uid;
		CreatePlayer(uid);
	}
}

void BBPlayerManager::AjustVector()
{
	auto& playerQuitOperates = gameManager->frameCacheManager.playerQuitOperates;
	for (int i = 0; i < playerQuitOperates.size(); i++)
	{
		int uid = playerQuitOperates[i].uid;
		if (mapPlayers.find(uid) != mapPlayers.end())
		{
			Player* player = mapPlayers[uid];
			player->Direction.SetPoint(0, 0);
		}
	}
	
	auto& moveOperates = gameManager->frameCacheManager.moveOperates;
	for (auto iter : moveOperates)
	{
		int uid = iter.second.uid;
		int angle = iter.second.angle;
		int pressure = iter.second.pressure;

		if (mapPlayers.find(uid) != mapPlayers.end())
		{
			Player* player = mapPlayers[uid];
			player->Direction.SetPoint(angle, pressure);
			if (pressure != 0)
			{
				player->Stopped = false;
			}
		}
	}
}