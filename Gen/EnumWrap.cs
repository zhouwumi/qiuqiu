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
    
    public class LanguageWrap
    {
		public static void __Register(RealStatePtr L)
        {
		    ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
		    Utils.BeginObjectRegister(typeof(Language), L, translator, 0, 0, 0, 0);
			Utils.EndObjectRegister(typeof(Language), L, translator, null, null, null, null, null);
			
            //从这里开始CS.Language表的生成
			Utils.BeginClassRegister(typeof(Language), L, null, 3, 0, 0);

            
            Utils.RegisterObject(L, translator, Utils.CLS_IDX, "Chinese", Language.Chinese);
            
            Utils.RegisterObject(L, translator, Utils.CLS_IDX, "Arabic", Language.Arabic);
            

			Utils.RegisterFunc(L, Utils.CLS_IDX, "__CastFrom", __CastFrom);
            
            Utils.EndClassRegister(typeof(Language), L, translator);
        }
		
		[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int __CastFrom(RealStatePtr L)
		{
			ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			LuaTypes lua_type = LuaAPI.lua_type(L, 1);
            if (lua_type == LuaTypes.LUA_TNUMBER)
            {
                translator.PushLanguage(L, (Language)LuaAPI.xlua_tointeger(L, 1));
            }
			
            else if(lua_type == LuaTypes.LUA_TSTRING)
            {

			    if (LuaAPI.xlua_is_eq_str(L, 1, "Chinese"))
                {
                    translator.PushLanguage(L, Language.Chinese);
                }
				else if (LuaAPI.xlua_is_eq_str(L, 1, "Arabic"))
                {
                    translator.PushLanguage(L, Language.Arabic);
                }
				else
                {
                    return LuaAPI.luaL_error(L, "invalid string for Language!");
                }

            }
			
            else
            {
                return LuaAPI.luaL_error(L, "invalid lua type for Language! Expect number or string, got + " + lua_type);
            }

            return 1;
		}
	}
    
}