#ifndef BB_SERVER_GAME_MANAGER_CPP
#define BB_SERVER_GAME_MANAGER_CPP
#include "BBGameManager.h"

class BBServerGameManager :public BBGameManager
{
public:
	static BBServerGameManager* Create();
	BBServerGameManager();
	~BBServerGameManager();

	void InitFoodSpiky();
	std::vector<int> GetEatFoods();
	std::vector<int> GetEatSpores();
	std::vector<int> GetEatSpikys();

	

	void AddOperatePlayerJoin(int uid);//ÕÊº“º”»Î
	void AddPlayerQuit(int uid);

	unsigned int GetAckCommand(unsigned int uid);
	bool IsPlayerCommandMatch(unsigned int uid);

	std::vector<int> GetAllFoodInfos();
	std::vector<int> GetAllSpikyInfos();
	std::vector<int>& GetAllSporeInfos();
	std::vector<int>& GetAllPlayerIdxs();
	

	std::vector<int>& GetFrameNewPlayer();
	std::vector<int>& GetFrameNewFood();
	std::vector<int>& GetFrameNewSpiky();
	std::vector<int>& GetFrameNewSpore();
	

	std::vector <std::string> GetServerLog();
	void ClearServerLog();

	std::vector<int> GetCrcs(unsigned int uid);
};
#endif
