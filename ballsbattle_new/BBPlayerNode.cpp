#include "BBPlayerNode.h"
#include<math.h>

#include "BBConst.h"
#include "BBMathUtils.h"
#include "BBGameManager.h"
#include "BBPlayer.h"
#include "BBConfigManager.h"

BBPlayerNode::BBPlayerNode():
	cd(0),
	protect(0),
	initStopFrame(0),
	initSpeed(0),
	initDeltaSpeed(0)
{

}
BBPlayerNode::~BBPlayerNode()
{

}

void BBPlayerNode::ChangeCd(int timeDelta)
{
	int cd = roundf((timeDelta * mass * BBConst::FPS) / 1000.0f);
	this->cd = (cd / 5) * 5;
}

void BBPlayerNode::InitMove()
{
	initSpeed -= initDeltaSpeed;
	currentSpeedVec = BBVector::GetFixedVetor2(currentSpeedVec, initSpeed);
	BBMathUtils::bb_fix_bb_vector(currentSpeedVec);
	
}

void BBPlayerNode::SetSpeedVec(double targetX, double targetY)
{
	currentSpeedVec = BBVector::GetFixedVetor2(BBVector(targetX, targetY), speed);
	BBMathUtils::bb_fix_bb_vector(currentSpeedVec);
}

void BBPlayerNode::CalculateInitMoveParams(int radius, int frame, int initSpeed)
{
	this->initStopFrame = frame;
	double delta = fabs((double)(initSpeed) / frame);
	delta = radius / double(frame * (frame - 1) / 2) + delta;

	int intDelta = delta;
	int init = frame * intDelta;

	this->initSpeed = init;
	this->initDeltaSpeed = intDelta;
}


unsigned int BBPlayerNode::GetCrc()
{
	static unsigned int playerNodeBuffer[4];
	unsigned int index = 0;
	
	playerNodeBuffer[index++] = BBMathUtils::bb_float_to_int(this->location.x);
	playerNodeBuffer[index++] = BBMathUtils::bb_float_to_int(this->location.y);

	unsigned int size = index * sizeof(int) / sizeof(char);
	unsigned int ret = BBMathUtils::crc32((char*)(&playerNodeBuffer), size);
	return ret;
}

void BBPlayerNode::Move(BBGameManager* gameManager)
{
	BBVector oldLocation = location;
	BBVector locVec = _stepMove();
	//hit
	if (this->initStopFrame == 0 && this->player->vecPlayerNodes.size() > 1)
	{
		bool isHit = _handleNodeHit(locVec);
		if (isHit)
		{
			//BBMathUtils::BBLOG("node %d happend hit from position from %f-%f  to %f-%f\n", this->idx, this->location.x, this->location.y, locVec.x, locVec.y);
		}
	}
	//fix circle
	double fixedX, fixedY;
	BBMathUtils::FixCircle(gameManager->mapRect, locVec.x, locVec.y, this->GetRadius(), fixedX, fixedY);
	if (this->currentSpeedVec != BBVector::ZERO && !this->player->gameManager->IsServer())
	{
		BBMathUtils::BBLOG("node %d change position from %f-%f  to %f-%f\n", this->idx, this->location.x, this->location.y, fixedX, fixedY);
		BBMathUtils::BBLOG("node %d currentSpeedVec is %f-%f, init is %d cd is %d\n", this->idx, this->currentSpeedVec.x, this->currentSpeedVec.y, this->initStopFrame, this->cd);
	}
	this->ChangePosition(fixedX, fixedY);
	if (!this->player->isCatchingUp && !gameManager->IsServer())
	{
		double deltaX = location.x - oldLocation.x;
		double deltaY = location.y - oldLocation.y;
		double newRenderX = mDeltaData.location.x + deltaX;
		double newRenderY = mDeltaData.location.y + deltaY;
		if (mDeltaData.wrapTicks > 0)
		{
			newRenderX += mDeltaData.wrapLocationOffset.x;
			newRenderY += mDeltaData.wrapLocationOffset.y;
			mDeltaData.wrapTicks--;
		}
		if (!this->player->gameManager->IsServer() && !this->player->isMe())
		{
			BBMathUtils::BBLOG("change render %i, %f-%f, %f-%f, %f-%f  %i-%i",idx,deltaX, deltaY, mDeltaData.wrapLocationOffset.x, mDeltaData.wrapLocationOffset.y, newRenderX, newRenderY, this->initStopFrame, mDeltaData.wrapTicks);
		}
		this->ChangeRenderPosition(newRenderX, newRenderY);
	}
	gameManager->NodeTree.UpdateCircleNode(this);
}

