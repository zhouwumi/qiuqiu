#include "scripting/lua-bindings/auto/lua_ccext_logic_auto.hpp"
#include "scripting/lua-bindings/manual/ext/logic/fish/FishCocosMapManager.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "scripting/lua-bindings/manual/ext/logic/ballsbattle/BBGameManager.h"

int lua_ccext_logic_FishCocosMapManager_AddCocosBulletObject(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_AddCocosBulletObject'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        cocos2d::Node* arg0;
        int arg1;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0, "FishCocosMapManager:AddCocosBulletObject");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "FishCocosMapManager:AddCocosBulletObject");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_AddCocosBulletObject'", nullptr);
            return 0;
        }
        cobj->AddCocosBulletObject(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:AddCocosBulletObject",argc, 2);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_AddCocosBulletObject'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_GetDebugFishRegions(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_GetDebugFishRegions'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:GetDebugFishRegions");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_GetDebugFishRegions'", nullptr);
            return 0;
        }
        cocos2d::ValueVector ret = cobj->GetDebugFishRegions(arg0);
        ccvaluevector_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:GetDebugFishRegions",argc, 1);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_GetDebugFishRegions'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_RemoveCocosFishObject(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_RemoveCocosFishObject'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:RemoveCocosFishObject");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_RemoveCocosFishObject'", nullptr);
            return 0;
        }
        cobj->RemoveCocosFishObject(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:RemoveCocosFishObject",argc, 1);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_RemoveCocosFishObject'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_CheckBulletHit(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_CheckBulletHit'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:CheckBulletHit");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_CheckBulletHit'", nullptr);
            return 0;
        }
        cocos2d::ValueVector ret = cobj->CheckBulletHit(arg0);
        ccvaluevector_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:CheckBulletHit",argc, 1);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_CheckBulletHit'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_TranslateFishGridInfo2MapInfo(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_TranslateFishGridInfo2MapInfo'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:TranslateFishGridInfo2MapInfo");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_TranslateFishGridInfo2MapInfo'", nullptr);
            return 0;
        }
        cobj->TranslateFishGridInfo2MapInfo(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:TranslateFishGridInfo2MapInfo",argc, 1);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_TranslateFishGridInfo2MapInfo'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_GetDebugHitLineInfo(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_GetDebugHitLineInfo'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:GetDebugHitLineInfo");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_GetDebugHitLineInfo'", nullptr);
            return 0;
        }
        cocos2d::ValueVector ret = cobj->GetDebugHitLineInfo(arg0);
        ccvaluevector_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:GetDebugHitLineInfo",argc, 1);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_GetDebugHitLineInfo'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_GetFishIdsInGrid(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_GetFishIdsInGrid'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        int arg0;
        int arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:GetFishIdsInGrid");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "FishCocosMapManager:GetFishIdsInGrid");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_GetFishIdsInGrid'", nullptr);
            return 0;
        }
        cocos2d::ValueVector ret = cobj->GetFishIdsInGrid(arg0, arg1);
        ccvaluevector_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:GetFishIdsInGrid",argc, 2);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_GetFishIdsInGrid'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_init(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_init'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_init'", nullptr);
            return 0;
        }
        bool ret = cobj->init();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:init",argc, 0);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_init'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_RemoveCurvePath(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_RemoveCurvePath'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:RemoveCurvePath");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_RemoveCurvePath'", nullptr);
            return 0;
        }
        cobj->RemoveCurvePath(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:RemoveCurvePath",argc, 1);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_RemoveCurvePath'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_ReSetMapInfo(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_ReSetMapInfo'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_ReSetMapInfo'", nullptr);
            return 0;
        }
        cobj->ReSetMapInfo();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:ReSetMapInfo",argc, 0);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_ReSetMapInfo'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_IsFishInVisible(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_IsFishInVisible'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:IsFishInVisible");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_IsFishInVisible'", nullptr);
            return 0;
        }
        bool ret = cobj->IsFishInVisible(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:IsFishInVisible",argc, 1);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_IsFishInVisible'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_UpdateFishRegion(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_UpdateFishRegion'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:UpdateFishRegion");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_UpdateFishRegion'", nullptr);
            return 0;
        }
        cobj->UpdateFishRegion(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:UpdateFishRegion",argc, 1);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_UpdateFishRegion'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_SetMapSize(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_SetMapSize'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        cocos2d::Vec2 arg0;
        int arg1;
        int arg2;

        ok &= luaval_to_vec2(tolua_S, 2, &arg0, "FishCocosMapManager:SetMapSize");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "FishCocosMapManager:SetMapSize");

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "FishCocosMapManager:SetMapSize");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_SetMapSize'", nullptr);
            return 0;
        }
        cobj->SetMapSize(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:SetMapSize",argc, 3);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_SetMapSize'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_GetMaxScoreFishInGrid(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_GetMaxScoreFishInGrid'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        int arg0;
        int arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:GetMaxScoreFishInGrid");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "FishCocosMapManager:GetMaxScoreFishInGrid");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_GetMaxScoreFishInGrid'", nullptr);
            return 0;
        }
        int ret = cobj->GetMaxScoreFishInGrid(arg0, arg1);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:GetMaxScoreFishInGrid",argc, 2);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_GetMaxScoreFishInGrid'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_SetMaxHitLinePointCount(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_SetMaxHitLinePointCount'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:SetMaxHitLinePointCount");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_SetMaxHitLinePointCount'", nullptr);
            return 0;
        }
        cobj->SetMaxHitLinePointCount(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:SetMaxHitLinePointCount",argc, 1);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_SetMaxHitLinePointCount'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_IsFishOnGrid(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_IsFishOnGrid'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        int arg0;
        int arg1;
        int arg2;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:IsFishOnGrid");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "FishCocosMapManager:IsFishOnGrid");

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "FishCocosMapManager:IsFishOnGrid");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_IsFishOnGrid'", nullptr);
            return 0;
        }
        bool ret = cobj->IsFishOnGrid(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:IsFishOnGrid",argc, 3);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_IsFishOnGrid'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_AddCocosFishObject(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_AddCocosFishObject'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 4) 
    {
        cocos2d::Node* arg0;
        int arg1;
        int arg2;
        int arg3;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0, "FishCocosMapManager:AddCocosFishObject");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "FishCocosMapManager:AddCocosFishObject");

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "FishCocosMapManager:AddCocosFishObject");

        ok &= luaval_to_int32(tolua_S, 5,(int *)&arg3, "FishCocosMapManager:AddCocosFishObject");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_AddCocosFishObject'", nullptr);
            return 0;
        }
        cobj->AddCocosFishObject(arg0, arg1, arg2, arg3);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:AddCocosFishObject",argc, 4);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_AddCocosFishObject'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_CheckBulletHitFishId(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_CheckBulletHitFishId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        int arg0;
        int arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:CheckBulletHitFishId");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "FishCocosMapManager:CheckBulletHitFishId");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_CheckBulletHitFishId'", nullptr);
            return 0;
        }
        cocos2d::ValueVector ret = cobj->CheckBulletHitFishId(arg0, arg1);
        ccvaluevector_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:CheckBulletHitFishId",argc, 2);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_CheckBulletHitFishId'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_AddCurvePath(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_AddCurvePath'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        int arg0;
        cocos2d::ValueVector arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:AddCurvePath");

        ok &= luaval_to_ccvaluevector(tolua_S, 3, &arg1, "FishCocosMapManager:AddCurvePath");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_AddCurvePath'", nullptr);
            return 0;
        }
        int ret = cobj->AddCurvePath(arg0, arg1);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:AddCurvePath",argc, 2);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_AddCurvePath'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_RemoveCocosBulletObject(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_RemoveCocosBulletObject'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:RemoveCocosBulletObject");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_RemoveCocosBulletObject'", nullptr);
            return 0;
        }
        cobj->RemoveCocosBulletObject(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:RemoveCocosBulletObject",argc, 1);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_RemoveCocosBulletObject'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_UpdateFish(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FishCocosMapManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_FishCocosMapManager_UpdateFish'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        int arg0;
        int arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "FishCocosMapManager:UpdateFish");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "FishCocosMapManager:UpdateFish");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_UpdateFish'", nullptr);
            return 0;
        }
        cobj->UpdateFish(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:UpdateFish",argc, 2);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_UpdateFish'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_FishCocosMapManager_destroyInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif

#if 1
    if (!tolua_isusertable(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_destroyInstance'", nullptr);
            return 0;
        }
        FishCocosMapManager::destroyInstance();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "FishCocosMapManager:destroyInstance",argc, 0);
    return 0;
#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_destroyInstance'.",&tolua_err);
#endif
    return 0;
}
int lua_ccext_logic_FishCocosMapManager_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif

