#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>


extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

class Exception : public std::exception
{
	std::string exstring;
public:
	Exception(std::string exString) {
		exstring = exString;
	}

	virtual const char* what() const throw()
	{
		return exstring.c_str();
	}
};

class Lua {
	lua_State* L;
	bool isAvailable = true;
	void Pcall(int args, int rets){
		if (lua_pcall(L, args, rets, 0) != 0) {
			std::string s = lua_tostring(L, -1);
			if (s.empty()) {
				throw Exception("Unknown Lua Error");
			}
			else {
				throw Exception(s);
			}
		}
	}

public:
	void WaitForAvailability() {
		while (!isAvailable) {

		}
	}
	void WaitForAvailabilityAndSetFalse() {
		WaitForAvailability();
		isAvailable = false;
	}
	bool IsAvalible() {
		return isAvailable;
	}
	void SetAvailability(bool b) {
		isAvailable=b;
	}

	std::map<std::string, int (*)(lua_State* L)> functs;
	struct TableValue {
		enum valType{E_boolean,E_number,E_string,E_function,E_table};
		valType type = E_boolean;
		bool boolean=NULL;
		double number=NULL;
		std::string string="";
		int (*function)(lua_State* L) = nullptr;
		std::map<std::string, TableValue> table;
	};
	typedef std::map<std::string,TableValue> Table;

	class LuaTableBuilder {
		std::map<std::string, int (*)(lua_State* L)> Functions;
		std::map<std::string,std::string> Strings;
		std::map<std::string,bool> Bools;
		std::map<std::string,double> Numbers;
		std::map<std::string,Table> Tables;
	public:
		void AddFunction(std::string Name, int (*Function)(lua_State* L)) {
			Functions.insert(std::pair<std::string, int (*)(lua_State* L)>(Name, Function));
		}
		void AddString(std::string Name, std::string s) {
			Strings.insert(std::pair<std::string, std::string>(Name, s));
		}
		void AddBool(std::string Name, bool b) {
			Bools.insert(std::pair<std::string, bool>(Name, b));
		}
		void AddNumber(std::string Name, double d) {
			Numbers.insert(std::pair<std::string, double>(Name, d));
		}
		void AddTable(std::string Name, Table t) {
			Tables.insert(std::pair<std::string, Table>(Name, t));
		}

		Table GetTable() {
			Table ret;
			for (std::map<std::string, int (*)(lua_State* L)>::iterator it = Functions.begin(); it != Functions.end(); it++)
			{
				TableValue tmp;
				tmp.type = TableValue::E_function;
				tmp.function = it->second;
				ret.insert(std::pair<std::string, TableValue>(it->first, tmp));
			}
			for (std::map<std::string, std::string>::iterator it = Strings.begin(); it != Strings.end(); it++)
			{
				TableValue tmp;
				tmp.type = TableValue::E_string;
				tmp.string = it->second;
				ret.insert(std::pair<std::string, TableValue>(it->first, tmp));
			}
			for (std::map<std::string, bool>::iterator it = Bools.begin(); it != Bools.end(); it++)
			{
				TableValue tmp;
				tmp.type = TableValue::E_boolean;
				tmp.boolean = it->second;
				ret.insert(std::pair<std::string, TableValue>(it->first, tmp));
			}
			for (std::map<std::string, double>::iterator it = Numbers.begin(); it != Numbers.end(); it++)
			{
				TableValue tmp;
				tmp.type = TableValue::E_number;
				tmp.number = it->second;
				ret.insert(std::pair<std::string, TableValue>(it->first, tmp));
			}
			for (std::map<std::string, Table>::iterator it = Tables.begin(); it != Tables.end(); it++)
			{
				TableValue tmp;
				tmp.type = TableValue::E_table;
				tmp.table = it->second;
				ret.insert(std::pair<std::string, TableValue>(it->first, tmp));
			}
			return ret;
		}
	};

	static Table lua_gettable(lua_State* L,int index) {
		int top = lua_gettop(L);
		lua_settop(L,index);
		LuaTableBuilder ret;
		lua_pushnil(L);

		if (L == NULL) {
			throw Exception("Lua State is NULL!");
		}

		while (lua_next(L, -2) != 0)
		{
			if (lua_isboolean(L, -1)) {
				ret.AddBool(lua_tostring(L, -2), lua_toboolean(L, -1));
			}
			else if (lua_isnumber(L, -1)) {
				ret.AddNumber(lua_tostring(L, -2), lua_tonumber(L, -1));
			}
			else if (lua_isstring(L, -1)) {
			ret.AddString(lua_tostring(L, -2), lua_tostring(L, -1));
			}
			else if (lua_istable(L, -1)) {
				ret.AddTable(lua_tostring(L, -2), lua_gettable(L,1));
			}
			lua_pop(L, 1);
		}
		lua_settop(L, top);
		return ret.GetTable();
	}


