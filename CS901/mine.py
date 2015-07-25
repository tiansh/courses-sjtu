# vim: fileencoding=UTF-8
import pygame, sys, random, time

winObj = 0        # 程序主窗口
imgs = [
	"cover.png", "active.png", "down.png", "boom.png",
	"hit.png", "wrong.png", "right.png", "flag.png", "aflag.png",
	"question.png", "aquest.png", "minewq.png", "win.png",
	"clock.png", "mine.png", "refresh.png", "arefresh.png",
	"drefresh.png", "tick.png", "cross.png",
	"easy.png", "normal.png", "hard.png"
]                 # 预加载的图片
imgfolder = "./image/"    # 图片所在文件夹
colorTable = (
	(224, 224, 240), ( 64,  88, 192), ( 32, 104,   8),
	(168,   8,   8), (  0,   0, 128), (128,   0,   0),
	(  0, 120, 120), (168,   8,   8), (168,   8,  16),
	(255, 255, 255), (  0,   0,   0)
)                 # 调色板，0号为背景色，9号为时间和剩余雷数
imgObj = []       # 预加载图片存放的数组
drawObj = []      # 当前绘制在屏幕上的图形
minefield = []    # 雷阵
mineShow = []     # 当前的显示状态，用mineShowS的枚举值
mineShowS = { "cover":0, "down":1, "flag":2, "question":3 }
mineW, mineH = 32, 32     # 每个方格显示的宽和高
minefX, minefY = 0, 64    # 雷阵左上角在窗口的坐标
infoY = minefY - 56
mousePos = (0, 0) # 当前鼠标位置
mouseButton = (0, 0, 0)   # 当前鼠标按键
fontSize = 24     # 默认字号
gameStateS = {"normal":0, "boom":1, "win":2}      # gameState的枚举值
gameState = gameStateS["normal"]                  # 当前游戏的状态
startTime = 0     # 游戏开始的时间
endTime = 0       # 游戏结束的时间
timeWork = False  # 当前秒表是否在工作
freshStrage = True        # 当前是否为新的一局
diffStr = ["", "easy", "normal", "hard"]  # 三个难度等级
levelS = [(), (9, 9, 10), (16, 16, 40), (30, 16, 99)]
diffLevel = diffStr[1]    # 默认为简单
mineX, mineY, mineTotal = levelS[1]       # 雷阵的宽和高和雷的总数
defaultFont = ["monaco", "dejavuserif", "stsong"] # 程序使用的字体，先出现的优先

# 构造一个文本控件
def textObj(text, color=colorTable[9], size = fontSize, bold = False):
	f = pygame.font.SysFont(defaultFont, size, bold)
	o = f.render(text, False, color)
	return o

# 绘制一个控件
def paintObj(obj, pos):
	winObj.blit(obj, pos)

# 加载图片
# 图片文件名由imgs指定，加载到imgObj
def loadImages():
	global imgObj
	imgObj = [(pygame.image.load(imgfolder+p)) for p in imgs]

# 从imgObj中根据文件名返回对应图片控件
def getImgObj(name):
	m = [p for p in imgs if p.split(".")[0]==name]
	if len(m)==0:
		return
	return imgObj[imgs.index(m[0])]

# 在指定位置绘制一个图片
def paintImg(name, pos):
	winObj.blit(getImgObj(name), pos)

# 初始化计时器
def timerInit():
	global startTime, endTime, timeWork
	startTime = 0
	endTime = 0
	timeWork = False

# 计时器打开
def timerStart():
	global startTime, timeWork
	if not timeWork:
		startTime = time.time()
		timeWork = True

# 计算经过的时间
def timePast():
	if timeWork:
		return time.time() - startTime
	else:
		return endTime - startTime

# 计时器停止
def timerStop():
	global endTime, timeWork
	endTime = time.time()
	timeWork = 0

# 计算剩余的雷的数量
def mineLeft():
	return (mineTotal - 
		sum([1 for x in range(mineX) for y in range(mineY) 
		if (mineShow[x][y]==mineShowS["flag"])]))

