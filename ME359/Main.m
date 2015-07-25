%Main Figure
g=figure;
set(g,'resize','off');
MP=get(0,'MonitorPositions');
set(g,'position',[ MP(3)/2-320 MP(4)/2-240 640 480]);
set(g,'menubar','none','name','');
m_g=uimenu(g,'label','游戏(&G)');
m_g_s=uimenu(m_g,'label','开始(&S)');
m_g_s_4=uimenu(m_g_s,'label','简单（&4色）','Callback', ...
  'set(m_g_s,''Enable'',''off''); TotalC=4; Init;');
m_g_s_6=uimenu(m_g_s,'label','普通（&6色）','Callback', ...
  'set(m_g_s,''Enable'',''off''); TotalC=6; Init;');
m_g_s_8=uimenu(m_g_s,'label','困难（&8色）','Callback', ...
  'set(m_g_s,''Enable'',''off''); TotalC=8; Init;');
m_g_e=uimenu(m_g,'label','退出(&Q)','callback','close gcf;');
m_h=uimenu(g,'label','帮助(&H)');
m_h_a=uimenu(m_h,'label','关于(&A)','callback', ...
  ['msgbox(''Matlab课程大作业' ...
  '您看着尽量多给分！'',''关于'',''help'')']);
e=axes;
set(e,'position',[0 0 1 1],'Color',[0 0 0], ...
      'xlim',[1 640], 'ylim',[1 480])
  
%Constants AND Variables
T=zeros(8,12); l=0;
NumBall=0;
