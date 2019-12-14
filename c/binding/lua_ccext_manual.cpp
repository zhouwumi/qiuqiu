#include "lua_ccext_manual.h"
#include "cc_ext/DrawNode3D.h"

#if CCEXT_USE_LIVE2DVER2
#include "scripting/lua-bindings/auto/lua_ccext_live2d_ver2_auto.hpp"
#include "cc_ext/Live2DSprite.h"
#include "Live2D.h"
#ifdef L2D_TARGET_ANDROID_ES2
#include "graphics/DrawParam_OpenGLES2.h"
#include "base/CCEventListenerCustom.h"
#endif
using namespace live2d;
#endif

#if CCEXT_USE_LIVE2DVER3
#include "scripting/lua-bindings/auto/lua_ccext_live2d_ver3_auto.hpp"
#include "cc_ext/live2d_version_3/Live2DSprite.h"
#endif

#if CCEXT_USE_FMOD_CORE
#include "scripting/lua-bindings/auto/lua_ccext_fmod_core_auto.hpp"
#endif

#include "cocos2d.h"

#include "scripting/lua-bindings/auto/lua_ccext_auto.hpp"
#include "scripting/lua-bindings/auto/lua_ccext_logic_auto.hpp"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

#include "scripting/lua-bindings/manual/CCLuaValue.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"

#include "scripting/lua-bindings/manual/ext/logic/ballsbattle/BBGameManager.h"

