## GetGlobalString(string Name)
#### Description:
gets the global value assosiated with Name from lua;
#### Example :
void main() {
&nbsp &nbsp &nbsp &nbsp Lua l;
&nbsp &nbsp &nbsp &nbsp l.SetGlobal("value","Hello World!");
&nbsp &nbsp &nbsp &nbsp printf("%s",l.GetGlobalString("value"));
}

#### Output:
\> Hello World!