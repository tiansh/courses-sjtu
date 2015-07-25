function [ list,r ] = pushs( list,x,y,r )
  for i=1:r
    if list(i,1)==x && list(i,2)==y
      break;
    end
  end
  if ~(list(i,1)==x && list(i,2)==y)
     list=[list; x, y];
     r=r+1;
  end
end

