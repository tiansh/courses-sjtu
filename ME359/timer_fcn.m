function [ arrow,angle,T,next,now,NumBall,l ] = timer_fcn( key, arrow, angle, T, next, now,TotalC,NumBall, NumBallText,time,l, timeText,enable)
  if get(timeText,'String')>'0'
    set(timeText,'String',get(timeText,'String')-49);
  else
    [arrow,angle,T,next,now,NumBall,l]=KbEvent(key, arrow, angle, T, next, now,TotalC,NumBall, NumBallText,time,l,timeText,enable);
  end
end