#include "GridMap.h"
#include<algorithm>

GridNode::GridNode()
{
	//objectIdxs.reserve(15);
}

GridMap::GridMap()
{
	
}

void GridMap::SetMap(int w, int h, int pDepth, int objNum)
{
	mapW = w;
	mapH = h;
	depth = pDepth;

	int size = pow(2, 2 * depth - 2);
	gridNodes.resize(size);
	sideSizeLen = pow(2, depth - 1);
	sideLen = (float)mapW / sideSizeLen;
	for (int i = 0; i < sideSizeLen; i++)
	{
		for (int j = 0; j < sideSizeLen; j++)
		{
			int gridIdx = j * sideSizeLen + i;
			GridNode& node = gridNodes[gridIdx];
			node.minX = i * sideLen;
			node.maxX = (i + 1) * sideLen;
			node.minY = j * sideLen;
			node.maxY = (j + 1) * sideLen;
		}
	}

	mapAllObjects.reserve(objNum);
}

GridMap::~GridMap()
{
}

void GridMap::AddObjectGridKey(BaseCircleNode* node, int gridX, int gridY)
{
	int index = gridY * sideSizeLen + gridX;
	if (gridNodes.size() > index)
	{
		GridNode& gridNode = gridNodes[index];
		gridNode.objectIdxs.emplace(node->Idx, node);
		mapAllObjects[node] = index;
	}
}

void GridMap::AddObjectMapKey(BaseCircleNode* node, int mapX, int mapY)
{
	int gridX = floor(mapX / sideLen);
	int gridY = floor(mapY / sideLen);
	AddObjectGridKey(node, gridX, gridY);
}

void GridMap::RemoveObject(BaseCircleNode* node)
{
	if (mapAllObjects.find(node) != mapAllObjects.end())
	{
		int gridIdx = mapAllObjects[node];
		if (gridNodes.size() > gridIdx)
		{
			GridNode& gridNode = gridNodes[gridIdx];
			gridNode.objectIdxs.erase(node->Idx);
		}
		mapAllObjects.erase(node);
	}
}

void GridMap::GetAllHitIdxs(BBRect rect, std::vector<int>& idxs)
{
	int minGridX = floor(rect.minX / sideLen);
	int minGridY = floor(rect.minY / sideLen);
	int maxGridX = floor(rect.maxX / sideLen);
	int maxGridY = floor(rect.maxY / sideLen);

	for (int i = minGridX; i <= maxGridX; i++)
	{
		for (int j = minGridY; j <= maxGridY; j++)
		{
			int index = j * sideSizeLen + i;
			if (gridNodes.size() > index)
			{
				GridNode& node = gridNodes[index];
				for (auto iter : node.objectIdxs)
				{
					idxs.emplace_back(iter.first);
				}
			}
		}
	}
	std::sort(idxs.begin(), idxs.end());
}

void GridMap::Clear()
{
	for (int i = 0; i < gridNodes.size(); i++)
	{
		GridNode& gridNode = gridNodes[i];
		gridNode.objectIdxs.clear();
	}
	mapAllObjects.clear();
}