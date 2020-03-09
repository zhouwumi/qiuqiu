#include "BBPlayer.h"
#include "BBPlayerNode.h"
#include<math.h>
#include "BBMathUtils.h"

#include "BBGameManager.h"
#include "BBMoveManager.h"
#include "BBConfigManager.h"

BBPlayer::BBPlayer(unsigned int id) :
	uid(id),
	Stopped(true),
	lastMoveCommandIdx(-1),
	NMass(0),
	isCatchingUp(false),
	isShoot(false),
	isSplit(false)
{
}

BBPlayer::~BBPlayer()
{
}

unsigned int BBPlayer::GetCrc()
{
	static unsigned int playerBuffer[20 * 4];
	unsigned int index = 0;

	for (int i = 0; i < vecPlayerNodes.size(); i++)
	{
		BBPlayerNode* node = vecPlayerNodes[i];
		playerBuffer[index++] = node->GetCrc();
	}
	unsigned int size = index * sizeof(int) / sizeof(char);
	unsigned int ret = BBMathUtils::crc32((char*)(&playerBuffer), size);
	return ret;
}

unsigned int BBPlayer::GetLuaCrc()
{
	unsigned int ret = 0;
	for (int i = 0; i < vecPlayerNodes.size(); i++)
	{
		BBPlayerNode* node = vecPlayerNodes[i];
		ret += node->GetLuaCrc();
	}
	return ret;
}

bool BBPlayer::isMe()
{
	if (gameManager)
	{
		return gameManager->GetUserId() == uid;
	}
	return false;
}

void BBPlayer::ResetCommands()
{
	lastMoveCommandIdx = -1;
	mCommandList.mCommandVec.clear();
	predictionData.ClearData();
	isCatchingUp = false;
}

bool BBPlayer::IsNeedSyncState()
{
	if (!this->gameManager->IsServer())
	{
		if (this->isMe())
		{
			return false;
		}
		//如果出现不一致的情况,需要进行一次状态同步
		return !predictionData.mPredictionRight;
	}
	if (mCommandList.mControlMismatch)
	{
		return true;
	}
	if (isShoot)
	{
		return true;
	}
	if (isSplit)
	{
		return true;
	}
	if (gameManager->frameOutManager.markDirtyUids.find(uid) != gameManager->frameOutManager.markDirtyUids.end())
	{
		return true;
	}
	return false;
}

bool BBPlayer::CanSkipSyncState()
{
	if (!this->gameManager->IsServer())
	{
		return predictionData.canSkipSync;
	}
	if (isShoot)
	{
		return false;
	}
	if (isSplit)
	{
		return false;
	}
	if (gameManager->frameOutManager.markDirtyUids.find(uid) != gameManager->frameOutManager.markDirtyUids.end())
	{
		return false;
	}
	return true;
}

void BBPlayer::UpdateFinalPoint(double x, double y)
{
	FinalPoint.SetPoint(BBMathUtils::bb_fix_float(x), BBMathUtils::bb_fix_float(y));
}

int BBPlayer::GetFinalPointX()
{
	return BBMathUtils::bb_float_to_int(FinalPoint.x);
}
int BBPlayer::GetFinalPointY()
{
	return BBMathUtils::bb_float_to_int(FinalPoint.y);
}

BBRect BBPlayer::GetGroupRect()
{
	BBRect ret;
	if (vecPlayerNodes.size() <= 0)
	{
		return ret;
	}
	BBPlayerNode* node = vecPlayerNodes[0];
	BBRect rect = node->rect;
	ret.setRect(rect.minX, rect.minY, rect.maxX, rect.maxY);
	for (int i = 1; i < vecPlayerNodes.size(); i++)
	{
		BBPlayerNode* stepNode = vecPlayerNodes[i];
		BBRect stepRect = stepNode->rect;
		if (ret.minX > stepRect.minX) {
			ret.minX = stepRect.minX;
		}
		if (ret.maxX < stepRect.maxX) {
			ret.maxX = stepRect.maxX;
		}
		if (ret.minY > stepRect.minY) {
			ret.minY = stepRect.minY;
		}
		if (ret.maxY < stepRect.maxY) {
			ret.maxY = stepRect.maxY;
		}
	}
	ret.centerX = (ret.minX + ret.maxX) / 2;
	ret.centerY = (ret.minY + ret.maxY) / 2;
	return ret;
}

bool PlayerClassComparePlayerNodes(BBPlayerNode* nodeA, BBPlayerNode* nodeB)
{
	return nodeA->idx < nodeB->idx;
}

