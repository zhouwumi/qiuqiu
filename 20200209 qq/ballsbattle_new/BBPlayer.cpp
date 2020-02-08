#include "BBPlayer.h"
#include "BBPlayerNode.h"
#include<math.h>
#include "BBMathUtils.h"
#include "BBConst.h"
#include "BBGameManager.h"
#include "BBMoveManager.h"

BBPlayer::BBPlayer(unsigned int id, unsigned int idx) :
	uid(id),
	playerIdx(idx),
	Stopped(true),
	lastMoveCommand(-1),
	nextNodeIdx(1),
	NMass(0),
	isCatchingUp(false)
{
}

BBPlayer::~BBPlayer()
{
}

bool BBPlayer::isMe()
{
	if (gameManager)
	{
		return gameManager->GetUserId() == uid;
	}
	return false;
}

unsigned int BBPlayer::GetNextPlayerNodeIdx()
{
	return uid * 10000 + nextNodeIdx;
}


void BBPlayer::UpdateFinalPoint(float x, float y)
{
	FinalPoint.SetPoint(BBMathUtils::bb_fix_float(x), BBMathUtils::bb_fix_float(y));
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
	nextNodeIdx++;
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
	int massToRemove = maxMass * 2;
	int delta = (massToRemove + NMass) / 1000;
	if (delta > 0)
	{
		NMass = (massToRemove + NMass) % 1000;
		if (maxMass - delta >= BBConst::InitMass)
		{
			maxPlayerNode->ChangeDeltaMass(-delta);
		}
	}
	else {
		NMass += massToRemove;
	}
}

