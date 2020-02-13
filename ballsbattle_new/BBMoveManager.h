#ifndef BB_MOVE_MANAGER_H
#define BB_MOVE_MANAGER_H

class BBMoveManager
{
public:
	static int angle;
	static int pressure;
	static bool isSplit; //split command
	static bool isShoot; //shoot command
	static int nextIndex;
	static unsigned int checkSum;

	//add frame 
	static void IncreaseFrame();
	static void ResetManager();
	static void ResetFrame();
	static void SetMove(int angle, int pressure);
	static void Split();
	static void Shoot();

	static void SetCheckSum(unsigned int checkSum);

	static bool IsSplit();
	static bool IsShoot();
	static int GetAngle();
	static int GetPressure();
	static int GetNextIndex();
	static unsigned int GetCheckSum();
};

#endif //BB_MOVE_MANAGER_H

