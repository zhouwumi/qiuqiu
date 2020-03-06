#ifndef BB_GRID_MAP_CPP
#define BB_GRID_MAP_CPP
#include<vector>
#include<unordered_map>
#include "BaseCircleNode.h"
#include "BBRect.h"

class GridNode
{
public:
	GridNode();
public:
	std::unordered_map<int, BaseCircleNode*> objectIdxs;
	int minX;
	int maxX;
	int minY;
	int maxY;
};

class GridMap
{
public:
	GridMap();
	void SetMap(int w, int h, int pDepth, int objNum);
	~GridMap();

	void AddObjectGridKey(BaseCircleNode* node, int gridX, int gridY);
	void AddObjectMapKey(BaseCircleNode* node, int mapX, int mapY);
	void RemoveObject(BaseCircleNode* node);

	void GetAllHitIdxs(BBRect rect, std::vector<int>& idxs);

	void Clear();
public:
	std::vector<GridNode> gridNodes;
	std::unordered_map<BaseCircleNode*, int> mapAllObjects;
	int sideSizeLen;
	double sideLen;
private:
	int depth;
	int mapW;
	int mapH;
};

#endif