void BBPlayer::Update()
{
	mCommandList.mControlMismatch = false;
	if (gameManager->IsServer())
	{
		int maxCnt = mCommandList.mCommandVec.size();
		BBMathUtils::BBLOG("mCommandVec  size:%i", maxCnt);
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
			BBMathUtils::BBLOG("server command: %i", command.idx);
			processTick(command);
		}

		mCommandList.clearCommands(maxCnt);
	}
	else {
		int cnt = mCommandList.mCommandVec.size();
		if (cnt > 0)
		{
			if (isMe())
			{
				BBMoveCommand& command = mCommandList.mCommandVec[cnt - 1];
				if (command.idx > lastMoveCommand)
				{
					processTick(command);
					lastMoveCommand = command.idx;
				}
			}
			else
			{
				int maxCnt = mCommandList.mCommandVec.size();
				if (maxCnt <= 0)
				{
					return;
				}
				if (maxCnt > 2)
				{
					maxCnt = 2;
				}
				predictionData.mPredictionRight = true;
				for (int i = 0; i < maxCnt; i++)
				{
					BBMoveCommand& command = mCommandList.mCommandVec[i];
					if (predictionData.predictionCommands.size() > 0)
					{
						if (!command.IsEqual(predictionData.predictionCommands[0]))
						{
							predictionData.mPredictionRight = false;
							processTick(command);
							predictionData.lastAngle = command.angle;
							predictionData.lastPressure = command.pressure;
						}
						predictionData.predictionCommands.pop_front();
					}
					else {
						processTick(command);
						predictionData.lastAngle = command.angle;
						predictionData.lastPressure = command.pressure;
					}
				}
				mCommandList.clearCommands(maxCnt);
			}
		}
		else {
			if (predictionData.predictionCommands.size() >= 5 || (predictionData.lastAngle == 0 && predictionData.lastPressure == 0))
			{
				return;
			}
			BBMoveCommand newCommand;
			newCommand.angle = predictionData.lastAngle;
			newCommand.pressure = predictionData.lastPressure;

			BBSimplePrediction prediction;
			prediction.angle = predictionData.lastAngle;
			prediction.pressure = predictionData.lastPressure;

			predictionData.predictionCommands.emplace_back(prediction);
			processTick(newCommand);
		}
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
		int baseLen = halfLen * BBConst::MoveRate / BBConst::MoveBase;
		if (baseLen < BBConst::FixLength) {
			baseLen = BBConst::FixLength;
		}
		BBMathUtils::BBLOG("rect info: %i-%i-%i ", w, h, halfLen);
		int extLen = baseLen * BBMathUtils::PressureToPercent(this->direction.y);
		BBVector targetVec = BBMathUtils::AngleToFixedVector(int(this->direction.x / 10.0f), extLen);
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
			BBMathUtils::BBLOG("calc speed vec %f-%f", node->currentSpeedVec.x, node->currentSpeedVec.y);
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

bool BBPlayerComparePlayerNodes(BBPlayerNode* nodeA, BBPlayerNode* nodeB)
{
	return nodeA->idx < nodeB->idx;
}

void BBPlayer::SelfSplit(BBGameManager* gameManager)
{
	if (vecPlayerNodes.size() >= BBConst::MaxCell)
	{
		return;
	}
	int curentCount = vecPlayerNodes.size();
	std::vector<BBPlayerNode*> splitPlayerNodes;
	for (auto playerNode : vecPlayerNodes)
	{
		if (playerNode->GetBallMass() > BBConst::MinSplitMass)
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
		if (curentCount == BBConst::MaxCell)
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

void BBPlayer::SelfShoot(BBGameManager* gameManager)
{
	for (int i = 0; i < vecPlayerNodes.size(); i++)
	{
		BBPlayerNode* playerNode = vecPlayerNodes[i];
		if (playerNode->GetBallMass() >= BBConst::ShootMinMass)
		{
			playerNode->ChangeDeltaMass(-1 * BBConst::SporeMass);

			//暂时孢子由服务器生成,客户端不在这里生成孢子.
			gameManager->sporeManager.CreateSpore(playerNode);
		}
	}
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

void BBPlayer::processTick(BBMoveCommand& command)
{
	if (gameManager->IsServer())
	{
		BBMathUtils::BBLOG("server handle command %d", command.idx);
	}
	this->direction.x = command.angle;
	this->direction.y = command.pressure;
	RemoveMass();
	_tryUpdateFinalPoint();
	BBMathUtils::BBLOG("calc final point %f-%f", this->FinalPoint.x, this->FinalPoint.y);
	_changeInitEndNodeSpeed();
	
	_updateNodeTime();
	if (command.isShoot)
	{
		SelfShoot(gameManager);
	}
	if (command.isSplit)
	{
		SelfSplit(gameManager);
	}
	Move();
	unsigned int crc = GetCrc();
	if (!gameManager->IsServer())
	{
		command.checkSum = crc;
		if (this->isMe())
		{
			BBMoveManager::SetCheckSum(crc);
		}
	}
	else {
		BBMathUtils::BBLOG("checkSum is %u:%u ", command.checkSum, crc, command.idx);
		if (command.checkSum != crc)
		{
			mCommandList.mControlMismatch = true;
		}
		else {
			mCommandList.mControlMismatch = false;
		}
	}
}

BBFrameData BBPlayer::serverGetFrameData()
{
	BBFrameData ret;
	if (!gameManager->IsServer())
	{
		return ret;
	}
	ret.uid = uid;
	ret.NMass = NMass;
	ret.angle = direction.x;
	ret.pressure = direction.y;
	ret.nextNodeIdx = nextNodeIdx;
	ret.ackCommand = mCommandList.serverGetCommandAck();
	for (auto iter : vecPlayerNodes)
	{
		ret.ballDatas.emplace_back(iter->GetBallData());
	}
	return ret;
}

BBFrameData BBPlayer::packUpdate()
{
	return serverGetFrameData();
}

void BBPlayer::ClientSyncServerPlayerInfo(int nextNodeIdx, int nMass, int directionX, int directionY, int ackCommand)
{
	if (gameManager->IsServer())
	{
		return;
	}
	this->NMass = nMass;
	this->nextNodeIdx = nextNodeIdx;
	if (!this->isMe())
	{
		this->direction.x = directionX;
		this->direction.y = directionY;
	}
	else {
		mCommandList.clientSyncServerCommandAck(ackCommand);
	}
}

void BBPlayer::clientReadFrameData(BBFrameData frameData)
{
	if (gameManager->IsServer())
	{
		return;
	}
	if (!this->isMe())
	{
		return;
	}
	int ackCommand = frameData.ackCommand;
	auto& ballDatas = frameData.ballDatas;
	if (ballDatas.size() <= 0)
	{
		mCommandList.clientSyncServerCommandAck(ackCommand);
		return;
	}
	mCommandList.clientSyncServerCommandAck(ackCommand);

	if (vecPlayerNodes.size() == ballDatas.size())
	{
		this->NMass = frameData.NMass;
		for (int i = 0; i < ballDatas.size(); i++)
		{
			BBPlayerNode* node = vecPlayerNodes[i];
			node->ClientSyncBallDataFromServer(ballDatas[i]);
		}
		clientCatchup();
		for (int i = 0; i < ballDatas.size(); i++)
		{
			BBPlayerNode* node = vecPlayerNodes[i];
			node->CalcBallDelta();
		}
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

void BBPlayer::unPackUpdate(BBFrameData frameData)
{
	int uid = frameData.uid;
	if (uid != this->uid)
	{
		return;
	}
	if (this->isMe())
	{
		return;
	}
	auto& ballDatas = frameData.ballDatas;
	if (vecPlayerNodes.size() == ballDatas.size())
	{
		this->NMass = frameData.NMass;
		for (int i = 0; i < ballDatas.size(); i++)
		{
			BBPlayerNode* node = vecPlayerNodes[i];
			node->ClientSyncBallDataFromServer(ballDatas[i]);
		}
		//clientCatchup();
		for (int i = 0; i < ballDatas.size(); i++)
		{
			BBPlayerNode* node = vecPlayerNodes[i];
			node->CalcBallDelta();
		}
		predictionData.ClearData();
	}
}