void BBPlayer::SetGameManager(BBGameManager* gameManager)
{
	this->gameManager = gameManager;
	mCommandList.isServer = gameManager->IsServer();
}

void BBPlayer::AddPlayerNode(BBPlayerNode* node)
{
	node->player = this;
	vecPlayerNodes.emplace_back(node);
	std::sort(vecPlayerNodes.begin(), vecPlayerNodes.end(), PlayerClassComparePlayerNodes);
	if (Stopped && vecPlayerNodes.size() == 1)
	{
		UpdateFinalPoint(vecPlayerNodes[0]->location.x, vecPlayerNodes[0]->location.y);
	}
}

BBPlayerNode* BBPlayer::GetPlayerNode(int idx)
{
	for (auto node : vecPlayerNodes)
	{
		if (node->idx == idx)
		{
			return node;
		}
	}
	return NULL;
}

void BBPlayer::RemoveMass()
{
	if (vecPlayerNodes.size() == 0)
	{
		return;
	}
	int maxMass = -1;
	int maxMassIdx = -1;
	BBPlayerNode* maxPlayerNode = NULL;
	for (int i = 0; i < vecPlayerNodes.size(); i++)
	{
		BBPlayerNode* node = vecPlayerNodes[i];
		int mass = node->GetBallMass();
		if (maxMass < mass)
		{
			maxMass = mass;
			maxMassIdx = node->idx;
			maxPlayerNode = node;
		}
		else if (maxMass == mass && node->idx < maxMassIdx)
		{
			maxMassIdx = node->idx;
			maxPlayerNode = node;
		}
	}
	int massToRemove = maxMass;
	int delta = (massToRemove + NMass) / 1000;
	if (delta > 0)
	{
		NMass = (massToRemove + NMass) % 1000;
		if (maxMass - delta >= BBConfigManager::initBallMass)
		{
			maxPlayerNode->ChangeDeltaMass(-delta);
		}
	}
	else {
		NMass += massToRemove;
	}
}


void BBPlayer::_serverUpdate()
{
	int maxCnt = mCommandList.mCommandVec.size();
	BBMathUtils::BBLOG("mCommandVec  size: %i, %i", uid, maxCnt);
	if (maxCnt <= 0)
	{
		return;
	}
	if (maxCnt > 5)
	{
		maxCnt = 5;
	}
	for (int i = 0; i < maxCnt; i++)
	{
		BBMoveCommand& command = mCommandList.mCommandVec[i];
		//BBMathUtils::BBLOG("server command: %i", command.idx);
		processTick(command);
	}

	mCommandList.clearCommands(maxCnt);
}

void BBPlayer::_clientUpdateHandleMe()
{
	int cnt = mCommandList.mCommandVec.size();
	if (cnt <= 0)
	{
		return;
	}
	BBMoveCommand& command = mCommandList.mCommandVec[cnt - 1];
	if (command.idx > lastMoveCommandIdx)
	{
		processTick(command);
		lastMoveCommandIdx = command.idx;
	}
}

