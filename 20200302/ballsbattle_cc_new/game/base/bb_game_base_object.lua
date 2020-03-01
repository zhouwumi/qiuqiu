BBGameBaseObject = CreateClass()

local BBGameRect = import('logic.dialog.ballsbattle_cc_new.game.base.bb_game_rect').BBGameRect
local BBGameMathUtils = import('logic.dialog.ballsbattle_cc_new.game.math.bb_game_math_utils').BBGameMathUtils

function BBGameBaseObject:__init__()
	self.location = {x = 0, y = 0}
	self.rect = BBGameRect:New()
	self.idx = -1
	self.treeIndex = -1
	self.radius = 0
end

function BBGameBaseObject:OnRemoveFromTreeNode()
	self.treeIndex = -1
end

function BBGameBaseObject:ChangeTreeIndex(newIdx)
	self.treeIndex = newIdx
end

function BBGameBaseObject:ChangePosition(x, y)
	self.location.x = BBGameMathUtils.bb_fix_float(x)
	self.location.y =  BBGameMathUtils.bb_fix_float(y)
	self.rect:SetRect(self.location.x - self.radius, self.location.y - self.radius, self.location.x + self.radius, self.location.y + self.radius)
end

function BBGameBaseObject:GetRadius()
	return self.radius
end

function BBGameBaseObject:SetRadius(r)
	self.radius = r
end