BBVector BBPlayerNode::_stepMove()
{
	BBVector finalPoint = player->FinalPoint;
	double lastX = this->location.x;
	double lastY = this->location.y;
	if (this->initStopFrame > 0)
	{
		//BBVector before = node->currentSpeedVec;
		this->InitMove();
		if (!player->gameManager->IsServer())
		{
			BBMathUtils::BBLOG("InitMove %i %f-%f %i", idx, currentSpeedVec.x, currentSpeedVec.y, this->initStopFrame);
		}
		//BBMathUtils::BBLOG("node %d ball init move beforedir = %f-%f, dir = %f-%f speed is: %d", node->idx, before.x, before.y, node->currentSpeedVec.x, node->currentSpeedVec.y, node->initSpeed);
	}

	BBVector locVec = location;
	if (this->currentSpeedVec != BBVector::ZERO)
	{
		locVec.x += this->currentSpeedVec.x;
		locVec.y += this->currentSpeedVec.y;
		if (this->initStopFrame > 0)
		{
			return locVec;
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
	return locVec;
}

bool BBPlayerNode::_handleNodeHit(BBVector& locVec)
{
	bool ret = false;
	std::vector<BBPlayerNode*>& allPlayerNodes = player->vecPlayerNodes;
	for (int j = 0; j < allPlayerNodes.size(); j++)
	{
		BBPlayerNode* ballB = allPlayerNodes[j];
		if (ballB->initStopFrame <= 0 && ballB != this)
		{
			double deltaX = ballB->location.x - locVec.x;
			double deltaY = ballB->location.y - locVec.y;

			double distance = sqrtf(powf(deltaX, 2) + powf(deltaY, 2));
			int totalCircle = ballB->GetRadius() + this->GetRadius();
			if (totalCircle > distance)
			{
				double length = totalCircle - distance;
				double deltaLen = (double)BBConst::Delta / BBConst::DeltaBase;
				if (this->cd > 0 || ballB->cd > 0 || BBMathUtils::NeedRollback(this, ballB, deltaLen))
				{
					BBVector fixVec(locVec.x - ballB->location.x, locVec.y - ballB->location.y);
					BBVector vec = BBVector::GetFixedVetor2(fixVec, length);
					BBVector oldVec = locVec;
					if (locVec.x <= ballB->location.x)
					{
						locVec.x = locVec.x - fabs(vec.x);
					}
					else if (locVec.x > ballB->location.x)
					{
						locVec.x = locVec.x + fabs(vec.x);
					}

					if (locVec.y <= ballB->location.y)
					{
						locVec.y = locVec.y - fabs(vec.y);
					}
					else if (locVec.y > ballB->location.y)
					{
						locVec.y = locVec.y + fabs(vec.y);
					}
				}
				ret = true;
			}
		}
	}
	return ret;
}

void BBPlayerNode::Eat(BBGameManager* gameManager)
{
	std::vector<int> vec;
	if (gameManager->GetCanEatFoodSpiky())
	{
		gameManager->hitManager.GetCanEatFood(this, vec);
		if (vec.size() > 0)
		{
			for (int i = 0; i < vec.size(); i += 1)
			{
				int uid = this->player->uid;
				int nodeIdx = this->idx;
				int foodIdx = vec[i];
				gameManager->foodSpikyManager.RemoveFoodByIdx(foodIdx);
				gameManager->frameOutManager.AddFoodEatInfo(uid, nodeIdx, foodIdx);
			}
			vec.clear();
		}
		gameManager->hitManager.GetCanEatSpiky(this, vec);
		if (vec.size() > 0)
		{
			for (int i = 0; i < vec.size(); i += 2)
			{
				int uid = this->player->uid;
				int nodeIdx = this->idx;
				int spikyIdx = vec[i];
				int spikyBallMass = vec[i + 1];
				gameManager->frameOutManager.AddSpikyEatInfo(uid, nodeIdx, spikyIdx, spikyBallMass);
				gameManager->foodSpikyManager.SpikyBeEat(spikyIdx);
			}
			vec.clear();
		}
	}
	std::vector<int> sporeVec;
	gameManager->hitManager.GetCanEatNodeOrSpore(this, vec, sporeVec);
	if (sporeVec.size() > 0)
	{
		for (int i = 0; i < sporeVec.size(); i += 2)
		{
			int uid = this->player->uid;
			int nodeId = this->idx;
			int beEatSporeId = sporeVec[i];
			int beEatSporeMass = sporeVec[i + 1];
			gameManager->frameOutManager.AddSporeEatInfo(uid, nodeId, beEatSporeId, beEatSporeMass);
			gameManager->sporeManager.RemoveSporeByIdx(beEatSporeId);
		}
		sporeVec.clear();
	}
	if (vec.size() > 0)
	{
		for (int i = 0; i < vec.size(); i += 3)
		{
			int uid = this->player->uid;
			int nodeId = this->idx;
			int beEatUid = vec[i];
			int beEatNodeId = vec[i + 1];
			int beEatBallMass = vec[i + 2];
			gameManager->frameOutManager.AddNodeEatInfo(uid, nodeId, beEatUid, beEatNodeId, beEatBallMass);
		}
	}
}

void BBPlayerNode::EatFoodSpikySporeChangeMass(BBGameManager* gameManager)
{
	int deltaMass = 0;
	auto nodeId2EatFoods = gameManager->frameOutManager.nodeId2EatFoods;
	auto nodeId2EatSpores = gameManager->frameOutManager.nodeId2EatSpores;

	if (nodeId2EatFoods.find(this->idx) != nodeId2EatFoods.end())
	{
		deltaMass += nodeId2EatFoods[this->idx].size() * BBConst::FoodMass;
	}

	if (nodeId2EatSpores.find(this->idx) != nodeId2EatSpores.end())
	{
		deltaMass += nodeId2EatSpores[this->idx].size() * BBConst::SporeMass;
	}

	if (deltaMass != 0)
	{
		int oldMass = this->GetBallMass();
		if (this->GetBallMass() + deltaMass < BBConst::InitMass)
		{
			deltaMass = BBConst::InitMass - this->GetBallMass();
		}
		this->ChangeDeltaMass(deltaMass);
		BBMathUtils::BBLOG("ball eat other mass change: %d-->%d\n", oldMass, this->GetBallMass());
	}
}

int BBPlayerNode::SpikySplit(BBGameManager* gameManager, int maxChildNode, int spikyMass)
{
	if (this->GetBallMass() < spikyMass)
	{
		return 0;
	}
	if (maxChildNode == 0)
	{
		this->ChangeDeltaMass(spikyMass);
		return 0;
	}
	BBPlayer* player = this->player;
	int assignMass = 2 * spikyMass;
	int childMass = assignMass / (maxChildNode + 1);
	if (childMass > 40)
	{
		childMass = 40;
	}
	int centerMass = this->GetBallMass() + spikyMass - childMass * maxChildNode;
	this->ChangeDeltaMass(centerMass - this->GetBallMass());
	int splitAngle = ceilf(360.0 / maxChildNode);
	int directionAngle = this->currentSpeedVec.GetAngle();
	for (int i = 0; i < maxChildNode; i++)
	{
		BBPlayerNode* newPlayerNode = new BBPlayerNode();
		newPlayerNode->fromId = this->idx;
		newPlayerNode->uid = this->uid;
		newPlayerNode->player = this->player;
		newPlayerNode->idx = this->player->GetNextPlayerNodeIdx();
		newPlayerNode->SetBallMass(childMass);
		newPlayerNode->currentSpeedVec = BBMathUtils::AngleToFixedVector(directionAngle + splitAngle * i, newPlayerNode->speed);

		BBVector moveVec = BBVector::GetFixedVetor2(newPlayerNode->currentSpeedVec, this->GetRadius());
		newPlayerNode->ChangePosition(this->location.x + moveVec.x, this->location.y + moveVec.y);
		newPlayerNode->CalculateInitMoveParams(newPlayerNode->GetRadius(), BBConst::SplitFrame, BBConst::SplitInitSpeed);
		gameManager->frameOutManager.AddCd(newPlayerNode->uid, newPlayerNode->idx);
		gameManager->playerManager.OnNewPlayerNodeGenerate(player, newPlayerNode);
	}
	gameManager->frameOutManager.AddCd(this->uid, this->idx);
	return maxChildNode;
}

BBPlayerNode* BBPlayerNode::SelfSplit(BBGameManager* gameManager)
{
	int childMas = this->GetBallMass() / 2;
	this->ChangeDeltaMass(-childMas);
	BBPlayerNode* newPlayerNode = new BBPlayerNode();
	newPlayerNode->fromId = this->idx;
	newPlayerNode->uid = this->uid;
	newPlayerNode->player = this->player;
	newPlayerNode->idx = this->player->GetNextPlayerNodeIdx();

	if (this->currentSpeedVec == BBVector::ZERO) {
		//newPlayerNode->currentSpeedVec = BBVector::GetFixedVetor2(BBVector(1, 0), BBMathUtils::Mass2Speed(childMas));
		newPlayerNode->currentSpeedVec.SetPoint(1, 0);
		BBMathUtils::bb_fix_bb_vector(newPlayerNode->currentSpeedVec);
	}
	else {
		newPlayerNode->currentSpeedVec = this->currentSpeedVec;
		BBMathUtils::bb_fix_bb_vector(newPlayerNode->currentSpeedVec);
	}
	newPlayerNode->SetBallMass(childMas);
	BBVector moveVec = BBVector::GetFixedVetor2(newPlayerNode->currentSpeedVec, this->GetRadius());
	newPlayerNode->ChangePosition(this->location.x + moveVec.x, this->location.y + moveVec.y);
	
	//newPlayerNode->initStopFrame = BBConst::SplitFrame;
	//newPlayerNode->currentSpeedVec = newPlayerNode->direction;
	newPlayerNode->CalculateInitMoveParams(newPlayerNode->GetRadius(), BBConst::SplitFrame, BBConst::SplitInitSpeed);
	return newPlayerNode;
}

void BBPlayerNode::ClientSyncServerPlayerNode(int fromId, int x, int y, int mass, int cd, int protect, int initStopFrame, int initSpeed, int initDeltaSpeed, int speedX, int speedY)
{
	/*if (initStopFrame > 0 || this->initStopFrame > 0)
	{
		return;
	}*/
	this->fromId = fromId;
	this->SetBallMass(mass);
	this->cd = cd;
	this->protect = protect;
	this->initStopFrame = initStopFrame;
	this->initSpeed = initSpeed;
	this->initDeltaSpeed = initDeltaSpeed;
	this->currentSpeedVec.SetPoint(BBMathUtils::bb_int_to_float(speedX), BBMathUtils::bb_int_to_float(speedY));
	this->ChangePosition(BBMathUtils::bb_int_to_float(x), BBMathUtils::bb_int_to_float(y));
	this->CalcBallDelta();
	//this->location.SetPoint(BBMathUtils::bb_int_to_float(x), BBMathUtils::bb_int_to_float(y));
}

void BBPlayerNode::CalcBallDelta()
{
	bool isOther = false;
	int ticks = 0;
	if (this->player->isMe())
	{
		ticks = this->player->mCommandList.mCommandVec.size();
		if (ticks < BBConfigManager::minCatchUpTicks)
		{
			ticks = BBConfigManager::minCatchUpTicks;
		}
		if (ticks > BBConfigManager::maxCatchUpTicks)
		{
			ticks = BBConfigManager::maxCatchUpTicks;
		}
	}
	else 
	{
		ticks = mDeltaData.wrapTicks + 1;
		
		if (ticks > 10)
		{
			ticks = 10;
		}
		if (ticks < 4)
		{
			ticks = 4;
		}
		isOther = true;
	}
	
	mDeltaData.wrapTicks = ticks;
	mDeltaData.wrapLocationOffset.x = (location.x - mDeltaData.location.x) / ticks;
	mDeltaData.wrapLocationOffset.y = (location.y - mDeltaData.location.y) / ticks;
	if (isOther)
	{
		BBMathUtils::BBLOG("calc wrap location offset: %i, %i, %i %f-%f", idx, ticks, speed, mDeltaData.wrapLocationOffset.x, mDeltaData.wrapLocationOffset.y);
	}
}

int BBPlayerNode::GetRenderX()
{
	return BBMathUtils::bb_float_to_int(mDeltaData.location.x);
}
int BBPlayerNode::GetRenderY()
{
	return BBMathUtils::bb_float_to_int(mDeltaData.location.y);
}