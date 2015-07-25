function [h] = Circle( x,y,r)
  h=[];
  for R=1:2:r
    h=[h line(x+R*cos([0:1/(2*r):2*pi+1/r]),y+R*sin([0:1/(2*r):2*pi+1/r]))];
  end
end