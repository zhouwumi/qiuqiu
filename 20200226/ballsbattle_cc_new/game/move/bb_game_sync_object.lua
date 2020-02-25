BBGameSyncObject = CreateClass()

local BBGameBallDeltaData = import('logic.dialog.ballsbattle_cc_new.game.data.bb_game_frame_data').BBGameBallDeltaData
local BBGameMoveCommandList = import('logic.dialog.ballsbattle_cc_new.game.move.bb_game_move_command').BBGameMoveCommandList


function BBGameSyncObject:__init__()
	self.mDelta = BBGameBallDeltaData:New()
	self.mCommandList = BBGameMoveCommandList:New()
end

function BBGameSyncObject:clientCatchup()
end

function BBGameSyncObject:processTick()
end