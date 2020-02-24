#include "BBSporeManager.h"
#include "BBGameManager.h"
#include "BBMathUtils.h"

#include "BBConfigManager.h"

BBSporeManager::BBSporeManager()
{
}


BBSporeManager::~BBSporeManager()
{
	ClearAllSpore();
}

void BBSporeManager::ClearAllSpore()
{
	for (auto iter : mapSpores)
	{
		delete iter.second;
	}
	mapSpores.clear();
}

void BBSporeManager::DebugCreateSpore()
{
	Spore* newSpore = gameManager->objectManager.DebugCreateSpore();
	FixSpore(newSpore);
	mapSpores.emplace(newSpore->idx, newSpore);
	gameManager->NodeTree.AddCircleNode(newSpore);
	gameManager->frameOutManager.AddNewSpore(newSpore->idx);
	vecSporeIds.emplace_back(newSpore->idx);
	std::sort(vecSporeIds.begin(), vecSporeIds.end());
}

Spore* BBSporeManager::CreateSpore(BBPlayerNode* node)
{
	Spore* newSpore = gameManager->objectManager.CreateSpore(node);
	FixSpore(newSpore);
	mapSpores.emplace(newSpore->idx, newSpore);
	gameManager->NodeTree.AddCircleNode(newSpore);
	gameManager->frameOutManager.AddNewSpore(newSpore->idx);
	vecSporeIds.emplace_back(newSpore->idx);
	std::sort(vecSporeIds.begin(), vecSporeIds.end());
	return newSpore;
}

void BBSporeManager::RemoveSporeByIdx(int idx)
{
	if (mapSpores.find(idx) == mapSpores.end())
	{
		return;
	}
	Spore* sporeNode = mapSpores[idx];
	mapSpores.erase(idx);
	gameManager->NodeTree.RemoveCircleNode(sporeNode);
	for (auto iter = vecSporeIds.begin(); iter != vecSporeIds.end(); iter++)
	{
		if ((*iter) == idx)
		{
			vecSporeIds.erase(iter);
			break;
		}
	}
	std::sort(vecSporeIds.begin(), vecSporeIds.end());
}

Spore* BBSporeManager::GetSpore(int idx)
{
	if (mapSpores.find(idx) == mapSpores.end())
	{
		return NULL;
	}
	return mapSpores[idx];
}

void BBSporeManager::FixSpore(Spore* spore)
{
	BBVector& point = spore->location;
	double fixedX, fixedY;
	BBMathUtils::FixCircle(gameManager->mapRect, point.x, point.y, spore->GetRadius(), fixedX, fixedY);
	spore->ChangePosition(fixedX, fixedY);
}

void BBSporeManager::MoveSpores()
{
	for (const auto& iter : mapSpores)
	{
		Spore* spore = iter.second;
		if (spore->initStopFrame <= 0)
		{
			continue;
		}
		double deltaX = spore->location.x;
		double deltaY = spore->location.y;
		spore->InitMove();
		FixSpore(spore);
		if (!gameManager->IsServer())
		{
			deltaX = spore->location.x - deltaX;
			deltaY = spore->location.y - deltaY;
			double newRenderX = spore->mDeltaData.location.x + deltaX;
			double newRenderY = spore->mDeltaData.location.y + deltaY;
			if (spore->mDeltaData.wrapTicks > 0)
			{
				spore->mDeltaData.wrapTicks--;
				newRenderX += spore->mDeltaData.wrapLocationOffset.x;
				newRenderY += spore->mDeltaData.wrapLocationOffset.y;
			}
			//BBMathUtils::BBLOG("spore change render real pos %i %f-%f, %f-%f", spore->idx, spore->location.x, spore->location.y, newRenderX, newRenderY);
			//BBMathUtils::BBLOG("spore change render pos  %i %f-%f, %f-%f", spore->idx, spore->mDeltaData.wrapLocationOffset.x, spore->mDeltaData.wrapLocationOffset.y, deltaX, deltaY);
			spore->ChangeRenderPosition(newRenderX, newRenderY);
		}

		
		if (spore->cd > 0)
		{
			spore->cd -= 1;
		}
		gameManager->NodeTree.UpdateCircleNode(spore);
	}
}

