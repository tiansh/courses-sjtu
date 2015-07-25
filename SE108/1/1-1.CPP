#include <iostream>
using namespace std;

#define MAX (1000)

//a、b、c用于检查是否发生冲突 
bool a[MAX*2+1],b[MAX*2+2],c[MAX*2+2];
//p用于记录当前的位置，q用来标记递归开始、结束的位置。 
int p[MAX+1],q[MAX+1];
//n表示棋盘边长，d表示递归深度。
//k表示时间忍受程度，t表示已流逝时间。 
int n,d,k,t;

// 初始化a、b、c使其为空，并随机生成一个棋盘。 
void init(int n) {
  memset(a,false,sizeof(a));
  memset(b,false,sizeof(b));
  memset(c,false,sizeof(c));
  for (int i=1;i<=n;i++)
    { p[i]=rand()%n; q[i]=p[i]+n;}
}

//输出解的情况 
void print() 
  { for (int i=1;i<=n-1;i++) cout<<p[i]%n+1<<" "; cout<<p[n]%n+1<<endl;}

//入栈 
inline void push() 
  { a[p[d]%n]=true;  b[p[d]%n+d]=true;  c[p[d]%n-d+n]=true;  d++; t++;}
//出栈 
inline void pop () 
  { d--; a[p[d]%n]=false; b[p[d]%n+d]=false; c[p[d]%n-d+n]=false;}

int main() {
  srand((unsigned) time(NULL));
  
  while (cin>>n) 
    if (!n || n>=2 && n<=4) break; else for (t=k=long(1.3*n);;)
     if (t>=k) init(n),d=1,t=0; //初始化或重置数据 
      else if (d>n) { print(); break; } //找到解 
      else if (p[d]==q[d]) { pop(); p[d]++;}  //无解，返回上一层 
      else if ((a[p[d]%n]) || (b[p[d]%n+d]) || (c[p[d]%n-d+n])) p[d]++; //冲突 
      else { push(); p[d]=q[d]-n;}; //不冲突，进入下一层 

  return 0;
}
