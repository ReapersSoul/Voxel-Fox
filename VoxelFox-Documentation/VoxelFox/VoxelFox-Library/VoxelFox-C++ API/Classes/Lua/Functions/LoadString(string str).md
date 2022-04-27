## LoadString(string str)
#### Description:
Loads a lua script in from the file system.
#### Example :
void main() {
&nbsp &nbsp &nbsp &nbsp [[VoxelFox/VoxelFox-Library/VoxelFox-C++ API/Classes/Lua/Lua]] l;
&nbsp &nbsp &nbsp &nbsp l.[[LoadString(string str)|LoadString("function hello () print("Hello World!") end")]];
&nbsp &nbsp &nbsp &nbsp l.[[CallLuaFunct(string Name,LuaCFunct ArgumentSetup=nullptr,int args=0,LuaCFunct ReturnedDataHandler=nullptr,int rets=0)|CallLuaFunct("hello")]];
}

#### Output:
\> Hello World!