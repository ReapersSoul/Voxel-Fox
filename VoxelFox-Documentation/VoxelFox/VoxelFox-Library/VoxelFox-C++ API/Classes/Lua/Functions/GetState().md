## GetState()
#### Description:
Returns a pointer to lua_State object assosiated with the current class object.
#### Example :
void main() {
&nbsp &nbsp &nbsp &nbsp [[Lua]] l;
&nbsp &nbsp &nbsp &nbsp [[Lua]]::[[LuaTableBuilder]] tbl;
&nbsp &nbsp &nbsp &nbsp tbl.AddString("1","Hello World!");
&nbsp &nbsp &nbsp &nbsp tbl.AddNumber("2",25);
&nbsp &nbsp &nbsp &nbsp l.[[SetGlobal(string Name, (insert typename) val)|SetGlobal("value",tbl)]];
&nbsp &nbsp &nbsp &nbsp [[Lua]]::[[PrintTable(lua_State pointer L)|PrintTable(l.[[GetState()]])]]
}

#### Output:
\> 1 = Hello World!
\> 2 = 25