function [ next, now, NumBall ] = RandNext( next, TotalC, NumBall, NumBallText )
  now=next;
  next=floor(rand*TotalC)+1;
  ShowNow(now)
  ShowNext(next)
  NumBall=NumBall+1;
  set(NumBallText,'String',int2str(NumBall));
end