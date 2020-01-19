#ifndef BB_FRAME_DATA_H
#define BB_FRAME_DATA_H

#include<vector>
#include "BaseFrameSyncData.h"

using namespace std;

class BBSyncBallData
{
public:
	BBSyncBallData();
	~BBSyncBallData();

public:
	int uniqueId; //absolute unique
	int mass; //mass
	int fromId; //generate from id ball
	int uid; // belong player
	int cd; //cd fusion cd time
	int springTime; // spring move time
	int springSpeed;
	int springDeltaSpeed;
	int protect; //birth can't eat and beEat time
};

class BBFrameData :public BaseFrameData
{
public:
	BBFrameData();
	int uid;
	int NMass;
	vector<BBSyncBallData> ballDatas;
};

#endif //BB_FRAME_DATA_H
