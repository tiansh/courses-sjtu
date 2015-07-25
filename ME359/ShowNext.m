function [  ] = ShowNext( Color )
  set(text(105,460-(2+10*sqrt(3))*16-24, 'ÏÂÒ»¸ö£º'), ...
      'fontname','Î¢ÈíÑÅºÚ','fontsize',13,'Color',[1 1 1]);
  if Color~=0 
    Ball(140,460-(2+10*sqrt(3))*16-48, Color);
  end;
end