# Lua
## Properties

### private:
##### lua_State \* L
the current state of the lua virtual machine.

##### [[Pcall(int args, int rets)]] :
calls a Lua function and throws an [[Exception]] if an error occours.

### public:

##### std::map<std::string, int (\*)(lua_State \* L) functs
pointers to all c functions that have been added to this Lua virtual machine.

##### [[TableValue]] :
A struct that can represent a lua variable.

##### [[Table]] :
A type definition of  a map using string [[TableValue]] pairs.

##### [[LuaTableBuilder]] :
a class that can be used to build lua tables that can be sent to the lua virtual machine.

##### [[static Table lua_gettable(lua_State pointer L)|static Table lua_gettable(lua_State * L)]] :
returns the table stored on the first position of the lua stack.

##### [[ClassTable]] :
a parent class that provides virtual functions to convert a classes functions and properties to and from a [[Table]].

##### [[PrintTable(lua_State pointer L)|PrintTable(lua_State \* L)]] :
Prints the table stored on the first position of the lua stack to the console.

##### [[lua_pushtable(lua_State pointer LUA, Table val)|lua_pushtable(lua_State * LUA, Table val)]] :
pushes a [[Table]] onto the lua stack. 

##### [[PushClass(string Name, LuaTableBuilder lc)|PushClass(std::string Name, LuaTableBuilder lc)]] :
creates a global lua[[Table]] using a [[LuaTableBuilder]]. used to add class representations to lua.

##### [[GetState()]] :
returns the lua_State for this instance.

##### [[Lua()]] :
returns a new [[VoxelFox/VoxelFox-Library/VoxelFox-C++ API/Classes/Lua/Lua]] object.

##### [[SetGlobal(string Name, (insert typename) val)]] :
sets a global value in the lua virtal machine.

##### [[AddSubTable(Lua_State pointer LUA, string Key, Table val)|AddSubTable(Lua_State * LUA, string Key, Table val)]] :
pushes a [[Table]] to lua and assigns it a key.

##### [[RegisterFunction(string Name, int (pointer funct)(lua_State pointer L))|RegisterFunction(string Name, int (* funct)(lua_State * L))]] :
registers a c function to lua.

##### [[GetGlobalNumber(string Name)]] :
returns the double from lua that is referenced by the variable name.

##### [[GetGlobalString(string Name)]] :
returns the string from lua that is referenced by the variable name.

##### [[GetGlobalBool(string Name)]] :
returns the boolean from lua that is referenced by the variable name.

##### [[GetGlobalTable(string Name)]] :
returns the [[Table]] from lua that is referenced by the variable name.

##### [[CallLuaFunct(string Name,LuaCFunct ArgumentSetup=nullptr,int args=0,LuaCFunct ReturnedDataHandler=nullptr,int rets=0)|CallLuaFunct(std::string Name,void(*ArgumentSetup)(lua_State* L)=nullptr,int args=0,void(*ReturnedDataHandler)(lua_State* L)=nullptr,int rets=0)]] :
calls a lua function referenced by Name must pass a function to setup variables and tho gather the returns of the lua function, along with the amount of inputs and outputs.

##### [[LoadScript(string FilePath)]] :
loads a lua script file without running the code.

##### [[RunScript(string FilePath)]] :
loads a lua script file and runs any code.

##### [[LoadString(string str)]] :
loads a string without running the code.

##### [[RunString(string FilePath)]] :
loads a string and runs any code.

##### [[~Lua()]] :
closes the lua vm.