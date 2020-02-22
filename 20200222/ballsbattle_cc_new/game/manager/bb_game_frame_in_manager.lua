BBGameFrameInManager = CreateClass()
local BBGameMoveCommand = import('logic.dialog.ballsbattle_cc_new.game.move.bb_game_move_command').BBGameMoveCommand

function BBGameFrameInManager:__init__(gameManager)
	self.gameManager = gameManager
	self.playerCommands = {}
	self.playerQuitIds = {}
	self.playerJoinIds = {}
end

function BBGameFrameInManager:FrameClear()
	self.playerCommands = {}
	self.playerQuitIds = {}
	self.playerJoinIds = {}
end

function BBGameFrameInManager:AddPlayerQuit(uid)
	self.playerQuitIds[uid] = true
end

function BBGameFrameInManager:AddPlayerJoin(uid)
	self.playerJoinIds[uid] = true
end

function BBGameFrameInManager:AddPlayerCommand(uid, angle, pressure, isSplit, isShoot, idx, checkSum)
	local command = BBGameMoveCommand:New()
	command.angle = angle;
	command.pressure = pressure;
	command.isSplit = isSplit;
	command.isShoot = isShoot;
	command.idx = idx;
	command.checkSum = checkSum;
	local vec = self.playerCommands[uid]
	if not vec then
		vec = {}
		self.playerCommands[uid] = vec
	end
	table.insert(vec, command)
end