# 显示时间和剩余雷数
def showInfo():
	global drawObj
	drawObj["timepast"]["obj"] = textObj(str(int(timePast())), colorTable[10], 32)
	drawObj["mineleft"]["obj"] = textObj("%3d" % mineLeft(), colorTable[10], 32)
	drawObj["mineicon"]["obj"] = { gameStateS["normal"]:getImgObj("mine"),
		gameStateS["boom"]:getImgObj("cross"),
		gameStateS["win"]:getImgObj("tick") }[gameState]
	drawObj["difficulty"]["obj"] = getImgObj(diffLevel)

# 检查一个坐标是否在雷阵的坐标范围内
def inrange(pos):
	x, y = pos
	return x in range(mineX) and y in range(mineY)

# 返回与某个坐标相邻的所有坐标
def neighbor(pos):
	px, py = pos
	p = [(x - 1, y - 1) for x in range(3) for y in range(3)]
	p = [(x + px, y + py) for (x, y) in p]
	p = [n for n in p if inrange(n) and n != pos]
	return p

# 根据鼠标位置计算所在方格
# 如果鼠标不在雷阵上则计算的结果会超出雷阵坐标的范围
def calcBlock(pos):
	ax, ay = pos
	ax, ay = (ax - minefX) // mineW, (ay - minefY) // mineH
	return (ax, ay)

