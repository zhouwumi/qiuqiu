#if USE_UNI_LUA
using LuaAPI = UniLua.Lua;
using RealStatePtr = UniLua.ILuaState;
using LuaCSFunction = UniLua.CSharpFunctionDelegate;
#else
using LuaAPI = XLua.LuaDLL.Lua;
using RealStatePtr = System.IntPtr;
using LuaCSFunction = XLua.LuaDLL.lua_CSFunction;
#endif

using XLua;
using System.Collections.Generic;


namespace XLua.CSObjectWrap
{
    using Utils = XLua.Utils;
    public class GameConfigWrap 
    {
        public static void __Register(RealStatePtr L)
        {
			ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			System.Type type = typeof(GameConfig);
			Utils.BeginObjectRegister(type, L, translator, 0, 4, 13, 13);
			
			Utils.RegisterFunc(L, Utils.METHOD_IDX, "FindEntry", _m_FindEntry);
			Utils.RegisterFunc(L, Utils.METHOD_IDX, "FindPlayer", _m_FindPlayer);
			Utils.RegisterFunc(L, Utils.METHOD_IDX, "Init", _m_Init);
			Utils.RegisterFunc(L, Utils.METHOD_IDX, "GetConfigs", _m_GetConfigs);
			
			
			Utils.RegisterFunc(L, Utils.GETTER_IDX, "Language", _g_get_Language);
            Utils.RegisterFunc(L, Utils.GETTER_IDX, "battleConstParameter", _g_get_battleConstParameter);
            Utils.RegisterFunc(L, Utils.GETTER_IDX, "heroParameter", _g_get_heroParameter);
            Utils.RegisterFunc(L, Utils.GETTER_IDX, "heroScoreParameter", _g_get_heroScoreParameter);
            Utils.RegisterFunc(L, Utils.GETTER_IDX, "miscellaneousParameter", _g_get_miscellaneousParameter);
            Utils.RegisterFunc(L, Utils.GETTER_IDX, "dialogParameter", _g_get_dialogParameter);
            Utils.RegisterFunc(L, Utils.GETTER_IDX, "heroTrailParameter", _g_get_heroTrailParameter);
            Utils.RegisterFunc(L, Utils.GETTER_IDX, "layerTerrain", _g_get_layerTerrain);
            Utils.RegisterFunc(L, Utils.GETTER_IDX, "layerFlyingItem", _g_get_layerFlyingItem);
            Utils.RegisterFunc(L, Utils.GETTER_IDX, "layerHeroBody", _g_get_layerHeroBody);
            Utils.RegisterFunc(L, Utils.GETTER_IDX, "layerUI", _g_get_layerUI);
            Utils.RegisterFunc(L, Utils.GETTER_IDX, "layerBattleObject", _g_get_layerBattleObject);
            Utils.RegisterFunc(L, Utils.GETTER_IDX, "layerBoss", _g_get_layerBoss);
            
			Utils.RegisterFunc(L, Utils.SETTER_IDX, "Language", _s_set_Language);
            Utils.RegisterFunc(L, Utils.SETTER_IDX, "battleConstParameter", _s_set_battleConstParameter);
            Utils.RegisterFunc(L, Utils.SETTER_IDX, "heroParameter", _s_set_heroParameter);
            Utils.RegisterFunc(L, Utils.SETTER_IDX, "heroScoreParameter", _s_set_heroScoreParameter);
            Utils.RegisterFunc(L, Utils.SETTER_IDX, "miscellaneousParameter", _s_set_miscellaneousParameter);
            Utils.RegisterFunc(L, Utils.SETTER_IDX, "dialogParameter", _s_set_dialogParameter);
            Utils.RegisterFunc(L, Utils.SETTER_IDX, "heroTrailParameter", _s_set_heroTrailParameter);
            Utils.RegisterFunc(L, Utils.SETTER_IDX, "layerTerrain", _s_set_layerTerrain);
            Utils.RegisterFunc(L, Utils.SETTER_IDX, "layerFlyingItem", _s_set_layerFlyingItem);
            Utils.RegisterFunc(L, Utils.SETTER_IDX, "layerHeroBody", _s_set_layerHeroBody);
            Utils.RegisterFunc(L, Utils.SETTER_IDX, "layerUI", _s_set_layerUI);
            Utils.RegisterFunc(L, Utils.SETTER_IDX, "layerBattleObject", _s_set_layerBattleObject);
            Utils.RegisterFunc(L, Utils.SETTER_IDX, "layerBoss", _s_set_layerBoss);
            
			
			Utils.EndObjectRegister(type, L, translator, null, null,
			    null, null, null);

		    Utils.BeginClassRegister(type, L, __CreateInstance, 1, 0, 0);
			
			
            
			
			
			
			Utils.EndClassRegister(type, L, translator);
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int __CreateInstance(RealStatePtr L)
        {
            
			try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
				if(LuaAPI.lua_gettop(L) == 1)
				{
					
					GameConfig gen_ret = new GameConfig();
					translator.Push(L, gen_ret);
                    
					return 1;
				}
				
			}
			catch(System.Exception gen_e) {
				return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
			}
            return LuaAPI.luaL_error(L, "invalid arguments to GameConfig constructor!");
            
        }
        
		
        
		
        
        
        
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_FindEntry(RealStatePtr L)
        {
		    try {
            
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
            
            
                
                {
                    string _configName = LuaAPI.lua_tostring(L, 2);
                    string _columnName = LuaAPI.lua_tostring(L, 3);
                    string _value = LuaAPI.lua_tostring(L, 4);
                    
                        SimpleJSON.JSONNode gen_ret = gen_to_be_invoked.FindEntry( _configName, _columnName, _value );
                        translator.Push(L, gen_ret);
                    
                    
                    
                    return 1;
                }
                
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_FindPlayer(RealStatePtr L)
        {
		    try {
            
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
            
            
                
                {
                    string _playerId = LuaAPI.lua_tostring(L, 2);
                    int _lv = LuaAPI.xlua_tointeger(L, 3);
                    
                        SimpleJSON.JSONNode gen_ret = gen_to_be_invoked.FindPlayer( _playerId, _lv );
                        translator.Push(L, gen_ret);
                    
                    
                    
                    return 1;
                }
                
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_Init(RealStatePtr L)
        {
		    try {
            
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
            
            
                
                {
                    
                    gen_to_be_invoked.Init(  );
                    
                    
                    
                    return 0;
                }
                
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_GetConfigs(RealStatePtr L)
        {
		    try {
            
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
            
            
                
                {
                    string[] _names;
                    string[] _jsons;
                    
                    gen_to_be_invoked.GetConfigs( out _names, out _jsons );
                    translator.Push(L, _names);
                        
                    translator.Push(L, _jsons);
                        
                    
                    
                    
                    return 2;
                }
                
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            
        }
        
        
        
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _g_get_Language(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                translator.PushLanguage(L, gen_to_be_invoked.Language);
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 1;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _g_get_battleConstParameter(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                translator.Push(L, gen_to_be_invoked.battleConstParameter);
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 1;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _g_get_heroParameter(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                translator.Push(L, gen_to_be_invoked.heroParameter);
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 1;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _g_get_heroScoreParameter(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                translator.Push(L, gen_to_be_invoked.heroScoreParameter);
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 1;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _g_get_miscellaneousParameter(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                translator.Push(L, gen_to_be_invoked.miscellaneousParameter);
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 1;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _g_get_dialogParameter(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                translator.Push(L, gen_to_be_invoked.dialogParameter);
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 1;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _g_get_heroTrailParameter(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                translator.Push(L, gen_to_be_invoked.heroTrailParameter);
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 1;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _g_get_layerTerrain(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                LuaAPI.xlua_pushinteger(L, gen_to_be_invoked.layerTerrain);
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 1;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _g_get_layerFlyingItem(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                LuaAPI.xlua_pushinteger(L, gen_to_be_invoked.layerFlyingItem);
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 1;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _g_get_layerHeroBody(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                LuaAPI.xlua_pushinteger(L, gen_to_be_invoked.layerHeroBody);
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 1;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _g_get_layerUI(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                LuaAPI.xlua_pushinteger(L, gen_to_be_invoked.layerUI);
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 1;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _g_get_layerBattleObject(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                LuaAPI.xlua_pushinteger(L, gen_to_be_invoked.layerBattleObject);
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 1;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _g_get_layerBoss(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                LuaAPI.xlua_pushinteger(L, gen_to_be_invoked.layerBoss);
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 1;
        }
        
        
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _s_set_Language(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                Language gen_value;translator.Get(L, 2, out gen_value);
				gen_to_be_invoked.Language = gen_value;
            
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 0;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _s_set_battleConstParameter(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                gen_to_be_invoked.battleConstParameter = (BattleConstParameter)translator.GetObject(L, 2, typeof(BattleConstParameter));
            
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 0;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _s_set_heroParameter(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                gen_to_be_invoked.heroParameter = (HeroParameter)translator.GetObject(L, 2, typeof(HeroParameter));
            
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 0;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _s_set_heroScoreParameter(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                gen_to_be_invoked.heroScoreParameter = (HeroScoreParameter)translator.GetObject(L, 2, typeof(HeroScoreParameter));
            
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 0;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _s_set_miscellaneousParameter(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                gen_to_be_invoked.miscellaneousParameter = (MiscellaneousParameter)translator.GetObject(L, 2, typeof(MiscellaneousParameter));
            
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 0;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _s_set_dialogParameter(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                gen_to_be_invoked.dialogParameter = (DialogParameter)translator.GetObject(L, 2, typeof(DialogParameter));
            
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 0;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _s_set_heroTrailParameter(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                gen_to_be_invoked.heroTrailParameter = (HeroTrailParameter)translator.GetObject(L, 2, typeof(HeroTrailParameter));
            
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 0;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _s_set_layerTerrain(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                gen_to_be_invoked.layerTerrain = LuaAPI.xlua_tointeger(L, 2);
            
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 0;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _s_set_layerFlyingItem(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                gen_to_be_invoked.layerFlyingItem = LuaAPI.xlua_tointeger(L, 2);
            
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 0;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _s_set_layerHeroBody(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                gen_to_be_invoked.layerHeroBody = LuaAPI.xlua_tointeger(L, 2);
            
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 0;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _s_set_layerUI(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                gen_to_be_invoked.layerUI = LuaAPI.xlua_tointeger(L, 2);
            
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 0;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _s_set_layerBattleObject(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                gen_to_be_invoked.layerBattleObject = LuaAPI.xlua_tointeger(L, 2);
            
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 0;
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _s_set_layerBoss(RealStatePtr L)
        {
		    try {
                ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			
                GameConfig gen_to_be_invoked = (GameConfig)translator.FastGetCSObj(L, 1);
                gen_to_be_invoked.layerBoss = LuaAPI.xlua_tointeger(L, 2);
            
            } catch(System.Exception gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + gen_e);
            }
            return 0;
        }
        
		
		
		
		
    }
}
