#include "BBPlayerManager.h"
#include "BBObjectManager.h"
#include "BBGameManager.h"
#include "BBConst.h"
#include<algorithm>
#include "BBMathUtils.h"
#include<math.h>
#include "BBFrameCacheManager.h"
#if defined(_WIN32) && defined(_WINDOWS)
#include "cocos2d.h"
#else
#include<stdarg.h>
#include<sys/time.h>
#endif

bool ComparePlayerNodes(PlayerNode* nodeA, PlayerNode* nodeB)
{
	return nodeA->Idx < nodeB->Idx;
}

BBPlayerManager::BBPlayerManager():
	timeHit(0),
	timeUpdateCirle(0),
	timeMovePlayer(0)
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



void BBPlayerManager::_onNewPlayerNodeGenerate(Player* player, PlayerNode* playerNode, bool addNew)
{
	player->vecPlayerNodes.emplace_back(playerNode);
	std::sort(player->vecPlayerNodes.begin(), player->vecPlayerNodes.end(), ComparePlayerNodes);
	mapPlayNodes.emplace(playerNode->Idx, playerNode);
	gameManager->NodeTree.AddCircleNode(playerNode);

	if (addNew)
	{
		gameManager->frameCacheManager.AddNewPlayerNode(player->uid, playerNode->Idx);
	}
}

