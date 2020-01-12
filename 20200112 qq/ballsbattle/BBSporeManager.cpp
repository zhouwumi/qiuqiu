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
	for (const auto& iter : mapSpores)
	{
		Spore* spore = iter.second;
		if (spore->Init <= 0)
		{
			continue;
		}

		BBVector& newSpeed = spore->InitMove();
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

void BBSporeManager::AddNewSporeFromServer(int idx, int fromId, int uid, int mass, int x, int y, int directionX, int directionY, int currentX, int currentY, int curSpeed, int deltaSpeed, int Init, int Cd)
{
	Spore* newSpore = new Spore();
	newSpore->FromId = fromId;
	newSpore->Idx = idx;
	newSpore->Uid = uid;
	newSpore->ChangePosition(x, y);
	newSpore->SetBallMass(BBConst::SporeMass);
	newSpore->Direction.SetPoint(directionX, directionY);
	newSpore->Current.SetPoint(currentX, currentY);
	newSpore->Init = Init;
	newSpore->Cd = Cd;
	newSpore->initSpeed = curSpeed;
	newSpore->initDeltaSpeed = deltaSpeed;
	newSpore->SetBallMass(mass);

	mapSpores.emplace(newSpore->Idx, newSpore);
	gameManager->NodeTree.AddCircleNode(newSpore);
	gameManager->frameCacheManager.AddNewSpore(newSpore->Idx);
	vecSporeIds.emplace_back(newSpore->Idx);
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
		sporeBuffer[index++] = spore->Location.x;
		sporeBuffer[index++] = spore->Location.y;
	}

	unsigned int size = index * sizeof(int) / sizeof(char);
	unsigned int ret = BBMathUtils::crc32((char*)(&sporeBuffer), size);
	return ret;
}

void BBSporeManager::MakePhoto(int keyFrame)
{
	for (const auto& iter : mapSpores)
	{
		Spore* spore = iter.second;
		spore->MakePhoto(keyFrame);
	}
}

void BBSporeManager::ResetPhoto(int keyFrame)
{
	for (const auto& iter : mapSpores)
	{
		Spore* spore = iter.second;
		spore->ResetPhoto(keyFrame);
	}
}