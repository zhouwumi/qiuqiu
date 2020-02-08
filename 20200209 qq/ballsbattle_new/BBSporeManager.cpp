#include "BBSporeManager.h"
#include "BBGameManager.h"
#include "BBMathUtils.h"
#include "BBConst.h"

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

Spore* BBSporeManager::CreateSpore(BBPlayerNode* node)
{
	Spore* newSpore = gameManager->objectManager.CreateSpore(node);
	FixSpore(newSpore);
	mapSpores.emplace(newSpore->idx, newSpore);
	gameManager->NodeTree.AddCircleNode(newSpore);
	gameManager->frameCacheManager.AddNewSpore(newSpore->idx);
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

Spore* BBSporeManager::GetSporeInfo(int idx)
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
	float fixedX, fixedY;
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
		spore->InitMove();
		FixSpore(spore);
		spore->initStopFrame -= 1;
		if (spore->cd > 0)
		{
			spore->cd -= 1;
		}
		gameManager->NodeTree.UpdateCircleNode(spore);
	}
}

void BBSporeManager::AddNewSporeFromServer(int idx, int fromId, int uid, int mass, int x, int y, int directionX, int directionY, int currentX, int currentY, int curSpeed, int deltaSpeed, int initStopFrame, int cd)
{
	Spore* newSpore = new Spore();
	newSpore->fromId = fromId;
	newSpore->idx = idx;
	newSpore->uid = uid;
	newSpore->ChangePosition(x, y);
	newSpore->SetBallMass(BBConst::SporeMass);
	//newSpore->direction.SetPoint(directionX, directionY);
	newSpore->currentSpeedVec.SetPoint(currentX, currentY);
	newSpore->initStopFrame = initStopFrame;
	newSpore->cd = cd;
	newSpore->initSpeed = curSpeed;
	newSpore->initDeltaSpeed = deltaSpeed;
	newSpore->SetBallMass(mass);

	mapSpores.emplace(newSpore->idx, newSpore);
	gameManager->NodeTree.AddCircleNode(newSpore);
	gameManager->frameCacheManager.AddNewSpore(newSpore->idx);
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