void BBPlayer::_clientUpdateHandleOther()
{
	//BBMathUtils::BBLOG("other player update");
	predictionData.mPredictionRight = true;
	predictionData.canSkipSync = true;
	int cnt = mCommandList.mCommandVec.size();
	if (cnt > 0)
	{
		predictionData.lastRunGameFrame = gameManager->GetGameFrame();
		if (cnt > BBConfigManager::maxOtherStepCommandCnt)
		{
			cnt = BBConfigManager::maxOtherStepCommandCnt;
		}
		//BBMathUtils::BBLOG("_clientUpdateHandleOther1:  %i  %i", cnt, predictionData.predictionCommands.size());
		if (cnt > 1)
		{
			this->isCatchingUp = true;
		}
		for (int i = 0; i < cnt; i++)
		{
			if (cnt > 1 && i == cnt -1)
			{
				this->isCatchingUp = false;
				this->CalcBallDelta();
			}

			BBMoveCommand& command = mCommandList.mCommandVec[i];
			predictionData.lastAngle = command.angle;
			predictionData.lastPressure = command.pressure;

			if (predictionData.predictionCommands.size() > 0)
			{
				auto& firstCommand = predictionData.predictionCommands[0];
				predictionData.predictionCommands.pop_front();
				//预测结果和服务器的结果如果出现不一致的情况,需要进行一次状态同步
				if (command.checkSum != firstCommand.crc)
				{
					//BBMathUtils::BBLOG("prediction not same  %u", command.checkSum);
					predictionData.mPredictionRight = false;
					predictionData.canSkipSync = false;
					processTick(command);
				}
				else 
				{
					//BBMathUtils::BBLOG("prediction same  %u", command.checkSum);
					if (i == cnt - 1)
					{
						//这里不能因为crc一致就放弃移动,只有最后一帧的command才需要去移动
						_clientOtherPrediction(true);
					}
				}
			}
			else 
			{
				//计算结果和服务器的结果如果出现不一致的情况,需要进行一次状态同步
				unsigned int crc = processTick(command);
				if (crc != command.checkSum)
				{
					predictionData.mPredictionRight = false;
					predictionData.canSkipSync = true;
					//BBMathUtils::BBLOG("calc not same  %u-%u", crc, command.checkSum);
				}
				else
				{
					predictionData.mPredictionRight = true;
					//BBMathUtils::BBLOG("calc same  %u-%u", crc, command.checkSum);
				}
			}
		}
		mCommandList.clearCommands(cnt);
	}
	else
	{
		if (gameManager->GetGameFrame() - predictionData.lastRunGameFrame >= BBConfigManager::otherTooSlowTicks)
		{
			predictionData.mPredictionRight = false;
			predictionData.canSkipSync = true;
			MoveBack();
		}
		else 
		{
			//BBMathUtils::BBLOG("_clientOtherPrediction");
			_clientOtherPrediction();
		}
	}
}

void BBPlayer::MoveBack()
{
	for (int i = 0; i < vecPlayerNodes.size(); i++)
	{
		BBPlayerNode* node = vecPlayerNodes[i];
		node->MoveBack(gameManager);
	}
}

void BBPlayer::_clientOtherPrediction(bool forceUpdate)
{
	if (predictionData.lastAngle == 0 && predictionData.lastPressure == 0)
	{
		predictionCommand.ClearData();
		BBSimplePrediction prediction;
		prediction.angle = predictionData.lastAngle;
		prediction.pressure = predictionData.lastPressure;

		unsigned int crc = processTick(predictionCommand);
		prediction.crc = crc;
		predictionData.predictionCommands.emplace_back(prediction);
		return;
	}
	//BBMathUtils::BBLOG("start prediction temp %i", predictionData.predictionCommands.size());
	if (predictionData.predictionCommands.size() >= BBConfigManager::maxPredictionCnt && !forceUpdate)
	{
		return;
	}

	//BBMathUtils::BBLOG("start prediction %i - %i ", predictionData.lastAngle, predictionData.lastPressure);
	predictionCommand.ClearData();
	predictionCommand.angle = predictionData.lastAngle;
	predictionCommand.pressure = predictionData.lastPressure;

	BBSimplePrediction prediction;
	prediction.angle = predictionData.lastAngle;
	prediction.pressure = predictionData.lastPressure;

	unsigned int crc = processTick(predictionCommand);
	prediction.crc = crc;
	predictionData.predictionCommands.emplace_back(prediction);
}

void BBPlayer::_clientUpdate()
{
	if (isMe())
	{
		_clientUpdateHandleMe();
	}
	else
	{
		_clientUpdateHandleOther();
	}
}

void BBPlayer::Update()
{
	isShoot = false;
	isSplit = false;
	mCommandList.mControlMismatch = false;
	if (gameManager->IsServer())
	{
		_serverUpdate();
	}
	else 
	{
		_clientUpdate();
	}
}

void BBPlayer::Eat()
{
	for (BBPlayerNode* node : vecPlayerNodes)
	{
		node->Eat(gameManager);
	}
}

void BBPlayer::Move()
{
	if (vecPlayerNodes.size() == 0)
	{
		this->direction = BBPoint::ZERO;
		this->Stopped = true;
		return;
	}

	//BBMathUtils::BBLOG("current target point %d-%d\n", this->FinalPoint.x, this->FinalPoint.y);
	for (int i = 0; i < vecPlayerNodes.size(); i++)
	{
		BBPlayerNode* node = vecPlayerNodes[i];
		node->Move(gameManager);
	}
}

