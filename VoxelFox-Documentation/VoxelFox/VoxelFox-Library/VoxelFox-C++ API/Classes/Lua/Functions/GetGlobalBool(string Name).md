## GetGlobalBool(string Name)
#### Description:
gets the global value assosiated with Name from lua;
#### Example :
void main() {
&nbsp &nbsp &nbsp &nbsp Lua l;
&nbsp &nbsp &nbsp &nbsp l.SetGlobal("value",true);
&nbsp &nbsp &nbsp &nbsp printf("%d",l.GetGlobalBool("value"));
}

#### Output:
\> True