# 显示一个方格
def paintBlock(x, y):
	px, py = x * mineW + minefX, y * mineH + minefY
	ax, ay = calcBlock(mousePos)
	# activeB保存需要高亮显示的坐标
	activeB = [(ax, ay)] 
	if mouseButton[0] and mouseButton[2]:
		activeB += neighbor((ax, ay))
	# 当前为游戏正常进行的状态
	if gameState == gameStateS["normal"]:
		if mineShow[x][y]==mineShowS["cover"]:
			if (x, y) in activeB:
				if mouseButton[0]:
					paintImg("down", (px, py))
				else:
					paintImg("active", (px, py))
			else:
				paintImg("cover", (px, py))
		elif mineShow[x][y]==mineShowS["down"]:
			n = minefield[x][y]
			paintImg("down", (px, py))
			if n > 0:
				textPos = (px + (mineW - fontSize) // 2 + 4, 
							 py + (mineH - fontSize) //2 - 2)
				paintObj(textObj(str(n), colorTable[n], bold = True), textPos)
		elif mineShow[x][y]==mineShowS["flag"]:
			if (x, y) == (ax, ay):
				paintImg("aflag", (px, py))
			else:
				paintImg("flag", (px, py))
		elif mineShow[x][y]==mineShowS["question"]:
			if (x, y) in activeB:
				paintImg("aquest", (px, py))
			else:
				paintImg("question", (px, py))
	# 当前为游戏已经踩到雷的状态
	elif gameState == gameStateS["boom"]:
		isB = minefield[x][y] == -1
		if mineShow[x][y]==mineShowS["cover"]:
			if isB:
				paintImg("boom", (px, py))
			else:
				paintImg("cover", (px, py))
		elif mineShow[x][y]==mineShowS["down"]:
			n = minefield[x][y]
			if isB:
				paintImg("hit", (px, py))
			else:
				paintImg("down", (px, py))
			if n > 0:
				textPos = (px + (mineW - fontSize) // 2 + 4, 
							 py + (mineH - fontSize) //2 - 2)
				paintObj(textObj(str(n), colorTable[n], bold = True), textPos)
		elif mineShow[x][y]==mineShowS["flag"]:
			if isB:
				paintImg("right", (px, py))
			else:
				paintImg("wrong", (px, py))
		elif mineShow[x][y]==mineShowS["question"]:
			if isB:
				paintImg("minewq", (px, py))
			else:
				paintImg("question", (px, py))
	# 当前为游戏已经获胜的状态
	elif gameState == gameStateS["win"]:
		isB = minefield[x][y] == -1
		if mineShow[x][y]!=mineShowS["down"]:
			paintImg("flag", (px, py))
		else:
			n = minefield[x][y]
			paintImg("down", (px, py))
			if n > 0:
				textPos = (px + (mineW - fontSize) // 2 + 4,
							 py + (mineH - fontSize) //2 - 2)
				paintObj(textObj(str(n), colorTable[n], bold = True), textPos)

# 绘制整个雷区
def paintField():
	for x in range(mineX):
		for y in range(mineY):
			paintBlock(x, y)

# 显示按钮
def showButton():
	if onRefresh(mousePos):
		if mouseButton[0]:
			drawObj["refresh"]["obj"] = getImgObj("drefresh")
		else:
			drawObj["refresh"]["obj"] = getImgObj("arefresh")
	else:
		drawObj["refresh"]["obj"] = getImgObj("refresh")
	if onDiff(mousePos):
		diff = diffStr[onDiff(mousePos)]
		drawObj["difficulty"]["obj"] = getImgObj(diff)
	else:
		drawObj["difficulty"]["obj"] = getImgObj(diffLevel)


# 重绘全部内容
def repaint():
	winObj.fill(colorTable[0])
	paintField()
	showInfo()
	showButton()
	for i in drawObj:
		paintObj(drawObj[i]["obj"], drawObj[i]["pos"])
	pygame.display.update()

# 初始化drawObj
def initDrawObj():
	global drawObj
	drawObj = {}
	windowW = mineX * mineW
	drawObj["timeicon"] = {"obj":getImgObj("clock"), "pos":(int(windowW * 0.37 - 72), infoY)}
	drawObj["mineicon"] = {"obj":getImgObj("mine"),  "pos":(int(windowW * 0.73 + 72) - 48, infoY)}
	drawObj["timepast"] = {"obj":textObj(""), "pos":(int(windowW * 0.40 - 72) + 48, infoY + 4)}
	drawObj["mineleft"] = {"obj":textObj(""), "pos":(int(windowW * 0.70 + 72) - 112, infoY + 4)}
	drawObj["refresh"]  = {"obj":getImgObj("refresh"), "pos":(windowW * 0.55 - 24, infoY)}
	drawObj["difficulty"] = {"obj":getImgObj(diffLevel), "pos":(windowW * 0.05 - 16, infoY)}

# 初始化字体
def fontInit():
	global defaultFont
	defaultFont = [i for i in defaultFont if i in pygame.font.get_fonts()][0]

# 主初始化
def mainInit():
	pygame.init()
	loadImages()
	fontInit()

# 初始化游戏窗口
def winInit():
	global winObj
	winObj = pygame.display.set_mode((mineX * 32, mineY * 32 + minefY))
	pygame.display.set_caption("Mine")
	initDrawObj()

# 初始化雷区
# 此处只随机选取mineTotal个方格标记为雷
# 并不计算周围的方格的数字
# 周围方格的计算等到点开第一块的时候进行
def fieldInit():
	global minefield, mineShow
	minefield = [[0 for y in range(mineY)] for x in range(mineX)]
	mineShow = [[mineShowS["cover"] for y in range(mineY)] for x in range(mineX)]
	boom = [(x, y) for y in range(mineY) for x in range(mineX)]
	boom = sorted(boom, key=(lambda x:random.random()))[:mineTotal]
	for (x, y) in boom:
		minefield[x][y]=-1

# 一局游戏的初始化
def strageInit():
	global gameState, freshStrage 
	fieldInit()
	gameState = gameStateS["normal"]
	timerInit()
	freshStrage = True

# 检查当前是否已经踩到雷或者已经获胜
def checkState():
	d = [(x, y) for y in range(mineY) for x in range(mineX)
		if mineShow[x][y] == mineShowS["down"]]
	for x, y in d:
		if minefield[x][y] == -1:
			return gameStateS["boom"]
	if len(d) + mineTotal == mineX * mineY:
		return gameStateS["win"]
	return gameStateS["normal"]

# 移出pos及其周围的雷
# 这是为了保证每局游戏第一次点击总会点到空白的地区
# 同时计算所有不是雷的方格的数值
def rmMine(pos):
	global freshStrage
	freshStrage = False
	neighbors = neighbor(pos) + [pos]
	neighborm = [p for p in neighbors if minefield[p[0]][p[1]] == -1]
	zeros = [(x, y) for x in range(mineX) for y in range(mineY)
		if minefield[x][y] != -1 and (x, y) not in neighbors]
	zeros = sorted(zeros, key=(lambda x:random.random()))[:len(neighborm)]
	for p in neighborm:
		minefield[p[0]][p[1]] = 0
	for p in zeros:
		minefield[p[0]][p[1]] = -1
	m = [(x, y) for x in range(mineX) for y in range(mineY)
		if minefield[x][y] == -1]
	for p in m:
		for pp in neighbor(p):
			if minefield[pp[0]][pp[1]] != -1:
				minefield[pp[0]][pp[1]] += 1

# 左击pos位置的反应
def clickL(pos):
	global mineShow
	if freshStrage:
		rmMine(pos)
		timerStart()
	x, y = pos
	if mineShow[x][y] == mineShowS["flag"]:
		return
	mineShow[x][y] = mineShowS["down"]
	if minefield[x][y] == 0:
		dc = neighbor(pos)
		# 如果这个位置是0需要打开所有周围的方格
		# 宽度优先遍历
		while len(dc):
			x0, y0 = dc[0]
			dc = dc[1:]
			if mineShow[x0][y0]==mineShowS["down"]:
				continue
			mineShow[x0][y0]=mineShowS["down"]
			if minefield[x0][y0] == 0:
				dc += neighbor((x0, y0))

# 右击时的处理
def clickR(pos):
	x, y = pos
	if mineShow[x][y] == mineShowS["cover"]:
		mineShow[x][y]=mineShowS["flag"]
	elif mineShow[x][y] == mineShowS["flag"]:
		mineShow[x][y]=mineShowS["question"]
	elif mineShow[x][y] == mineShowS["question"]:
		mineShow[x][y]=mineShowS["cover"]

# 左右同时击键时的处理
def clickLR(pos):
	global mineShow, gameState
	x, y = pos
	if mineShow[x][y] != mineShowS["down"]:
		return
	m = neighbor(pos)
	c = [(px, py) for px, py in m if mineShow[px][py] == mineShowS["flag"]]
	if len(c) != minefield[x][y]:
		return
	for px, py in m:
		if mineShow[px][py] != mineShowS["flag"]:
			clickL((px, py))

# 判断鼠标是否在重开按钮上
def onRefresh(pos):
	return (pos[0] - (mineX * mineW) * 0.55) ** 2 + (pos[1] - 32) ** 2 < 24 ** 2

# 判断鼠标是否在难度选择按钮上
def onDiff(pos):
	windowW = mineX * mineW
	if pos[0] in range(int(windowW * 0.05 - 14), int(windowW * 0.05 + 15)):
		if pos[1] in range(infoY + 2, infoY + 47):
			return 4 - ((pos[1] - infoY + 13) // 15)
	return 0

# 重新设置难度
def changeDiff(level):
	global diffLevel, mineX, mineY, mineTotal
	diffLevel = ["", "easy", "normal", "hard"][level]
	mineX, mineY, mineTotal = levelS[level]
	winInit()
	strageInit()


# 处理事件
def handleEvent(event):
	global mousePos, mouseButton, gameState
	if event.type == pygame.QUIT:
		pygame.quit()
		sys.exit()
	elif event.type == pygame.MOUSEMOTION:
		mousePos = event.pos
		if sum(pygame.mouse.get_pressed()) == 0:
			mouseButton = (0, 0, 0)
	elif event.type == pygame.MOUSEBUTTONDOWN:
		button = pygame.mouse.get_pressed()
		mouseButton = tuple([mouseButton[i] or button[i] for i in range(3)])
	elif event.type == pygame.MOUSEBUTTONUP:
		mouseButton = list(mouseButton)
		mouseButton[event.button - 1] = 1
		block = calcBlock(mousePos)
		if inrange(block):
			if gameState != gameStateS["normal"]:
				return;
			if mouseButton[0] and mouseButton[2]:
				clickLR(block)
			elif mouseButton[0]:
				clickL(block)
			elif mouseButton[2]:
				clickR(block)
			gameState = checkState()
			if gameState != gameStateS["normal"]:
				timerStop()
		elif onRefresh(mousePos):
			strageInit()
		elif onDiff(mousePos):
			changeDiff(onDiff(mousePos))
	elif event.type == pygame.KEYDOWN:
		if event.key == pygame.K_F2:
			strageInit()
		elif (event.key == pygame.K_ESCAPE or
				event.key == pygame.K_F4 and event.mod & pygame.KMOD_ALT):
			pygame.event.post(pygame.event.Event(pygame.QUIT))


		mouseButton = (0, 0, 0)

# 主循环
def mainLoop():
	ltime = int(timePast())
	action = True
	while True:
		if action or int(timePast()) != ltime:
			repaint()
			ltime = int(timePast())
		for event in pygame.event.get():
			handleEvent(event)
			action = True

# 程序主体
mainInit()
winInit()
strageInit()
mainLoop()