void BBPlayer::_tryUpdateFinalPoint()
{
	if (!this->Stopped)
	{
		BBRect rect = this->GetGroupRect();
		int w = rect.maxX - rect.minX;
		int h = rect.maxY - rect.minY;
		int halfLen = (sqrtf(pow(w, 2) + pow(h, 2)) / 2.0f);
		int baseLen = halfLen * BBConfigManager::ballMoveRate;

		if (baseLen < BBConfigManager::fixLength) {
			baseLen = BBConfigManager::fixLength;
		}
		//BBMathUtils::BBLOG("rect info: %i-%i-%i ", w, h, halfLen);
		int extLen = baseLen * BBMathUtils::PressureToPercent(this->direction.y);
		BBVector targetVec = BBMathUtils::AngleToFixedVector(int(this->direction.x / 10.0f), extLen);
		if (!gameManager->IsServer() && !isMe())
		{
			//BBMathUtils::BBLOG("rect info: %i-%i-%i-%i，center is %i-%i direction: %i-%i", w, h, halfLen, extLen, rect.centerX, rect.centerY,this->direction.x, this->direction.y);
		}
		if (targetVec == BBVector::ZERO && this->vecPlayerNodes.size() == 1)
		{
			for (int i = 0; i < this->vecPlayerNodes.size(); i++)
			{
				BBPlayerNode* node = this->vecPlayerNodes[i];
				node->currentSpeedVec = BBVector::ZERO;
				this->UpdateFinalPoint(node->location.x, node->location.y);
			}
		}
		else
		{
			BBVector point;
			point.SetPoint(rect.centerX + targetVec.x, rect.centerY + targetVec.y);
			this->UpdateFinalPoint(point.x, point.y);
		}
	
	}

	//一定要在这里,不能放在上面,否则停不下来，因为放在上面finalPoint就不会更新了.
	if (this->direction.y == 0)
	{
		this->Stopped = true;
	}
	else
	{
		this->Stopped = false;
	}
}

void BBPlayer::_changeInitEndNodeSpeed()
{
	for (int i = 0; i < this->vecPlayerNodes.size(); i++)
	{
		BBPlayerNode* node = this->vecPlayerNodes[i];
		if (node->initStopFrame == 0)
		{
			node->SetSpeedVec(this->FinalPoint.x - node->location.x, this->FinalPoint.y - node->location.y);
			if (!this->isMe() && !this->gameManager->IsServer())
			{
				//BBMathUtils::BBLOG("calc speed vec %f-%f", node->currentSpeedVec.x, node->currentSpeedVec.y);
				//BBMathUtils::BBLOG("calc final point is: %f-%f, location is: %f-%f", this->FinalPoint.x, this->FinalPoint.y, node->location.x, node->location.y);
			}
		}
	}
}

void BBPlayer::_updateNodeTime()
{
	for (int i = 0; i < this->vecPlayerNodes.size(); i++)
	{
		BBPlayerNode* node = this->vecPlayerNodes[i];
		if (node->initStopFrame > 0)
		{
			node->initStopFrame -= 1;
		}
		if (node->cd > 0)
		{
			node->cd -= 1;
		}
		if (node->protect > 0)
		{
			node->protect -= 1;
		}
	}
}



unsigned int BBPlayer::processTick(BBMoveCommand& command)
{
	if (gameManager->IsServer())
	{
		//BBMathUtils::BBLOG("server handle command %d", command.idx);
	}
	this->direction.x = command.angle;
	this->direction.y = command.pressure;
	RemoveMass();
	_tryUpdateFinalPoint();
	//if (!this->gameManager->IsServer())
	//{
		//BBMathUtils::BBLOG("calc final point %f-%f", this->FinalPoint.x, this->FinalPoint.y);
	//}
	_changeInitEndNodeSpeed();
	
	_updateNodeTime();
	if (gameManager->IsServer())
	{
		if (command.isShoot)
		{
			ServerSelfShoot(gameManager);
		}
	}
	Move();
	if (gameManager->IsServer())
	{
		if (command.isSplit)
		{
			SelfSplit(gameManager);
		}
	}
	unsigned int crc = GetCrc();
	if (!gameManager->IsServer())
	{
		if (this->isMe())
		{
			BBMoveManager::SetCheckSum(crc);
		}
	}
	else {
		if (!this->isMe())
		{
			BBMathUtils::BBLOG("calc checkSum is %u:%u ",crc, command.idx);
		}
		unsigned int luaCrc = GetLuaCrc();
		gameManager->frameOutManager.AddCrc(uid, crc, luaCrc);
		//BBMathUtils::BBLOG("checkSum is %u:%u ", command.checkSum, crc, command.idx);
		if (command.checkSum != crc)
		{
			mCommandList.mControlMismatch = true;
			BBMathUtils::BBLOG("server checksum not same : %u:%u", crc, command.checkSum);
		}
		else {
			BBMathUtils::BBLOG("server checksum same : %u:%u", crc, command.checkSum);
		}
	}
	return crc;
}


