#ifndef BB_CONFIG_MANAGER_H
#define BB_CONFIG_MANAGER_H

class BBConfigManager
{
public:
	BBConfigManager();
	~BBConfigManager();

	static bool isOpenLog;
	static int maxPredictionCnt;
	static int maxOtherStepCommandCnt;
	static int maxCommandNoAck;

	static int minCatchUpTicks;
	static int maxCatchUpTicks;

	static int protectTime;

	static void SetLogState(bool open);
	static void SetMaxPredictionCnt(int cnt);
	static void SetMaxOtherStepCommandCnt(int cnt);
	static void SetMaxCommandNoAck(int cnt);
	static void SetMinCatchUpTicks(int cnt);
	static void SetMaxCatchUpTicks(int cnt);

	static void SetProtectTime(int cnt);
};

#endif