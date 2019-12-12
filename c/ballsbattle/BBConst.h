#pragma once

#define BB_M_PI       3.14159265358979323846
#define BB_MATH_TOLERANCE              2e-37f

class BBConst
{
public:
	BBConst();
	~BBConst();

public:
	static const int FPS;

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
};

