#include <iostream>
using namespace std;

#define push(x,y) a|=1<<(x); b|=1<<(x+y); c|=1<<(x-y+n); y++; //��ջ�� 
#define pop(x,y)  y--; a^=1<<(x); b^=1<<(x+y); c^=1<<(x-y+n); // ��ջ�� 
#define ct(x,y) bool(a & 1<<(x))|(b & 1<<(x+y))|(c & 1<<(x-y+n)) //����ͻ�� 


unsigned int a,b,c; //a��b��c���ڼ���Ƿ�����ͻ�� 
unsigned int n,d; //n��ʾ���̱߳���d��ʾ�ݹ���ȡ�
char p[32]; //p���ڼ�¼��ǰ��λ�á�
unsigned long total=0,t; //total��¼�Ѿ��ҵ��Ľ��������t��¼�Գ��ԡ� 


int main(int paramnum, char* paramstr[]) {
    
  //�������в�������n��  
  if (paramnum<=1) {cerr<<"ȱ�ٲ�����\n"; exit(1); } 
  for (d=0,n=0;paramstr[1][d];d++) 
    if (paramstr[1][d]<'0' || paramstr[1][d]>'9') 
      { cerr<<"��������\n"; exit(1);}
    else n=10*n+paramstr[1][d]-'0';
  if (n>16) {cerr<<"������Χ��\n"; exit(1);} 

  //����n�Ľ⡣ 
  for (p[0]=1;p[0]<=n/2+n%2;) 
   for (a=b=1<<p[0],c=b<<n,p[1]=1,t=(p[0]<=n/2)+1,d=1;d;)
    if (d>=n) { total+=t; pop(p[d],d); p[d]++;} //�ҵ��� 
      else if (p[d]>n) { pop(p[d],d); p[d]++;} //�޽⣬������һ�� 
      else if (ct(p[d],d)) p[d]++; //��ͻ 
      else { push(p[d],d); p[d]=1;}; //����ͻ��������һ�� 
 
  cout<<n<<"�ʺ��ҵ�"<<total<<"��⡣\n"<<endl;
  return 0;
}