function [ T,l ] = NewLine( T,l, TotalC)
  for j=11:-1:1
    for i=1:8
      T(i,j+1)=T(i,j);
    end
  end
  l=l+1;
  for i=1:8-mod(l+1,2)
    T(i,1)=floor(rand*TotalC+1);
  end
  ShowAll(T,l);
end