int lua_cocos2dx_DrawNode3D_drawCube(lua_State* L)
{
    int argc = 0;
    cocos2d::DrawNode3D* cobj = nullptr;
    bool ok  = true;
    
#if 1
    tolua_Error tolua_err;
#endif
    
    
#if 1
    if (!tolua_isusertype(L,1,"cc.DrawNode3D",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (cocos2d::DrawNode3D*)tolua_tousertype(L,1,0);
    
#if 1
    if (!cobj)
    {
        tolua_error(L,"invalid 'cobj' in function 'lua_cocos2dx_DrawNode3D_drawCube'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(L)-1;
    if (argc == 2)
    {
        std::vector<cocos2d::Vec3> arg0;
        cocos2d::Color4F arg1;
        Vec3 vec3;
#if 1
        if (!tolua_istable(L, 2, 0, &tolua_err))
            goto tolua_lerror;
#endif
        size_t size = lua_objlen(L, 2);
        for (int i = 0; i < size; i++)
        {
            lua_pushnumber(L, i + 1);
            lua_gettable(L, 2);
#if 1
            if (!tolua_istable(L, -1, 0, &tolua_err))
            {
                lua_pop(L, 1);
                goto tolua_lerror;
            }
#endif
            ok &= luaval_to_vec3(L, lua_gettop(L), &vec3);
            
#if 1
            if (!ok)
            {
                lua_pop(L, 1);
                goto tolua_lerror;
            }
#endif
            //arg0[i] = vec3;
            arg0.push_back(vec3);
            lua_pop(L, 1);
        }
        
        ok &=luaval_to_color4f(L, 3, &arg1, "cc.DrawNode3D:drawCube");
        if(!ok)
            return 0;
        cobj->drawCube(&arg0[0], arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "cc.DrawNode3D:drawCube",argc, 2);
    return 0;
    
#if 1
tolua_lerror:
    tolua_error(L,"#ferror in function 'lua_cocos2dx_DrawNode3D_drawCube'.",&tolua_err);
#endif
    
    return 0;
}

int extendDrawNode3D(lua_State* tolua_S) {
	lua_pushstring(tolua_S, "cc.DrawNode3D");
	lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
		tolua_function(tolua_S, "drawCube", lua_cocos2dx_DrawNode3D_drawCube);
    }
    lua_pop(tolua_S, 1);

	return 1;
}

static int tolua_cocos2d_BBGameManager_GetPlayerNodeInfo(lua_State* tolua_S)
{
	if (NULL == tolua_S)
		return 0;

	int argc = 0;
	BBGameManager* self = nullptr;

#if 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "BBGameManager", 0, &tolua_err)) goto tolua_lerror;
#endif

	self = static_cast<BBGameManager*>(tolua_tousertype(tolua_S, 1, 0));
#if 1
	if (nullptr == self) {
		tolua_error(tolua_S, "invalid 'self' in function 'tolua_cocos2d_BBGameManager_GetPlayerNodeInfo'\n", NULL);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{
#if 1
		if (!tolua_isnumber(tolua_S, 2, 1, &tolua_err))
			goto tolua_lerror;
#endif
		float x = (float)tolua_tonumber(tolua_S, 2, 0);

		PlayerNode* node = self->GetPlayerNodeInfo(x);
		if (node)
		{
			tolua_pushnumber(tolua_S, (lua_Number)node->positionX);
			tolua_pushnumber(tolua_S, (lua_Number)node->positionY);
			tolua_pushnumber(tolua_S, (lua_Number)node->radius);
			return 3;
		}
		return 0;
	}

	luaL_error(tolua_S, "%s function in Node has wrong number of arguments: %d, was expecting %d\n", "BBGameManager:GetPlayerNodeInfo", argc, 0);
	return 0;

#if 1
	tolua_lerror:
		tolua_error(tolua_S, "#ferror in function 'tolua_cocos2d_BBGameManager_GetPlayerNodeInfo'.", &tolua_err);
	return 0;
#endif
}
int tolua_cocos2d_BBGameManager_SetEatFoodCbFunction(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;
	BBGameManager* cobj = nullptr;
#if 1
	tolua_Error tolua_err;
#endif

#if 1
	if (!tolua_isusertype(tolua_S, 1, "BBGameManager", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (BBGameManager*)tolua_tousertype(tolua_S, 1, 0);

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{
#if 1
		if (!toluafix_isfunction(tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err))
			goto tolua_lerror;
#endif

		LUA_FUNCTION handler = toluafix_ref_function(tolua_S, 2, 0);

		std::function<void(int)> arg1 = [handler](int hitIdx) {
			LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
			stack->pushInt(hitIdx);
			stack->executeFunctionByHandler(handler, 1);
		};
		cobj->SetEatFoodCbFunction(arg1);
		tolua_pushnumber(tolua_S, handler);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "cc.BBGameManager:SetHitCBFunction", argc, 1);
	return 0;
#if 1
	tolua_lerror:
		tolua_error(tolua_S, "#ferror in function 'tolua_cocos2d_BBGameManager_SetHitCBFunction'.", &tolua_err);
#endif
	return 0;
}

int lua_cocos2dx_BBGameManager_RemoveEatFoodCbFunction(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;
	BBGameManager* cobj = nullptr;
#if 1
	tolua_Error tolua_err;
#endif

#if 1
	if (!tolua_isusertype(tolua_S, 1, "BBGameManager", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (BBGameManager*)tolua_tousertype(tolua_S, 1, 0);

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{
		int handler;
		ok &= luaval_to_int32(tolua_S, 2, &handler, "cc.BBGameManager:RemoveHitCBFunction");
		LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
		stack->removeScriptHandler(handler);

		cobj->RemoveEatFoodCbFunction();
		lua_settop(tolua_S, 1);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "cc.BBGameManager:RemoveHitCBFunction", argc, 1);
	return 0;
#if 1
	tolua_lerror:
		tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_BBGameManager_RemoveHitCBFunction'.", &tolua_err);
#endif
	return 0;
}

int extendBBGameManager(lua_State* tolua_S) {
	lua_pushstring(tolua_S, "BBGameManager");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S, -1))
	{
		tolua_function(tolua_S, "GetPlayerNodeInfo", tolua_cocos2d_BBGameManager_GetPlayerNodeInfo);
		tolua_function(tolua_S, "SetEatFoodCbFunction", tolua_cocos2d_BBGameManager_SetEatFoodCbFunction);
		tolua_function(tolua_S, "RemoveEatFoodCbFunction", lua_cocos2dx_BBGameManager_RemoveEatFoodCbFunction);
	}
	lua_pop(tolua_S, 1);

	return 1;
}


#if CCEXT_USE_LIVE2DVER2
int lua_cocos2dx_Live2DSprite_SetUpdateCallback(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;
	Live2DSprite* cobj = nullptr;
#if 1
	tolua_Error tolua_err;
#endif

#if 1
	if (!tolua_isusertype(tolua_S, 1, "Live2DSprite", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (Live2DSprite*)tolua_tousertype(tolua_S, 1, 0);

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{
#if 1
		if (!toluafix_isfunction(tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err))
			goto tolua_lerror;
#endif

		LUA_FUNCTION handler = toluafix_ref_function(tolua_S, 2, 0);

		std::function<void()> arg1 = [handler]() {
			LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
			stack->executeFunctionByHandler(handler, 0);
		};
		cobj->SetUpdateCallback(arg1);
		tolua_pushnumber(tolua_S, handler);
		//lua_settop(tolua_S, 1);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "cc.Live2DSprite:SetUpdateCallback", argc, 1);
	return 0;
#if 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_Live2DSprite_SetUpdateCallback'.", &tolua_err);
#endif
				return 0;
}

int lua_cocos2dx_Live2DSprite_RemoveUpdateCallback(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;
	Live2DSprite* cobj = nullptr;
#if 1
	tolua_Error tolua_err;
#endif

#if 1
	if (!tolua_isusertype(tolua_S, 1, "Live2DSprite", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (Live2DSprite*)tolua_tousertype(tolua_S, 1, 0);

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{
		int handler;
		ok &= luaval_to_int32(tolua_S, 2, &handler, "cc.Live2DSprite:RemoveUpdateCallback");
		LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
		stack->removeScriptHandler(handler);

		cobj->RemoveUpdateCallback();
		lua_settop(tolua_S, 1);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "cc.Live2DSprite:RemoveUpdateCallback", argc, 1);
	return 0;
#if 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_Live2DSprite_RemoveUpdateCallback'.", &tolua_err);
#endif
				return 0;
}



int extendLive2DVer2Sprite(lua_State* tolua_S) {
    Live2D::init();

#ifdef L2D_TARGET_ANDROID_ES2
    char *exts = (char*)glGetString(GL_EXTENSIONS);
    if (strstr(exts, "GL_NV_shader_framebuffer_fetch ")) {
        live2d::DrawParam_OpenGLES2::setExtShaderMode(true, true);
    }

    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(EVENT_RENDERER_RECREATED,
        [](EventCustom*)
    {
        live2d::DrawParam_OpenGLES2::reloadShader();
    }), -1);
#endif

	lua_pushstring(tolua_S, "Live2DSprite");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S, -1))
	{
		tolua_function(tolua_S, "SetUpdateCallback", lua_cocos2dx_Live2DSprite_SetUpdateCallback);
		tolua_function(tolua_S, "RemoveUpdateCallback", lua_cocos2dx_Live2DSprite_RemoveUpdateCallback);
	}
	lua_pop(tolua_S, 1);

	return 1;
}
#endif

#if CCEXT_USE_LIVE2DVER3

int extendLive2DVer3Sprite(lua_State* tolua_S) {
	// init
	Live2DSpriteVer3::init_live2d();
	return 1;
}
#endif

TOLUA_API int register_all_ccext_manual(lua_State* tolua_S)
{
    register_all_ccext(tolua_S);
    register_all_ccext_logic(tolua_S);

    extendDrawNode3D(tolua_S);
	extendBBGameManager(tolua_S);

#if CCEXT_USE_LIVE2DVER2
	register_all_ccext_live2d_ver2(tolua_S);
	extendLive2DVer2Sprite(tolua_S);
#endif

#if CCEXT_USE_LIVE2DVER3
	register_all_ccext_live2d_ver3(tolua_S);
	extendLive2DVer3Sprite(tolua_S);
#endif

#if CCEXT_USE_FMOD_CORE
	register_all_ccext_fmod_core(tolua_S);
#endif

	return 1;
}
