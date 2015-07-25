function [] = ShowAll( T,l)
  set(rectangle('position',[100 460-(2+10*sqrt(3))*16 8*2*16 (2+10*sqrt(3))*16]), ...
    'EdgeColor' , [1 1 1], 'FaceColor', [0 0 0]);
  for j=1:11
    for i=1:8-mod(l+j,2)
      if T(i,j)==0
        BBall(i,j,l,0);
      else
        BBall(i,j,l,T(i,j));
      end
    end
  end
end