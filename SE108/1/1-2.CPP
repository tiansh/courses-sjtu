#include <iostream>
using namespace std;

#define push(x,y) a|=1<<(x); b|=1<<(x+y); c|=1<<(x-y+n); y++; //入栈。 
#define pop(x,y)  y--; a^=1<<(x); b^=1<<(x+y); c^=1<<(x-y+n); // 出栈。 
#define ct(x,y) bool(a & 1<<(x))|(b & 1<<(x+y))|(c & 1<<(x-y+n)) //检查冲突。 


unsigned int a,b,c; //a、b、c用于检查是否发生冲突。 
unsigned int n,d; //n表示棋盘边长，d表示递归深度。
char p[32]; //p用于记录当前的位置。
unsigned long total=0,t; //total记录已经找到的解的数量。t记录对称性。 


int main(int paramnum, char* paramstr[]) {
    
  //将命令行参数传给n。  
  if (paramnum<=1) {cerr<<"缺少参数！\n"; exit(1); } 
  for (d=0,n=0;paramstr[1][d];d++) 
    if (paramstr[1][d]<'0' || paramstr[1][d]>'9') 
      { cerr<<"参数错误！\n"; exit(1);}
    else n=10*n+paramstr[1][d]-'0';
  if (n>16) {cerr<<"超出范围！\n"; exit(1);} 

  //搜索n的解。 
  for (p[0]=1;p[0]<=n/2+n%2;) 
   for (a=b=1<<p[0],c=b<<n,p[1]=1,t=(p[0]<=n/2)+1,d=1;d;)
    if (d>=n) { total+=t; pop(p[d],d); p[d]++;} //找到解 
      else if (p[d]>n) { pop(p[d],d); p[d]++;} //无解，返回上一层 
      else if (ct(p[d],d)) p[d]++; //冲突 
      else { push(p[d],d); p[d]=1;}; //不冲突，进入下一层 
 
  cout<<n<<"皇后共找到"<<total<<"组解。\n"<<endl;
  return 0;
}
