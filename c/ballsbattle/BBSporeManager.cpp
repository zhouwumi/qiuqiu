#include "BBSporeManager.h"
#include "BBGameManager.h"
#include "BBMathUtils.h"

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

Spore* BBSporeManager::CreateSpore(PlayerNode* node)
{
	Spore* newSpore = gameManager->objectManager.CreateSpore(node);
	FixSpore(newSpore);
	mapSpores.emplace(newSpore->Idx, newSpore);
	gameManager->NodeTree.AddCircleNode(newSpore);
	gameManager->frameCacheManager.AddNewSpore(newSpore->Idx);
	vecSporeIds.emplace_back(newSpore->Idx);
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
	BBPoint& point = spore->Location;
	bool isFixedX, isFixedY;
	int fixedX, fixedY;
	BBMathUtils::FixCircle(gameManager->mapRect, point.x, point.y, spore->radius, fixedX, fixedY, isFixedX, isFixedY);
	spore->ChangePosition(fixedX, fixedY);
}

void BBSporeManager::MoveSpores()
{
	for (auto iter : mapSpores)
	{
		Spore* spore = iter.second;
		if (spore->Init <= 0)
		{
			continue;
		}

		BBVector& newSpeed = spore->InitMove();
		spore->FromLocation = spore->Location;
		spore->ChangePosition(spore->Location.x + newSpeed.x, spore->Location.y + newSpeed.y);
		FixSpore(spore);
		spore->Init -= 1;
		if (spore->Cd > 0)
		{
			spore->Cd -= 1;
		}
		gameManager->NodeTree.UpdateCircleNode(spore);
	}
}