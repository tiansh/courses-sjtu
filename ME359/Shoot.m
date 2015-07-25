function [ T ] = Shoot( T, angle, Color, l )
  ShowNow(0);
  x=100+8*16; y=460-(2+10*sqrt(3)+1.8)*16;
  y0=y;
  t=0;
  b=Ball(x,y,Color);
  while y0<460-16
    t=t+6;
    x0=x+sin(angle)*t;
    y0=y+cos(angle)*t;
    if x0<100+16
      x=2*(100+16)-x;
      angle=-angle;
    end
    if x0>100+8*2*16-16
      x=2*(100+8*2*16-16)-x;
      angle=-angle;
    end
    x0=x+sin(angle)*t;
    delete(b);
    
    flag=0; R=16;
    for j=1:11
      for i=1:8-mod(l+j,2)
        if T(i,j)~=0
          if sqrt((100+R*(2*i+mod(l+j,2))-R-x0)^2+(460-R-R*sqrt(3)*(j-1)-y0)^2)<2*R
            flag=1;
          end
        end
      end
    end
    if flag==1 
      break;
    end
    
    b=Ball(x0,y0,Color);
    refresh;
    pause(.001);
  end
  
  
  min=1024;
  min_i=0; min_j=0;
  for j=1:11
    for i=1:8-mod(l+j,2)
      if T(i,j)==0
        if sqrt((100+R*(2*i+mod(l+j,2))-R-x0)^2+(460-R-R*sqrt(3)*(j-1)-y0)^2)<min
          min=sqrt((100+R*(2*i+mod(l+j,2))-R-x0)^2+(460-R-R*sqrt(3)*(j-1)-y0)^2);
          min_i=i; min_j=j;
        end
      end
    end
  end
  
  T(min_i,min_j)=Color;
  BBall(min_i,min_j,l,Color);

  list=[min_i, min_j];
  left=1; right=1;
  while left<=right
    if list(left,1)-1>0
    if T(list(left,1),list(left,2))==T(list(left,1)-1,list(left,2))
     [list,right]=Pushs(list,list(left,1)-1, list(left,2),right);
    end; end
    if list(left,1)+1<=8-mod(l+list(left,2),2)
    if T(list(left,1),list(left,2))==T(list(left,1)+1,list(left,2))
      [list,right]=Pushs(list,list(left,1)+1, list(left,2),right);
    end; end
    if list(left,2)-1>0 && list(left,1)<=8-mod(l+list(left,2)-1,2)
    if T(list(left,1),list(left,2))==T(list(left,1),list(left,2)-1)
      [list,right]=Pushs(list,list(left,1), list(left,2)-1,right);
    end; end
    if list(left,2)+1<=11 && list(left,1)<=8-mod(l+list(left,2)+1,2)
    if T(list(left,1),list(left,2))==T(list(left,1),list(left,2)+1)
      [list,right]=Pushs(list,list(left,1), list(left,2)+1,right);
    end; end
    if list(left,2)-1>0 && list(left,1)+mod(l+list(left,2),2)*2-1>0 && list(left,1)+mod(l+list(left,2),2)*2-1<=8-mod(l+list(left,2)+1,2)
    if T(list(left,1),list(left,2))==T(list(left,1)+mod(l+list(left,2),2)*2-1,list(left,2)-1)
      [list,right]=Pushs(list,list(left,1)+mod(l+list(left,2),2)*2-1,list(left,2)-1,right);
    end; end;
    if list(left,2)+1<=11 && list(left,1)+mod(l+list(left,2),2)*2-1>0 && list(left,1)+mod(l+list(left,2),2)*2-1<=8-mod(l+list(left,2)+1,2)
    if T(list(left,1),list(left,2))==T(list(left,1)+mod(l+list(left,2),2)*2-1,list(left,2)+1)
      [list,right]=Pushs(list,list(left,1)+mod(l+list(left,2),2)*2-1,list(left,2)+1,right);
    end; end
    left=left+1;
  end
  if right>=3
    for i=1:right
      T(list(i,1),list(i,2))=0;
    end
  end
  F=zeros(8,12);
  for i=1:8-mod(l+1,2)
    F(i,1)=1;
  for j=1:11
    for i=1:8-mod(l+j,2)
      if T(i,j)~=0
        if i-1>0
        if F(i-1,j)==1
          F(i,j)=1;
        end; end;
        if j-1>0
        if F(i,j-1)==1
          F(i,j)=1;
        end; end;
        if j-1>0 && 0<i+mod(l+j,2)*2-1 && i+mod(l+j,2)*2-1<=8-mod(l+j-1,2)
        if F(i+mod(l+j,2)*2-1,j-1)==1
          F(i,j)=1;
        end; end;
      end
    end
    for i=8-mod(l+j,2)-1:-1:1
      if T(i,j)~=0
        if F(i+1,j)==1
          F(i,j)=1;
        end;
      end
    end
  end
  for j=1:11
    for i=1:8-mod(l+j,2)
      if F(i,j)==0
        T(i,j)=0;
      end
    end
  end
  ShowAll(T,l);
end