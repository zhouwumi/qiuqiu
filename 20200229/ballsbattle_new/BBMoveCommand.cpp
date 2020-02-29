#include "BBMoveCommand.h"
#include "BBMoveManager.h"

BBMoveCommand::BBMoveCommand():
	isShoot(false),
	isSplit(false),
	angle(0),
	pressure(0),
	checkSum(0),
	idx(0)
{

}

BBMoveCommand::~BBMoveCommand()
{

}

void BBMoveCommand::ClearData()
{
	isShoot = false;
	isSplit = false;
	angle = 0;
	pressure = 0;
	checkSum = 0;
	idx = 0;
}

BBMoveCommand BBMoveCommand::Create()
{
	BBMoveCommand ret;
	ret.angle = BBMoveManager::angle;
	ret.pressure = BBMoveManager::pressure;
	ret.isShoot = BBMoveManager::isShoot;
	ret.isSplit = BBMoveManager::isSplit;
	ret.checkSum = CHECKSUM_MISMATCH;
	ret.idx = BBMoveManager::nextIndex;
	return ret;
}

bool BBMoveCommand::IsEqual(BBSimplePrediction& command)
{
	if (this->isShoot || this->isSplit)
	{
		return false;
	}
	return this->angle == command.angle && this->pressure == command.pressure;
}