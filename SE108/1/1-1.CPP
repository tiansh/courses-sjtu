#include <iostream>
using namespace std;

#define MAX (1000)

//a��b��c���ڼ���Ƿ�����ͻ 
bool a[MAX*2+1],b[MAX*2+2],c[MAX*2+2];
//p���ڼ�¼��ǰ��λ�ã�q������ǵݹ鿪ʼ��������λ�á� 
int p[MAX+1],q[MAX+1];
//n��ʾ���̱߳���d��ʾ�ݹ���ȡ�
//k��ʾʱ�����̶ܳȣ�t��ʾ������ʱ�䡣 
int n,d,k,t;

// ��ʼ��a��b��cʹ��Ϊ�գ����������һ�����̡� 
void init(int n) {
  memset(a,false,sizeof(a));
  memset(b,false,sizeof(b));
  memset(c,false,sizeof(c));
  for (int i=1;i<=n;i++)
    { p[i]=rand()%n; q[i]=p[i]+n;}
}

//��������� 
void print() 
  { for (int i=1;i<=n-1;i++) cout<<p[i]%n+1<<" "; cout<<p[n]%n+1<<endl;}

//��ջ 
inline void push() 
  { a[p[d]%n]=true;  b[p[d]%n+d]=true;  c[p[d]%n-d+n]=true;  d++; t++;}
//��ջ 
inline void pop () 
  { d--; a[p[d]%n]=false; b[p[d]%n+d]=false; c[p[d]%n-d+n]=false;}

int main() {
  srand((unsigned) time(NULL));
  
  while (cin>>n) 
    if (!n || n>=2 && n<=4) break; else for (t=k=long(1.3*n);;)
     if (t>=k) init(n),d=1,t=0; //��ʼ������������ 
      else if (d>n) { print(); break; } //�ҵ��� 
      else if (p[d]==q[d]) { pop(); p[d]++;}  //�޽⣬������һ�� 
      else if ((a[p[d]%n]) || (b[p[d]%n+d]) || (c[p[d]%n-d+n])) p[d]++; //��ͻ 
      else { push(); p[d]=q[d]-n;}; //����ͻ��������һ�� 

  return 0;
}