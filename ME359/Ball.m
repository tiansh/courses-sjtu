function [ h ] = Ball( x,y,c )
  R=16; 
  h=Circle(x,y,R);
  ColorTable=[1 .2 0.2; .2 1 .2;.2 .2 1;1 1 .2;
            .2 1 1;1 .2 1;.5 .5 .5;.75 .75 .75];
  if c==0
    set(h,'Color',[0 0 0]);
  else
    set(h,'Color',ColorTable(c,1:3));
  end
end