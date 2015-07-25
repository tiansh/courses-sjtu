  %Draw BOX
  set(rectangle('position', ...
      [100 460-(2+10*sqrt(3))*16 8*2*16 (2+10*sqrt(3))*16]), ...
      'EdgeColor' , [1 1 1], 'FaceColor', [0 0 0]);
  set(rectangle('position',[100 460-(2+10*sqrt(3))*16-72 8*2*16 72]), ...
      'EdgeColor' , [1 1 1], 'FaceColor', [0 0 0]);
  
  %Printl

  set(text(400,400, 'ÇòÊý£º'), ...
      'fontname','Î¢ÈíÑÅºÚ','fontsize',13,'Color',[1 1 1]);

  NumBall; NumBall=-2;
  NumBallText=text(440,380,int2str(NumBall), ...
      'fontname','Î¢ÈíÑÅºÚ','fontsize',13,'Color',[1 1 1]);
  
  T; T=zeros(8,12);
  l; l=0;
  for i=1:5
    [T l]=NewLine(T,l,TotalC);
  end
  next=0;
  [next, now, NumBall]=RandNext(next, TotalC, NumBall, NumBallText);
  [next, now, NumBall]=RandNext(next, TotalC, NumBall, NumBallText);
  angle=0; 
  arrow=ShowArrow(angle,line);
  set(g,'KeyPressFcn', ...
    '[arrow,angle,T,next,now,NumBall,l]=KbEvent(get(gcf,''CurrentKey''), arrow, angle, T, next, now,TotalC,NumBall, NumBallText,time,l,timeText,enable);');
  text(100+16*16-50,460-(2+10*sqrt(3))*16-24, 'µ¹Êý', ...
      'fontname','Î¢ÈíÑÅºÚ','fontsize',13,'Color',[1 1 1]);  
  timeText=text(100+16*16-50,460-(2+10*sqrt(3))*16-42, '9', ...
      'fontname','Î¢ÈíÑÅºÚ','fontsize',13,'Color',[1 1 1]);
  enable=text(0,0,'1','Visible','off');
  time=timer('timerfcn','[arrow,angle,T,next,now,NumBall,l]=timer_fcn(''space'', arrow, angle, T, next, now,TotalC,NumBall, NumBallText,time,l,timeText,enable);','StartDelay',1,'TasksToExecute',inf,'ExecutionMode','fixedDelay');
  start(time)