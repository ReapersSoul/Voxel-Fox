## CallLuaFunct(string Name, [[LuaCFunct]] ArgumentSetup=nullptr, int args=0, [[LuaCFunct]] ReturnedDataHandler=nullptr,int rets=0)
#### Example :
void main() {
&nbsp &nbsp &nbsp &nbsp Lua l;
&nbsp &nbsp &nbsp &nbsp l.callLuaFunct(
&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp "print",
&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp [](lua_State* L){
&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp lua_pushstring(L,"Hello World!");
&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp }, 1);
}

#### Output:
\> Hello World!