## GetGlobalTable(string Name)
#### Description:
gets the global [[Table]] assosiated with Name from lua;
#### Example :
void main() {
&nbsp &nbsp &nbsp &nbsp [[VoxelFox/VoxelFox-Library/VoxelFox-C++ API/Classes/Lua/Lua]] l;
&nbsp &nbsp &nbsp &nbsp [[VoxelFox/VoxelFox-Library/VoxelFox-C++ API/Classes/Lua/Lua]]::[[LuaTableBuilder]] tbl;
&nbsp &nbsp &nbsp &nbsp tbl.AddString("1","Hello World!");
&nbsp &nbsp &nbsp &nbsp tbl.AddNumber("2",25);
&nbsp &nbsp &nbsp &nbsp l.[[SetGlobal(string Name, (insert typename) val)|SetGlobal("value",tbl)]];
&nbsp &nbsp &nbsp &nbsp [[VoxelFox/VoxelFox-Library/VoxelFox-C++ API/Classes/Lua/Lua]]::[[PrintTable(lua_State pointer L)|PrintTable(l.GetState())]]
}

#### Output:
\> 1 = Hello World!
\> 2 = 25