void BBSporeManager::AddNewSporeFromServer(int idx, int fromId, unsigned int uid, int x, int y, int directionX, int directionY, int initSpeed, int initDeltaSpeed, int initStopFrame, int cd)
{
	Spore* newSpore = new Spore();
	newSpore->fromId = fromId;
	newSpore->idx = idx;
	newSpore->uid = uid;
	newSpore->ChangePosition(BBMathUtils::bb_int_to_float(x), BBMathUtils::bb_int_to_float(y));
	newSpore->ChangeRenderPosition(BBMathUtils::bb_int_to_float(x), BBMathUtils::bb_int_to_float(y));
	

	newSpore->SetBallMass(BBConfigManager::sporeMass);
	newSpore->SetSpeedVec(directionX, directionY);
	newSpore->initStopFrame = initStopFrame;
	newSpore->cd = cd;
	newSpore->initSpeed = initSpeed;
	newSpore->initDeltaSpeed = initDeltaSpeed;

	newSpore->CalcBallDelta();

	mapSpores.emplace(newSpore->idx, newSpore);
	gameManager->NodeTree.AddCircleNode(newSpore);
	//gameManager->frameCacheManager.AddNewSpore(newSpore->idx);
	vecSporeIds.emplace_back(newSpore->idx);
	std::sort(vecSporeIds.begin(), vecSporeIds.end());
}

void BBSporeManager::SyncShootFromServer(int idx, int fromId, unsigned int uid, int speedX, int speedY, int locationX, int locationY)
{
	BBPlayerNode* playerNode = gameManager->GetPlayerNode(fromId);
	if (!playerNode)
	{
		return;
	}
	Spore* newSpore = new Spore();
	newSpore->fromId = fromId;
	newSpore->idx = idx;
	newSpore->uid = uid;
	newSpore->SetBallMass(BBConfigManager::sporeMass);
	newSpore->SetSpeedVec(speedX, speedY);
	newSpore->ChangePosition(BBMathUtils::bb_int_to_float(locationX), BBMathUtils::bb_int_to_float(locationY));

	BBVector moveVec = BBVector::GetFixedVetor2(newSpore->currentSpeedVec, playerNode->GetRadius());
	//moveVec.SetPoint(0, 0);
	newSpore->ChangeRenderPosition(playerNode->mDeltaData.location.x + moveVec.x, playerNode->mDeltaData.location.y + moveVec.y);
	
	newSpore->cd = BBConfigManager::sporeCdTime;
	newSpore->CalculateInitMoveParams(newSpore->GetRadius(), BBConfigManager::sporeStopFrame, BBConfigManager::sporeInitSpeed);
	newSpore->CalcBallDelta();
	mapSpores.emplace(newSpore->idx, newSpore);
	gameManager->NodeTree.AddCircleNode(newSpore);
	//gameManager->frameCacheManager.AddNewSpore(newSpore->idx);
	vecSporeIds.emplace_back(newSpore->idx);
	std::sort(vecSporeIds.begin(), vecSporeIds.end());
}

unsigned int BBSporeManager::GetAllSporeCrc()
{
	static unsigned int sporeBuffer[1024 * 4];
	unsigned int index = 0;

	std::vector<unsigned int> idxs;
	for (const auto& iter : mapSpores)
	{
		idxs.emplace_back(iter.first);
	}
	std::sort(idxs.begin(), idxs.end());
	for (int i = 0; i < idxs.size(); i++)
	{
		int id = idxs[i];
		Spore* spore = mapSpores[id];
		sporeBuffer[index++] = spore->location.x;
		sporeBuffer[index++] = spore->location.y;
	}

	unsigned int size = index * sizeof(int) / sizeof(char);
	unsigned int ret = BBMathUtils::crc32((char*)(&sporeBuffer), size);
	return ret;
}
