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

	//add frame 
	static void IncreaseFrame();
	static void ResetManager();
	static void ResetFrame();
	static void SetMove(int angle, int pressure);
	static void Split();
	static void Shoot();
};

#endif //BB_MOVE_MANAGER_H