#if 1
    if (!tolua_isusertable(tolua_S,1,"FishCocosMapManager",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_getInstance'", nullptr);
            return 0;
        }
        FishCocosMapManager* ret = FishCocosMapManager::getInstance();
        object_to_luaval<FishCocosMapManager>(tolua_S, "FishCocosMapManager",(FishCocosMapManager*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "FishCocosMapManager:getInstance",argc, 0);
    return 0;
#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_getInstance'.",&tolua_err);
#endif
    return 0;
}
int lua_ccext_logic_FishCocosMapManager_constructor(lua_State* tolua_S)
{
    int argc = 0;
    FishCocosMapManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_FishCocosMapManager_constructor'", nullptr);
            return 0;
        }
        cobj = new FishCocosMapManager();
        tolua_pushusertype(tolua_S,(void*)cobj,"FishCocosMapManager");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "FishCocosMapManager:FishCocosMapManager",argc, 0);
    return 0;

#if 1
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_FishCocosMapManager_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_ccext_logic_FishCocosMapManager_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (FishCocosMapManager)");
    return 0;
}

int lua_register_ccext_logic_FishCocosMapManager(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"FishCocosMapManager");
    tolua_cclass(tolua_S,"FishCocosMapManager","FishCocosMapManager","",nullptr);

    tolua_beginmodule(tolua_S,"FishCocosMapManager");
        tolua_function(tolua_S,"new",lua_ccext_logic_FishCocosMapManager_constructor);
        tolua_function(tolua_S,"AddCocosBulletObject",lua_ccext_logic_FishCocosMapManager_AddCocosBulletObject);
        tolua_function(tolua_S,"GetDebugFishRegions",lua_ccext_logic_FishCocosMapManager_GetDebugFishRegions);
        tolua_function(tolua_S,"RemoveCocosFishObject",lua_ccext_logic_FishCocosMapManager_RemoveCocosFishObject);
        tolua_function(tolua_S,"CheckBulletHit",lua_ccext_logic_FishCocosMapManager_CheckBulletHit);
        tolua_function(tolua_S,"TranslateFishGridInfo2MapInfo",lua_ccext_logic_FishCocosMapManager_TranslateFishGridInfo2MapInfo);
        tolua_function(tolua_S,"GetDebugHitLineInfo",lua_ccext_logic_FishCocosMapManager_GetDebugHitLineInfo);
        tolua_function(tolua_S,"GetFishIdsInGrid",lua_ccext_logic_FishCocosMapManager_GetFishIdsInGrid);
        tolua_function(tolua_S,"init",lua_ccext_logic_FishCocosMapManager_init);
        tolua_function(tolua_S,"RemoveCurvePath",lua_ccext_logic_FishCocosMapManager_RemoveCurvePath);
        tolua_function(tolua_S,"ReSetMapInfo",lua_ccext_logic_FishCocosMapManager_ReSetMapInfo);
        tolua_function(tolua_S,"IsFishInVisible",lua_ccext_logic_FishCocosMapManager_IsFishInVisible);
        tolua_function(tolua_S,"UpdateFishRegion",lua_ccext_logic_FishCocosMapManager_UpdateFishRegion);
        tolua_function(tolua_S,"SetMapSize",lua_ccext_logic_FishCocosMapManager_SetMapSize);
        tolua_function(tolua_S,"GetMaxScoreFishInGrid",lua_ccext_logic_FishCocosMapManager_GetMaxScoreFishInGrid);
        tolua_function(tolua_S,"SetMaxHitLinePointCount",lua_ccext_logic_FishCocosMapManager_SetMaxHitLinePointCount);
        tolua_function(tolua_S,"IsFishOnGrid",lua_ccext_logic_FishCocosMapManager_IsFishOnGrid);
        tolua_function(tolua_S,"AddCocosFishObject",lua_ccext_logic_FishCocosMapManager_AddCocosFishObject);
        tolua_function(tolua_S,"CheckBulletHitFishId",lua_ccext_logic_FishCocosMapManager_CheckBulletHitFishId);
        tolua_function(tolua_S,"AddCurvePath",lua_ccext_logic_FishCocosMapManager_AddCurvePath);
        tolua_function(tolua_S,"RemoveCocosBulletObject",lua_ccext_logic_FishCocosMapManager_RemoveCocosBulletObject);
        tolua_function(tolua_S,"UpdateFish",lua_ccext_logic_FishCocosMapManager_UpdateFish);
        tolua_function(tolua_S,"destroyInstance", lua_ccext_logic_FishCocosMapManager_destroyInstance);
        tolua_function(tolua_S,"getInstance", lua_ccext_logic_FishCocosMapManager_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(FishCocosMapManager).name();
    g_luaType[typeName] = "FishCocosMapManager";
    g_typeCast["FishCocosMapManager"] = "FishCocosMapManager";
    return 1;
}

