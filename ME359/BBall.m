function [ h ] = BBall( x,y,l,c )
  R=16;
  h=Ball(100+R*(2*x+mod(l+y,2))-R,460-R-R*sqrt(3)*(y-1),c);
end