void BBPlayer::ClientSyncServerPlayerInfo(int nMass, int directionX, int directionY, int ackCommand, int finalPointX, int finalPointY, bool Stopped)
{
	if (gameManager->IsServer())
	{
		return;
	}
	this->NMass = nMass;
	this->Stopped = Stopped;
	this->FinalPoint.x = BBMathUtils::bb_int_to_float(finalPointX);
	this->FinalPoint.y = BBMathUtils::bb_int_to_float(finalPointY);
	if (!this->isMe())
	{
		//同步完了状态就没必要保存预测的数据
		this->direction.x = directionX;
		this->direction.y = directionY;
		this->predictionData.ClearData();
		this->predictionData.lastAngle = directionX;
		this->predictionData.lastPressure = directionY;
		/*int commandCnt = mCommandList.mCommandVec.size();
		if (commandCnt > 0)
		{
			BBMoveCommand& lastCommand = mCommandList.mCommandVec[commandCnt - 1];
			predictionData.lastAngle = lastCommand.angle;
			predictionData.lastPressure = lastCommand.pressure;
		}*/
		mCommandList.mCommandVec.clear();
		//gameManager->RemovePlayerCommand(uid);
	}
	else 
	{
		mCommandList.clientSyncServerCommandAck(ackCommand);
	}
}


void BBPlayer::CalcBallDelta()
{
	for (int i = 0; i < vecPlayerNodes.size(); i++)
	{
		BBPlayerNode* node = vecPlayerNodes[i];
		node->CalcBallDelta();
	}
}

void BBPlayer::clientCatchup()
{
	if (gameManager->IsServer())
	{
		return;
	}
	if (!this->isMe())
	{
		return;
	}
	this->isCatchingUp = true;
	int maxCnt = mCommandList.mCommandVec.size();
	for (int i = 0; i < maxCnt; i++)
	{
		BBMoveCommand& command = mCommandList.mCommandVec[i];
		processTick(command);
	}
	this->isCatchingUp = false;
}

bool BBPlayerComparePlayerNodes(BBPlayerNode* nodeA, BBPlayerNode* nodeB)
{
	return nodeA->GetBallMass() > nodeB->GetBallMass();
}

void BBPlayer::SelfSplit(BBGameManager* gameManager)
{
	if (vecPlayerNodes.size() >= BBConfigManager::maxBallCount)
	{
		return;
	}
	isSplit = true;
	int curentCount = vecPlayerNodes.size();
	std::vector<BBPlayerNode*> splitPlayerNodes;
	for (auto playerNode : vecPlayerNodes)
	{
		if (playerNode->GetBallMass() > BBConfigManager::minSplitMass)
		{
			splitPlayerNodes.emplace_back(playerNode);
		}
	}
	std::sort(splitPlayerNodes.begin(), splitPlayerNodes.end(), BBPlayerComparePlayerNodes);
	std::vector<int> selectedNodes;
	for (int i = 0; i < splitPlayerNodes.size(); i++)
	{
		selectedNodes.emplace_back(i);
		curentCount++;
		if (curentCount == BBConfigManager::maxBallCount)
		{
			break;
		}
	}
	for (int idx : selectedNodes)
	{
		BBPlayerNode* child = splitPlayerNodes[idx];
		//gameManager->frameCacheManager.AddCd(child->idx, uid);
		gameManager->frameOutManager.AddCd(uid, child->idx);
		BBPlayerNode* newNode = child->SelfSplit(gameManager);
		gameManager->frameOutManager.AddCd(uid, newNode->idx);
		//gameManager->frameCacheManager.AddCd(newNode->idx, uid);
		gameManager->playerManager.OnNewPlayerNodeGenerate(this, newNode);
	}
}

void BBPlayer::ServerSelfShoot(BBGameManager* gameManager)
{
	for (int i = 0; i < vecPlayerNodes.size(); i++)
	{
		BBPlayerNode* playerNode = vecPlayerNodes[i];
		if (playerNode->GetBallMass() >= BBConfigManager::minShootBallMass)
		{
			isShoot = true;
			playerNode->ChangeDeltaMass(-1 * BBConfigManager::sporeMass);

			//暂时孢子由服务器生成,客户端不在这里生成孢子.
			gameManager->sporeManager.CreateSpore(playerNode);
		}
	}
}