	class ClassTable {
	public:
		virtual void FromTable(Table t) {};
		virtual Table ToTable() { return Table(); };
	};

	typedef  void(*LuaCFunct)(lua_State* L);

	void PrintTable(lua_State* L)
	{
		WaitForAvailabilityAndSetFalse();
		lua_pushnil(L);

		while (lua_next(L, -2) != 0)
		{
			if (lua_isstring(L, -1)) {
				printf("%s = %s\n", lua_tostring(L, -2), lua_tostring(L, -1));
			}
			else if (lua_isnumber(L, -1)) {
				printf("%s = %d\n", lua_tostring(L, -2), lua_tonumber(L, -1));
			}
			else if (lua_istable(L, -1)) {
				printf("SubTable:");
				SetAvailability(true);
				PrintTable(L);
			}
			lua_pop(L, 1);
		}
		SetAvailability(true);
	}


	static void lua_pushtable(lua_State * LUA, Table val) {
		lua_newtable(LUA);
		for (Table::iterator it = val.begin(); it != val.end(); it++)
		{
			switch (it->second.type)
			{
			case TableValue::E_boolean:
				lua_pushstring(LUA, it->first.c_str());
				lua_pushboolean(LUA, it->second.boolean);
				break;
			case TableValue::E_function:
				lua_pushstring(LUA, it->first.c_str());
				lua_pushcfunction(LUA, it->second.function);
				break;
			case TableValue::E_table:
				lua_pushstring(LUA, it->first.c_str());
				lua_pushtable(LUA,it->second.table);
				break;
			case TableValue::E_number:
				lua_pushstring(LUA, it->first.c_str());
				lua_pushnumber(LUA, it->second.number);
				break;
			case TableValue::E_string:
				lua_pushstring(LUA, it->first.c_str());
				lua_pushstring(LUA, it->second.string.c_str());
				break;
			default:
				break;
			}
			lua_settable(LUA, -3);
		}
	}

	void PushClass(std::string Name, LuaTableBuilder lc) {
		WaitForAvailability();
		SetGlobal(Name, lc.GetTable());
	}

	lua_State* GetState() {
		WaitForAvailability();
		return L;
	}

	Lua() {
		L = luaL_newstate();

		// load Lua libraries
		luaL_openlibs(L);
		static const luaL_Reg lualibs[] =
		{
			{ NULL, NULL}
		};

		const luaL_Reg* lib = lualibs;
		for (; lib->func != NULL; lib++)
		{
			lib->func(L);
			lua_settop(L, 0);
		}
		this->RegisterFunction("Load_Library", [](lua_State* L) {
			std::string filename = lua_tostring(L, 1);
			if (luaL_dofile(L, filename.c_str()) != 0) {
				std::string s = lua_tostring(L, -1);
				if (s.empty()) {
					throw Exception("Unknown Lua Error");
				}
				else {
					throw Exception(s);
				}
			}
			return 0;
			});
	}

	void SetGlobal(std::string Name, std::string val) {
		WaitForAvailabilityAndSetFalse();
		lua_pushstring(L, val.c_str());
		lua_setglobal(L, Name.c_str());
		SetAvailability(true);
	}
	void SetGlobal(std::string Name, int val) {
		WaitForAvailabilityAndSetFalse();
		lua_pushnumber(L, val);
		lua_setglobal(L, Name.c_str());
		SetAvailability(true);
	}
	void SetGlobal(std::string Name, double val) {
		WaitForAvailabilityAndSetFalse();
		lua_pushnumber(L, val);
		lua_setglobal(L, Name.c_str());
		SetAvailability(true);
	}
	void SetGlobal(std::string Name, bool val) {
		WaitForAvailabilityAndSetFalse();
		lua_pushboolean(L, val);
		lua_setglobal(L, Name.c_str());
		SetAvailability(true);
	}
	void SetGlobal(std::string Name, float val) {
		WaitForAvailabilityAndSetFalse();
		lua_pushnumber(L, val);
		lua_setglobal(L, Name.c_str());
		SetAvailability(true);
	}
	void SetGlobal(std::string Name, Table val) {
		WaitForAvailabilityAndSetFalse();
		lua_pushtable(L, val);
		lua_setglobal(L, Name.c_str());
		SetAvailability(true);
	}
	static void AddSubTable(lua_State * LUA, std::string Key, Table val) {
		lua_pushstring(LUA, Key.c_str());
		lua_newtable(LUA);
		for (Table::iterator it = val.begin(); it != val.end(); it++)
		{
			if (!it->second.table.empty()) {
				AddSubTable(LUA,it->first, it->second.table);
			}
			else if (it->second.function != nullptr) {
				lua_pushstring(LUA, it->first.c_str());
				lua_pushcfunction(LUA, it->second.function);
			}
			else if (it->second.string.empty()) {
				lua_pushstring(LUA, it->first.c_str());
				lua_pushstring(LUA, it->second.string.c_str());
			}
			else if (it->second.number != NULL) {
				lua_pushstring(LUA, it->first.c_str());
				lua_pushnumber(LUA, it->second.number);
			}
			else if (it->second.boolean != NULL) {
				lua_pushstring(LUA, it->first.c_str());
				lua_pushboolean(LUA, it->second.boolean);
			}
			lua_settable(LUA, -3);
		}
	}