/**玩家部分**/
void BBPlayerManager::ServerCreatePlayer(int uid)
{
	if (mapPlayers.find(uid) == mapPlayers.end())
	{
		Player* player = gameManager->objectManager.CreatePlayer(uid);
		mapPlayers.emplace(uid, player);
		ServerCreatePlayerNode(uid);
		PlayerNode* node = player->vecPlayerNodes[0];
		player->ResetPoint(node->Location.x, node->Location.y);
		_KeyFrameUpdatePlayer(player);
	}
	else
	{
		Player* player = mapPlayers[uid];
		if (player->vecPlayerNodes.size() == 0)
		{
			ServerCreatePlayerNode(uid);
			PlayerNode* node = player->vecPlayerNodes[0];
			player->ResetPoint(node->Location.x, node->Location.y);
			_KeyFrameUpdatePlayer(player);
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

void BBPlayerManager::CreatePlayerFromServer(int uid, int directionX, int directionY, int finalX, int finalY, bool isStopped, int NMass)
{
	Player* player = new Player(uid);
	player->NMass = NMass;
	player->Direction.x = directionX;
	player->Direction.y = directionY;
	player->Stopped = isStopped;
	player->FinalPoint.x = finalX;
	player->FinalPoint.y = finalY;
	mapPlayers.emplace(uid, player);
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

void BBPlayerManager::ServerCreatePlayerNode(int uid)
{
	Player* player = mapPlayers[uid];
	if (!player)
	{
		return;
	}
	PlayerNode* newPlayerNode = gameManager->objectManager.CreatePlayerNode(uid, 200);
	int radius = newPlayerNode->GetRadius();
	int x, y;
	gameManager->hitManager.FindFreePlayerNodePos(radius, x, y);
	newPlayerNode->ChangePosition(x, y);

	
	_onNewPlayerNodeGenerate(player, newPlayerNode);
}

Player* BBPlayerManager::GetPlayer(int uid)
{
	auto iter = mapPlayers.find(uid);
	if (iter == mapPlayers.end())
	{
		return NULL;
	}
	return (*iter).second;
}

void BBPlayerManager::RemovePlayerNode(int uid, int nodeIdx, bool removeFromVec)
{
	if (mapPlayNodes.find(nodeIdx) == mapPlayNodes.end())
	{
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
		if (playerNode->GetBallMass() > BBConst::MinSplitMass)
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
		gameManager->frameCacheManager.AddCd(child->Idx, uid);

		if (gameManager->IsServer())
		{
			PlayerNode* newNode = DoPlayerNodeSelfSplit(child);
			gameManager->frameCacheManager.AddCd(newNode->Idx, uid);
			_onNewPlayerNodeGenerate(player, newNode);
		}
		else {
			float childMas = child->GetBallMass() / 2.0f;
			child->ChangeDeltaMass(-childMas);
		}
	}
}

PlayerNode* BBPlayerManager::DoPlayerNodeSelfSplit(PlayerNode* sourceNode)
{
	int childMas = sourceNode->GetBallMass() / 2;
	sourceNode->ChangeDeltaMass(-childMas);
	PlayerNode* newPlayerNode = gameManager->objectManager.CreateSimplePlayerNode();
	newPlayerNode->Uid = sourceNode->Uid;
	if (sourceNode->Direction == BBVector::ZERO) {
		newPlayerNode->Direction = BBVector::GetFixedVetor2(BBVector(1, 0), BBMathUtils::Mass2Speed(childMas));
	}
	else {
		newPlayerNode->Direction = sourceNode->Direction;
	}
    newPlayerNode->SetBallMass(childMas);
	BBVector moveVec = BBVector::GetFixedVetor2(newPlayerNode->Direction, sourceNode->GetRadius());
	newPlayerNode->ChangePosition(sourceNode->Location.x + moveVec.x, sourceNode->Location.y + moveVec.y);
	newPlayerNode->FromId = sourceNode->Idx;
	newPlayerNode->Init = BBConst::SplitFrame;
	newPlayerNode->Current = newPlayerNode->Direction;
	newPlayerNode->CalculateInitMoveParams(newPlayerNode->GetRadius(), BBConst::SplitFrame, BBConst::SplitInitSpeed, BBConst::SplitFinalSpeed);
	return newPlayerNode;
}

int BBPlayerManager::DoPlayerNodeSpikySplit(PlayerNode* sourceNode, int maxChildNode, int spikyMass)
{
	if (sourceNode->GetBallMass() < spikyMass)
	{
		return 0;
	}
	if (maxChildNode == 0)
	{
		sourceNode->ChangeDeltaMass(spikyMass);
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
	int centerMass = sourceNode->GetBallMass() + spikyMass - childMass * maxChildNode;
	sourceNode->ChangeDeltaMass(centerMass - sourceNode->GetBallMass());
	if (!gameManager->IsServer())
	{
		return maxChildNode;
	}
	int splitAngle = ceilf(360.0f / maxChildNode);
	int directionAngle = sourceNode->Direction.GetAngle();
	for (int i = 0; i < maxChildNode; i++)
	{
		PlayerNode* newPlayerNode = gameManager->objectManager.CreateSimplePlayerNode();
		newPlayerNode->Uid = sourceNode->Uid;
		newPlayerNode->Cd = 0;
        newPlayerNode->SetBallMass(childMass);
		newPlayerNode->Init = BBConst::SplitFrame;
		newPlayerNode->Direction = BBMathUtils::AngleToFixedVector(directionAngle + splitAngle * i, BBMathUtils::Mass2Speed(childMass));

		BBVector moveVec = BBVector::GetFixedVetor2(newPlayerNode->Direction, sourceNode->GetRadius());
		newPlayerNode->ChangePosition(sourceNode->Location.x + moveVec.x, sourceNode->Location.y + moveVec.y);
		newPlayerNode->FromId = sourceNode->Idx;
		newPlayerNode->CalculateInitMoveParams(newPlayerNode->GetRadius(), BBConst::SplitFrame, BBConst::SplitInitSpeed, BBConst::SplitFinalSpeed);

		gameManager->frameCacheManager.AddCd(newPlayerNode->Idx, newPlayerNode->Uid);
		_onNewPlayerNodeGenerate(player, newPlayerNode);
	}
	return maxChildNode;
}

void BBPlayerManager::AddNewBallFromServer(int idx, int fromId, int uid, int mass, int x, int y, int directionX, int directionY, int currentX, int currentY, int curSpeed, int deltaSpeed, int Init, int Cd, int Protect)
{
	PlayerNode* newPlayerNode = new PlayerNode();
	newPlayerNode->Uid = uid;
	newPlayerNode->Idx = idx;
	newPlayerNode->Cd = Cd;
	newPlayerNode->FromId = fromId;
    newPlayerNode->SetBallMass(mass);
	newPlayerNode->Init = Init;
	newPlayerNode->Protect = Protect;
	newPlayerNode->Current.SetPoint(currentX, currentY);
	newPlayerNode->Direction.SetPoint(directionX, directionY);
	newPlayerNode->ChangePosition(x, y);
	newPlayerNode->initSpeed = curSpeed;
	newPlayerNode->initDeltaSpeed = deltaSpeed;
	Player* player = GetPlayer(uid);
	//从lua层来主动创建的node不放入到新的玩家节点列表中
	_onNewPlayerNodeGenerate(player, newPlayerNode, false);
}

void BBPlayerManager::AddPlayerSplitNewBallFromServer(int idx, int fromId, int uid, int mass, int x, int y, int directionX, int directionY, int currentX, int currentY, int curSpeed, int deltaSpeed, int Init, int Cd, int Protect)
{
	PlayerNode* newPlayerNode = new PlayerNode();
	newPlayerNode->Uid = uid;
	newPlayerNode->Idx = idx;
	newPlayerNode->Cd = Cd;
	newPlayerNode->FromId = fromId;
	newPlayerNode->SetBallMass(mass);
	newPlayerNode->Init = Init;
	newPlayerNode->Protect = Protect;
	newPlayerNode->Current.SetPoint(currentX, currentY);
	newPlayerNode->Direction.SetPoint(directionX, directionY);
	newPlayerNode->ChangePosition(x, y);
	newPlayerNode->initSpeed = curSpeed;
	newPlayerNode->initDeltaSpeed = deltaSpeed;
	Player* player = GetPlayer(uid);
	//从lua层来主动创建的node不放入到新的玩家节点列表中
	_onNewPlayerNodeGenerate(player, newPlayerNode, false);
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

void BBPlayerManager::_keyFrameChangePlayerFinalPoint(Player* player)
{
	if (!player->Stopped) {
		BBRect rect = player->GetGroupRect();
		int w = rect.maxX - rect.minX;
		int h = rect.maxY - rect.minY;
		int halfLen = (sqrtf(pow(w, 2) + pow(h, 2)) / 2.0f);
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
					node->SetSpeedVec(player->FinalPoint.x - node->Location.x, player->FinalPoint.y - node->Location.y);
					BBMathUtils::BBLOG("key frame change node:%d directionX:%d ,directionY:%d\n", node->Idx, node->Current.x, node->Current.y);
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
				node->SetSpeedVec(player->FinalPoint.x - node->Location.x, player->FinalPoint.y - node->Location.y);
			}
		}
	}
	if (player->Direction.y == 0)
	{
		player->Stopped = true;
	}
}

bool BBPlayerManager::_handleNodeHit(Player* player, PlayerNode* node, BBPoint& locVec)
{
	bool ret = false;
	std::vector<PlayerNode*>& allPlayerNodes = player->vecPlayerNodes;
	for (int j = 0; j < allPlayerNodes.size(); j++)
	{
		PlayerNode* ballB = allPlayerNodes[j];
		if (ballB->Init <= 0 && ballB != node)
		{
			int deltaX = ballB->Location.x - locVec.x;
			int deltaY = ballB->Location.y - locVec.y;

			int distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
			int totalCircle = ballB->GetRadius() + node->GetRadius();
			if (totalCircle > distance)
			{
				int length = totalCircle - distance;
				float deltaLen = (float)BBConst::Delta / BBConst::DeltaBase;
				if (node->Cd > 0 || ballB->Cd > 0 || BBMathUtils::NeedRollback(*node, *ballB, deltaLen))
				{
					BBVector fixVec(locVec.x - ballB->Location.x, locVec.y - ballB->Location.y);
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
				ret = true;
			}
		}
	}
	return ret;
}

void BBPlayerManager::_moveNode(Player* player, PlayerNode* node, BBPoint& locVec)
{
	BBPoint& finalPoint = player->FinalPoint;
	int lastX = node->Location.x;
	int lastY = node->Location.y;
	if (node->Init > 0)
	{
		//BBVector before = node->Current;
		node->InitMove();
		//BBMathUtils::BBLOG("node %d ball init move beforedir = %f-%f, dir = %f-%f speed is: %d", node->Idx, before.x, before.y, node->Current.x, node->Current.y, node->initSpeed);
	}

	if (node->Current != BBVector::ZERO)
	{
		locVec.x += node->Current.x;
		locVec.y += node->Current.y;
		if (node->Init > 0)
		{
			return;
		}
		if (lastX <= finalPoint.x && locVec.x >= finalPoint.x)
		{
			locVec.x = finalPoint.x;
		}
		if (lastX >= finalPoint.x && locVec.x <= finalPoint.x)
		{
			locVec.x = finalPoint.x;
		}

		if (lastY <= finalPoint.y && locVec.y >= finalPoint.y)
		{
			locVec.y = finalPoint.y;
		}
		if (lastY >= finalPoint.y && locVec.y <= finalPoint.y)
		{
			locVec.y = finalPoint.y;
		}
	}
}

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

//玩家移动
void BBPlayerManager::MovePlayer(Player* player)
{
	std::vector<PlayerNode*>& allPlayerNodes = player->vecPlayerNodes;
	if (allPlayerNodes.size() == 0)
	{
		player->Direction = BBVector::ZERO;
		player->Stopped = true;
		return;
	}
	
	BBMathUtils::BBLOG("current target point %d-%d\n", player->FinalPoint.x, player->FinalPoint.y);
	bool isBalance = true;
	bool isAllHit = true;
	for (int i = 0; i < allPlayerNodes.size(); i++)
	{
		PlayerNode* node = allPlayerNodes[i];
		BBPoint locVec = node->Location;
		_moveNode(player, node, locVec);
		if (node->Init == 0 && allPlayerNodes.size() > 1)
		{
			bool isHit = _handleNodeHit(player, node, locVec);
			if (isHit)
			{
				BBMathUtils::BBLOG("node %d happend hit from position from %d-%d  to %d-%d\n", node->Idx, node->Location.x, node->Location.y, locVec.x, locVec.y);
			}
			else {
				isAllHit = false;
			}
		}

		bool isFixedX, isFixedY;
		int fixedX, fixedY;
		BBMathUtils::FixCircle(gameManager->mapRect, locVec.x, locVec.y, node->GetRadius(), fixedX, fixedY, isFixedX, isFixedY);
		if (node->Current != BBVector::ZERO)
		{
			BBMathUtils::BBLOG("node %d change position from %d-%d  to %d-%d\n", node->Idx, node->Location.x, node->Location.y, fixedX, fixedY);
			BBMathUtils::BBLOG("node %d Current is %f-%f, init is %d cd is %d\n", node->Idx, node->Current.x, node->Current.y, node->Init, node->Cd);
		}
		
		int distance = (fixedX - node->Location.x) * (fixedX - node->Location.x) + (fixedY - node->Location.y) * (fixedY - node->Location.y);
		if (distance > 2)
		{
			isBalance = false;
		}
		node->ChangePosition(fixedX, fixedY);
		gameManager->NodeTree.UpdateCircleNode(node);
	}
}

void BBPlayerManager::_KeyFrameUpdatePlayer(Player* player)
{
	_keyFrameChangePlayerFinalPoint(player);
	for (int i = 0; i < player->vecPlayerNodes.size(); i++)
	{
		PlayerNode* node = player->vecPlayerNodes[i];
		if (node->Init > 0)
		{
			node->Init -= 5;
			if (node->Init == 0)
			{
				node->SetSpeedVec(player->FinalPoint.x - node->Location.x, player->FinalPoint.y - node->Location.y);
			}
		}
		if (node->Cd > 0)
		{
			node->Cd -= 5;
		}
		if (node->Protect > 0)
		{
			node->Protect -= 5;
		}
	}
}

void BBPlayerManager::KeyFrameUpdatePlayer()
{
	for (int i = 0; i < playerIds.size(); i++)
	{
		auto iter = mapPlayers.find(playerIds[i]);
		if (iter != mapPlayers.end())
		{
			_KeyFrameUpdatePlayer((*iter).second);
		}
	}
}

void BBPlayerManager::MovePlayers()
{
	timeHit = 0;
	timeUpdateCirle = 0;
	timeMovePlayer = 0;
	for (int i = 0; i < playerIds.size(); i++)
	{
		auto iter = mapPlayers.find(playerIds[i]);
		if (iter != mapPlayers.end())
		{
			MovePlayer((*iter).second);
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
			if (playerNode->GetBallMass() >= BBConst::ShootMinMass)
			{
				playerNode->ChangeDeltaMass(-1 * BBConst::SporeMass);

				//暂时孢子由服务器生成,客户端不在这里生成孢子.
				if (gameManager->IsServer())
				{
					gameManager->sporeManager.CreateSpore(playerNode);
				}
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
			gameManager->frameCacheManager.AddEatenFood(vec[i], vec[i + 1]);
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
			gameManager->frameCacheManager.AddEatenSpiKy(vec[i], vec[i + 1],vec[i + 2]);
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
					deltaMass += allFoodEatInfos[node->Idx].size() * BBConst::FoodMass;
				}

				if (allSporeEatInfos.find(node->Idx) != allSporeEatInfos.end())
				{
					deltaMass += allSporeEatInfos[node->Idx].size() * BBConst::SporeMass;
				}

				if (deltaMass != 0)
				{
					int oldMass = node->GetBallMass();
					if (node->GetBallMass() + deltaMass < BBConst::InitMass)
					{
						deltaMass = BBConst::InitMass - node->GetBallMass();
					}
					node->ChangeDeltaMass(deltaMass);
					BBMathUtils::BBLOG("ball eat other mass change: %d-->%d\n", oldMass, node->GetBallMass());
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
				massMap[key] += node->GetBallMass();
			}
		}
	}

	for (auto iter : massMap)
	{
		int eatNodeIdx = iter.first;
		int changeMass = iter.second;
		PlayerNode* node = mapPlayNodes[eatNodeIdx];
		if (node->GetBallMass() + changeMass < BBConst::InitMass)
		{
			changeMass = BBConst::InitMass - node->GetBallMass();
		}
		node->ChangeDeltaMass(changeMass);
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

void BBPlayerManager::DoSpikySplit()
{
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
						std::vector<int>& vec = iter->second;
						if (vec.size() > 1)
						{
							int splitNum = DoPlayerNodeSpikySplit(playerNode, canSplit, vec[1]);
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
}

void BBPlayerManager::DoPlayerSplit()
{
	auto& splitOperates = gameManager->frameCacheManager.splitOperates;
	if (splitOperates.size() > 0)
	{
		for (auto& iter : splitOperates)
		{
			PlayerSplit(iter.first);
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
		ServerCreatePlayer(uid);
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

		BBMathUtils::BBLOG("step move uid:%d angle:%d, pressure:%d\n", uid, angle, pressure);
		if (mapPlayers.find(uid) != mapPlayers.end())
		{
			Player* player = mapPlayers[uid];
			player->Direction.SetPoint(angle, pressure);
			BBMathUtils::BBLOG("change Direction uid:%d directionX:%d, directionY:%d\n", uid, player->Direction.x, player->Direction.y);
			if (pressure != 0)
			{
				player->Stopped = false;
			}
		}
	}
}

unsigned int BBPlayerManager::GetAllPlayerCrc()
{
	static unsigned int gameBuffer[50 * 4];
	unsigned int index = 0;

	for (int i = 0; i < playerIds.size(); i++)
	{
		int playerId = playerIds[i];
		Player* player = GetPlayer(playerId);
		if (player)
		{	
			gameBuffer[index++] = player->GetCrc();
		}
	}
	unsigned int size = index * sizeof(int) / sizeof(char);
	unsigned int ret = BBMathUtils::crc32((char*)(&gameBuffer), size);
	return ret;
}