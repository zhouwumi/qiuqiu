#ifndef BB_CONFIG_MANAGER_H
#define BB_CONFIG_MANAGER_H

class BBConfigManager
{
public:
	BBConfigManager();
	~BBConfigManager();

	static bool clientDebug;

	static int fixLength;
	static int spikyTime;

	static int gameWidth;
	static int gameHeight;

	static int maxBallCount;
	static int foodCount;
	static int foodMass;
	static int spikyCount;

	static int initBallMass;
	static int minSpikyMass;
	static int maxSpikyMass;
	static int maxSpikySplitChildCount;

	static int minShootBallMass;
	static int sporeMass;
	static int sporeStopFrame;
	static int sporeInitSpeed;
	static int sporeCdTime;

	static int minSplitMass;
	static int splitStopFrame;
	static int splitInitSpeed;

	static int ballMoveRate;
	static double ballEatRate;
	
	static double massToSpeedControllerRate;

	static bool isOpenLog;
	static int maxPredictionCnt;
	static int maxOtherStepCommandCnt;
	static int maxCommandNoAck;

	static int minMyCatchUpTicks;
	static int maxMyCatchUpTicks;
	static int minOtherCatchUpTicks;
	static int maxOtherCatchUpTicks;
	static double otherCatchRate;

	static int otherTooSlowTicks; //some one network too slow don't update

	static int protectTime;
	static int protectGripRadius;
	static int mergeCdTime;

	static int debugMyBallMass;
	static int debugOtherBallMass;

	static void SetClientDebug(bool isDebug);

	static void SetFixLength(int cnt);
	static void SetSpikyTime(int cnt);
	static void SetGameWH(int w, int h);
	static void SetMaxBallCount(int cnt);
	static void SetFoodCount(int cnt);
	static void SetFoodMass(int mass);
	static void SetSpikyCount(int cnt);

	static void SetInitBallMass(int mass);
	static void SetMinSpikyMass(int mass);
	static void SetMaxSpikyMass(int mass);
	static void SetMaxSpikySplitChildCount(int cnt);

	static void SetMinShootBallMass(int mass);
	static void SetSporeMass(int mass);
	static void SetSporeStopFrame(int frame);
	static void SetSporeInitSpeed(int speed);
	static void SetSporeCdTime(int time);

	static void SetSplitStopFrame(int frame);
	static void SetSplitInitSpeed(int speed);
	static void SetMinSplitMass(int mass);
	static void SetBallMoveRate(int rate);
	static void SetBallEatRate(double rate);
		
	static void SetLogState(bool open);
	static void SetMaxPredictionCnt(int cnt);
	static void SetMaxOtherStepCommandCnt(int cnt);
	static void SetMaxCommandNoAck(int cnt);
	static void SetMinCatchUpTicks(int cnt);
	static void SetMaxCatchUpTicks(int cnt);
	static void SetMinOtherCatchUpTicks(int cnt);
	static void SetMaxOtherCatchUpTicks(int cnt);
	static void SetOtherCatchSpeedRate(double rate);
	static void SetOtherTooSlowTicks(int cnt);

	static void SetProtectTime(int cnt);
	static void SetProtectGripRadius(int radius);
	static void SetMergeCdTime(int cnt);

	static void SetDebugMyBallMass(int cnt);
	static void SetDebugOtherBallMas(int cnt);

	static void SetMassToSpeedControllerRate(double rate);
};

#endif