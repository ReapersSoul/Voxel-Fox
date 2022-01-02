## Exception(string exString)
#### Example :
void main() {
&nbsp &nbsp &nbsp &nbsp try{
&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp throw [[Exception(string exString)|Exception("This is an example!")]];
&nbsp &nbsp &nbsp &nbsp } catch([[Exception]] e){
&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp std::cout<<e.what()<<std::endl;
&nbsp &nbsp &nbsp &nbsp }
}

#### Output:
\> This is an example!