int lua_ccext_logic_BBGameManager_destroyInstance(lua_State* tolua_S)
{
	int argc = 0;
	BBGameManager* cobj = nullptr;
	bool ok = true;

#if 1
	tolua_Error tolua_err;
#endif


#if 1
	if (!tolua_isusertype(tolua_S, 1, "BBGameManager", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (BBGameManager*)tolua_tousertype(tolua_S, 1, 0);

#if 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_ccext_logic_BBGameManager_destroyInstance'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0)
	{
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_ccext_logic_BBGameManager_destroyInstance'", nullptr);
			return 0;
		}
		cobj->destroyInstance();
		lua_settop(tolua_S, 1);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "BBGameManager:destroyInstance", argc, 0);
	return 0;

#if 1
	tolua_lerror:
		tolua_error(tolua_S, "#ferror in function 'lua_ccext_logic_BBGameManager_destroyInstance'.", &tolua_err);
#endif

	return 0;
}
int lua_ccext_logic_BBGameManager_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif

#if 1
    if (!tolua_isusertable(tolua_S,1,"BBGameManager",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_BBGameManager_getInstance'", nullptr);
            return 0;
        }
        BBGameManager* ret = BBGameManager::getInstance();
        object_to_luaval<BBGameManager>(tolua_S, "BBGameManager",(BBGameManager*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "BBGameManager:getInstance",argc, 0);
    return 0;
#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_BBGameManager_getInstance'.",&tolua_err);
#endif
    return 0;
}
int lua_ccext_logic_BBGameManager_InitRoom(lua_State* tolua_S)
{
    int argc = 0;
    BBGameManager* cobj = nullptr;
    bool ok = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S, 1, "BBGameManager", 0, &tolua_err)) goto tolua_lerror;
#endif

    cobj = (BBGameManager*)tolua_tousertype(tolua_S, 1, 0);

#if 1
    if (!cobj)
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_ccext_logic_BBGameManager_InitRoom'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0)
    {
        if (!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_ccext_logic_BBGameManager_InitRoom'", nullptr);
            return 0;
        }
        cobj->InitRoom();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "BBGameManager:InitRoom", argc, 0);
    return 0;

#if 1
    tolua_lerror:
        tolua_error(tolua_S, "#ferror in function 'lua_ccext_logic_BBGameManager_InitRoom'.", &tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_BBGameManager_InitFood(lua_State* tolua_S)
{
    int argc = 0;
    BBGameManager* cobj = nullptr;
    bool ok = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S, 1, "BBGameManager", 0, &tolua_err)) goto tolua_lerror;
#endif

    cobj = (BBGameManager*)tolua_tousertype(tolua_S, 1, 0);

#if 1
    if (!cobj)
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_ccext_logic_BBGameManager_InitFood'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0)
    {
        if (!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_ccext_logic_BBGameManager_InitFood'", nullptr);
            return 0;
        }
        cobj->InitFood();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "BBGameManager:InitFood", argc, 0);
    return 0;

#if 1
    tolua_lerror:
        tolua_error(tolua_S, "#ferror in function 'lua_ccext_logic_BBGameManager_InitFood'.", &tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_BBGameManager_CreatePlayer(lua_State* tolua_S)
{
    int argc = 0;
    BBGameManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"BBGameManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BBGameManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_BBGameManager_AddPlayer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "BBGameManager:CreatePlayer");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_BBGameManager_AddPlayer'", nullptr);
            return 0;
        }
        cobj->CreatePlayer(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "BBGameManager:CreatePlayer",argc, 2);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_BBGameManager_AddPlayer'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_BBGameManager_RemovePlayer(lua_State* tolua_S)
{
	int argc = 0;
	BBGameManager* cobj = nullptr;
	bool ok = true;

#if 1
	tolua_Error tolua_err;
#endif


#if 1
	if (!tolua_isusertype(tolua_S, 1, "BBGameManager", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (BBGameManager*)tolua_tousertype(tolua_S, 1, 0);

#if 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_ccext_logic_BBGameManager_RemovePlayer'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1)
	{
		int arg0;

		ok &= luaval_to_int32(tolua_S, 2, (int *)&arg0, "BBGameManager:RemovePlayer");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_ccext_logic_BBGameManager_RemovePlayer'", nullptr);
			return 0;
		}
		cobj->RemovePlayer(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "BBGameManager:RemovePlayer", argc, 2);
	return 0;

#if 1
	tolua_lerror:
		tolua_error(tolua_S, "#ferror in function 'lua_ccext_logic_BBGameManager_RemovePlayer'.", &tolua_err);
#endif

	return 0;
}
int lua_ccext_logic_BBGameManager_GetAllFoodInfos(lua_State* tolua_S)
{
    int argc = 0;
    BBGameManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"BBGameManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BBGameManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_BBGameManager_GetAllFoodInfos'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
		std::vector<int> ret = cobj->GetAllFoodInfos();
		ccvector_int_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "BBGameManager:GetAllFoodInfos",argc, 2);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_BBGameManager_GetAllFoodInfos'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_BBGameManager_CheckPlayerHit(lua_State* tolua_S)
{
    int argc = 0;
    BBGameManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"BBGameManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BBGameManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_BBGameManager_CheckPlayerHit'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "BBGameManager:CheckPlayerHit");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_BBGameManager_CheckPlayerHit'", nullptr);
            return 0;
        }
        std::vector<int> ret =  cobj->CheckPlayerHit(arg0);
		ccvector_int_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "BBGameManager:CheckPlayerHit",argc, 2);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_BBGameManager_CheckPlayerHit'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_BBGameManager_GetPlayerNodeIdx(lua_State* tolua_S)
{
    int argc = 0;
    BBGameManager* cobj = nullptr;
    bool ok  = true;

#if 1
    tolua_Error tolua_err;
#endif


#if 1
    if (!tolua_isusertype(tolua_S,1,"BBGameManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BBGameManager*)tolua_tousertype(tolua_S,1,0);

#if 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ccext_logic_BBGameManager_GetPlayerNodeIdx'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "BBGameManager:GetPlayerNodeIdx");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ccext_logic_BBGameManager_GetPlayerNodeIdx'", nullptr);
            return 0;
        }
        std::vector<int> ret =  cobj->GetPlayerNodeIdx(arg0);
		ccvector_int_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "BBGameManager:GetPlayerNodeIdx",argc, 2);
    return 0;

#if 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ccext_logic_BBGameManager_GetPlayerNodeIdx'.",&tolua_err);
#endif

    return 0;
}
int lua_ccext_logic_BBGameManager_ChangePlayerNodePos(lua_State* tolua_S)
{
	int argc = 0;
	BBGameManager* cobj = nullptr;
	bool ok = true;

#if 1
	tolua_Error tolua_err;
#endif


#if 1
	if (!tolua_isusertype(tolua_S, 1, "BBGameManager", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (BBGameManager*)tolua_tousertype(tolua_S, 1, 0);

#if 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_ccext_logic_BBGameManager_ChangePlayerNodePos'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 3)
	{
		int arg0, arg1, arg2;

		ok &= luaval_to_int32(tolua_S, 2, (int *)&arg0, "BBGameManager:ChangePlayerNodePos");
		ok &= luaval_to_int32(tolua_S, 3, (int *)&arg1, "BBGameManager:ChangePlayerNodePos");
		ok &= luaval_to_int32(tolua_S, 4, (int *)&arg2, "BBGameManager:ChangePlayerNodePos");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_ccext_logic_BBGameManager_ChangePlayerNodePos'", nullptr);
			return 0;
		}
		cobj->ChangePlayerNodePos(arg0, arg1, arg2);
		lua_settop(tolua_S, 1);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "BBGameManager:ChangePlayerNodePos", argc, 2);
	return 0;

#if 1
	tolua_lerror:
		tolua_error(tolua_S, "#ferror in function 'lua_ccext_logic_BBGameManager_ChangePlayerNodePos'.", &tolua_err);
#endif

	return 0;
}


int lua_ccext_logic_BBGameManager_GetFoodIdxByPos(lua_State* tolua_S)
{
	int argc = 0;
	BBGameManager* cobj = nullptr;
	bool ok = true;

#if 1
	tolua_Error tolua_err;
#endif


#if 1
	if (!tolua_isusertype(tolua_S, 1, "BBGameManager", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (BBGameManager*)tolua_tousertype(tolua_S, 1, 0);

#if 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_ccext_logic_BBGameManager_GetFoodIdxByPos'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1)
	{
		int arg0, arg1, arg2;

		ok &= luaval_to_int32(tolua_S, 2, (int *)&arg0, "BBGameManager:GetFoodIdxByPos");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_ccext_logic_BBGameManager_GetFoodIdxByPos'", nullptr);
			return 0;
		}
		int ret = cobj->GetFoodIdxByPos(arg0);
		tolua_pushnumber(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "BBGameManager:GetFoodIdxByPos", argc, 2);
	return 0;

#if 1
	tolua_lerror:
		tolua_error(tolua_S, "#ferror in function 'lua_ccext_logic_BBGameManager_GetFoodIdxByPos'.", &tolua_err);
#endif

	return 0;
}

int lua_ccext_logic_BBGameManager_GetFoodPosByIdx(lua_State* tolua_S)
{
	int argc = 0;
	BBGameManager* cobj = nullptr;
	bool ok = true;

#if 1
	tolua_Error tolua_err;
#endif


#if 1
	if (!tolua_isusertype(tolua_S, 1, "BBGameManager", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (BBGameManager*)tolua_tousertype(tolua_S, 1, 0);

#if 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_ccext_logic_BBGameManager_GetFoodPosByIdx'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1)
	{
		int arg0, arg1, arg2;

		ok &= luaval_to_int32(tolua_S, 2, (int *)&arg0, "BBGameManager:GetFoodPosByIdx");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_ccext_logic_BBGameManager_GetFoodPosByIdx'", nullptr);
			return 0;
		}
		int ret = cobj->GetFoodPosByIdx(arg0);
		tolua_pushnumber(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "BBGameManager:GetFoodPosByIdx", argc, 2);
	return 0;

#if 1
	tolua_lerror:
		tolua_error(tolua_S, "#ferror in function 'lua_ccext_logic_BBGameManager_GetFoodPosByIdx'.", &tolua_err);
#endif

	return 0;
}
int lua_ccext_logic_BBGameManager_OnUpdate(lua_State* tolua_S)
{
	int argc = 0;
	BBGameManager* cobj = nullptr;
	bool ok = true;

#if 1
	tolua_Error tolua_err;
#endif


#if 1
	if (!tolua_isusertype(tolua_S, 1, "BBGameManager", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (BBGameManager*)tolua_tousertype(tolua_S, 1, 0);

#if 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_ccext_logic_BBGameManager_OnUpdate'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0)
	{
		cobj->OnUpdate();
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "BBGameManager:OnUpdate", argc, 2);
	return 0;

#if 1
	tolua_lerror:
		tolua_error(tolua_S, "#ferror in function 'lua_ccext_logic_BBGameManager_OnUpdate'.", &tolua_err);
#endif

	return 0;
}

int lua_register_ccext_logic_BBGameManager(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"BBGameManager");
    tolua_cclass(tolua_S,"BBGameManager","BBGameManager","",nullptr);

    tolua_beginmodule(tolua_S,"BBGameManager");
        tolua_function(tolua_S,"destroyInstance", lua_ccext_logic_BBGameManager_destroyInstance);
        tolua_function(tolua_S,"getInstance", lua_ccext_logic_BBGameManager_getInstance);
        tolua_function(tolua_S,"InitRoom", lua_ccext_logic_BBGameManager_InitRoom);
        tolua_function(tolua_S,"InitFood", lua_ccext_logic_BBGameManager_InitFood);
        tolua_function(tolua_S,"CreatePlayer", lua_ccext_logic_BBGameManager_CreatePlayer);
		tolua_function(tolua_S, "RemovePlayer", lua_ccext_logic_BBGameManager_RemovePlayer);
		
        tolua_function(tolua_S,"GetAllFoodInfos", lua_ccext_logic_BBGameManager_GetAllFoodInfos);
        tolua_function(tolua_S,"CheckPlayerHit", lua_ccext_logic_BBGameManager_CheckPlayerHit);
        tolua_function(tolua_S,"GetPlayerNodeIdx", lua_ccext_logic_BBGameManager_GetPlayerNodeIdx);
		tolua_function(tolua_S, "ChangePlayerNodePos", lua_ccext_logic_BBGameManager_ChangePlayerNodePos);
		tolua_function(tolua_S, "GetFoodIdxByPos", lua_ccext_logic_BBGameManager_GetFoodIdxByPos);
		tolua_function(tolua_S, "GetFoodPosByIdx", lua_ccext_logic_BBGameManager_GetFoodPosByIdx);
		tolua_function(tolua_S, "OnUpdate", lua_ccext_logic_BBGameManager_OnUpdate);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(BBGameManager).name();
    g_luaType[typeName] = "BBGameManager";
    g_typeCast["BBGameManager"] = "BBGameManager";
    return 1;
}
TOLUA_API int register_all_ccext_logic(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"cc",0);
	tolua_beginmodule(tolua_S,"cc");

	lua_register_ccext_logic_FishCocosMapManager(tolua_S);
	lua_register_ccext_logic_BBGameManager(tolua_S);
	
	tolua_endmodule(tolua_S);
	return 1;
}

