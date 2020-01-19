#include "BBMoveCommand.h"
#include "BBMoveManager.h"

BBMoveCommand::BBMoveCommand()
{

}

BBMoveCommand::~BBMoveCommand()
{

}

BBMoveCommand BBMoveCommand::Create()
{
	BBMoveCommand ret;
	ret.angle = BBMoveManager::angle;
	ret.pressure = BBMoveManager::pressure;
	ret.isShoot = BBMoveManager::isShoot;
	ret.isSplit = BBMoveManager::isSplit;
	ret.checkSum = CHECKSUM_MISMATCH;
	ret.id = BBMoveManager::nextIndex;
	return ret;
}