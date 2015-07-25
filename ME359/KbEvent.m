function [ arrow,angle,T,next,now,NumBall,l ] = KbEvent( key, arrow, angle, T, next, now,TotalC,NumBall, NumBallText,time,l,timeText,enable)
  if get(enable,'String')=='0'
    return;
  else 
    set(enable,'String','0');
  end
  if strcmp(key,'leftarrow')
    if angle>-pi*.45
      angle=angle-pi/90;
    end
  end
  if strcmp(key,'rightarrow')
    if angle<pi*.45
      angle=angle+pi/90;
    end
  end
  if strcmp(key,'uparrow')
    if angle<0
      angle=angle+pi/90;
    else
      if angle>0
        angle=angle-pi/90;
      end
    end
  end
  if strcmp(key,'downarrow') || strcmp(key,'space')
    stop(time);
    delete(arrow);
    [T]=Shoot(T,angle,now,l);
    [next now NumBall]=RandNext(next,TotalC,NumBall, NumBallText);
    arrow = ShowArrow( angle, line );
    if mod(NumBall,10)==0
      [T l]=NewLine(T,l,TotalC);
      ShowAll;
    end
    set(timeText,'String','9');
    start(time)
  end
  set(enable,'String','1');
  arrow=ShowArrow(angle,arrow);
end