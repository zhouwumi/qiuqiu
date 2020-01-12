#ifndef BB_CONST_CPP
#define BB_CONST_CPP

#define BB_M_PI       3.14159
#define BB_MATH_TOLERANCE              2e-37f

enum ObjectType
{
	Type_PlayerNode = 1,
	Type_Food = 2,
	Type_Spiky = 3,
	Type_Spore = 4
};

class BBConst
{
public:
	BBConst();
	~BBConst();

public:
	static const int FPS;
	static const int SyncFPS;
	static const int FixFps;
	static const int FixLength;

	static const int MaxHeight;
	static const int MaxWidth;
	static const int InitMass;
	static const int InitWidth;
	static const int InitHeight;
	static const int MoveBase;
	static const int MoveRate;
	static const int MaxCell;
	static const int MinSplitMass;
	static const int SpikyChild;
	static const int SplitFrame;
	static const int SplitInitSpeed;
	static const int SplitFinalSpeed;
	static const int RoomCapacity;
	static const int GameTime;
	static const int FoodNum;
	static const int FoodMass;
	static const int MergeCD;
	static const int SpikyNum;
	static const int DeltaBase;
	static const int Delta;
	static const int ShootMinMass;
	static const int SporeMass;
	static const int SporeInitSpeed;
	static const int SporeStopFrame;
	static const int SporeStopSpeed;
	static const int SporeCd;
	static const int RespawnCost;
	static const int RespawnTime;
	static const int GripRadius;
	static const int ProtectTime;

	static const int MinSpikyMass;
	static const int MaxSpikyMass;

	static const int MinPartition;
	static const int MaxPartition;

	static const int SpikyTime;
};

#endif
