## LoadScript(string FilePath)
#### Description:
Loads a lua script in from the file system.
#### Example :

##### Test.lua :
function hello () 
&nbsp &nbsp &nbsp &nbsp print("Hello World!")
end

##### Main File :
void main() {
&nbsp &nbsp &nbsp &nbsp [[VoxelFox/VoxelFox-Library/VoxelFox-C++ API/Classes/Lua/Lua]] l;
&nbsp &nbsp &nbsp &nbsp l.[[LoadScript(string FilePath)|LoadScript("Test.lua")]];
&nbsp &nbsp &nbsp &nbsp l.[[CallLuaFunct(string Name,LuaCFunct ArgumentSetup=nullptr,int args=0,LuaCFunct ReturnedDataHandler=nullptr,int rets=0)|CallLuaFunct("hello")]];
}

#### Output:
\> Hello World!