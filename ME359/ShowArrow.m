function [arrow] = ShowArrow( angle, arrow )
   delete(arrow);
   arrow=line([100+8*16-sin(angle)*32, 100+8*16+sin(angle)*48, ...
      100+8*16+sin(angle+pi/45)*42, 100+8*16+sin(angle-pi/45)*42,...
      100+8*16+sin(angle)*48], ...
           [460-(2+10*sqrt(3)+1.8)*16-cos(angle)*32, ...
           460-(2+10*sqrt(3)+1.8)*16+cos(angle)*48, ...
           460-(2+10*sqrt(3)+1.8)*16+cos(angle+pi/45)*42, ...
           460-(2+10*sqrt(3)+1.8)*16+cos(angle-pi/45)*42, ...
           460-(2+10*sqrt(3)+1.8)*16+cos(angle)*48]);
   set(arrow,  'color',[1 1 1]);
   refresh;
end