	void RegisterFunction(std::string Name, int (*funct)(lua_State* L)) {
		WaitForAvailabilityAndSetFalse();
		functs.insert(std::pair<std::string, int (*)(lua_State* L)>(Name, funct));
		lua_register(L, Name.c_str(), funct);
		SetAvailability(true);
	}

	double GetGlobalNumber(std::string Name) {
		WaitForAvailabilityAndSetFalse();
		lua_getglobal(L, Name.c_str());
		int x = lua_tonumber(L, -1);
		lua_pop(L, 1);
		SetAvailability(true);
		return x;
	}

	std::string GetGlobalString(std::string Name) {
		WaitForAvailabilityAndSetFalse();
		lua_getglobal(L, Name.c_str());
		std::string x = lua_tostring(L, -1);
		lua_pop(L, 1);
		SetAvailability(true);
		return x;
	}
	bool GetGlobalBool(std::string Name) {
		WaitForAvailabilityAndSetFalse();
		lua_getglobal(L, Name.c_str());
		bool x = lua_toboolean(L, -1);
		lua_pop(L, 1);
		SetAvailability(true);
		return x;
	}
	Table GetGlobalTable(std::string Name) {
		WaitForAvailabilityAndSetFalse();
		lua_getglobal(L, Name.c_str());
		if (L == NULL || L == nullptr) {
			throw Exception("L is null");
		}
		Table ret = lua_gettable(L,1);
		lua_pop(L, 1);
		SetAvailability(true);
		return ret;
	}

	void CallLuaFunct(std::string Name,void(*ArgumentSetup)(lua_State* L)=nullptr,int args=0,void(*ReturnedDataHandler)(lua_State* L)=nullptr,int rets=0) {
		WaitForAvailabilityAndSetFalse();
		lua_getglobal(L, Name.c_str());
		if (lua_isfunction(L, -1))
		{
			// push function arguments into stack
			if (ArgumentSetup != nullptr) {
				ArgumentSetup(L);
			}
			Pcall(args, rets);
			if (ReturnedDataHandler != nullptr) {
				ReturnedDataHandler(L);
			}
		}
		SetAvailability(true);
	}

	void LoadScript(std::string Name) {
		WaitForAvailabilityAndSetFalse();
		if (luaL_loadfile(L, Name.c_str()) != 0) {
			std::string s = lua_tostring(L, -1);
			if (s.empty()) {
				throw Exception("Unknown Lua Error");
			}
			else {
				throw Exception(s);
			}
		}
		Pcall(0, 0);
		SetAvailability(true);
	}
	void RunScript(std::string Name) {
		WaitForAvailabilityAndSetFalse();
		if (luaL_dofile(L, Name.c_str()) != 0) {
			std::string s = lua_tostring(L, -1);
			if (s.empty()) {
				throw Exception("Unknown Lua Error");
			}
			else {
				throw Exception(s);
			}
		}
		SetAvailability(true);
	}
	void LoadString(std::string code) {
		WaitForAvailabilityAndSetFalse();
		if (luaL_loadstring(L, code.c_str()) != 0) {
			std::string s = lua_tostring(L, -1);
			if (s.empty()) {
				throw Exception("Unknown Lua Error");
			}
			else {
				throw Exception(s);
			}
		}
		Pcall(0, 0);
		SetAvailability(true);
	}
	void RunString(std::string code) {
		WaitForAvailabilityAndSetFalse();
		if (luaL_dostring(L, code.c_str()) != 0) {
			std::string s = lua_tostring(L, -1);
			if (s.empty()) {
				throw Exception("Unknown Lua Error");
			}
			else {
				throw Exception(s);
			}
		}
		SetAvailability(true);
	}

	~Lua() {
		lua_close(L);
	}
};