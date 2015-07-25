# University of Washington, Programming Languages, Homework 6, hw6runner.rb

require_relative './hw6graphics'

class MyPiece < Piece
  
  def self.next_piece (board, cheat = false)
    MyPiece.new(cheat ? [[[0, 0]]] : All_My_Pieces.sample, board) 
  end
  
  All_My_Pieces = [[[[0, 0], [1, 0], [0, 1], [1, 1]]], 
                  rotations([[0, 0], [-1, 0], [1, 0], [0, -1]]), 
                  [[[0, 0], [-1, 0], [1, 0], [2, 0]], 
                  [[0, 0], [0, -1], [0, 1], [0, 2]]],
                  rotations([[0, 0], [0, -1], [0, 1], [1, 1]]), 
                  rotations([[0, 0], [0, -1], [0, 1], [-1, 1]]), 
                  rotations([[0, 0], [-1, 0], [0, -1], [1, -1]]), 
                  rotations([[0, 0], [1, 0], [0, -1], [-1, -1]]), 
                  rotations([[0, 0], [-1, 0], [1, 0], [0, -1], [-1, -1]]), 
                  [[[0, 0], [-1, 0], [1, 0], [2, 0], [-2, 0]], 
                  [[0, 0], [0, -1], [0, 1], [0, 2], [0, -2]]],
                  rotations([[0, 0], [-1, 0], [0, 1]])] 


end



class MyBoard < Board

  def initialize (game)
    @grid = Array.new(num_rows) {Array.new(num_columns)}
    @current_block = MyPiece.next_piece(self)
    @score = 0
    @game = game
    @delay = 500
    @cheat_enabled = false
  end
   
  def rotate_half_circle
    if !game_over? and @game.is_running?
      @current_block.move(0, 0, 2)
    end
    draw
  end
  
  def next_piece
    @current_block = MyPiece.next_piece(self, @cheat_enabled)
    @cheat_enabled = false
    @current_pos = nil
  end

  def store_current
    locations = @current_block.current_rotation
    displacement = @current_block.position
    locations.size.times{|index| 
      current = locations[index];
      @grid[current[1]+displacement[1]][current[0]+displacement[0]] = 
      @current_pos[index]
    }
    remove_filled
    @delay = [@delay - 2, 80].max
  end

  def cheat_next_time
    return if @score < 100 or @cheat_enabled or game_over?
    @cheat_enabled = true
    @score -= 100
    @game.update_score
  end

end

class MyTetris < Tetris

  def set_board
    @canvas = TetrisCanvas.new
    @board = MyBoard.new(self)
    @canvas.place(@board.block_size * @board.num_rows + 3,
                  @board.block_size * @board.num_columns + 6, 24, 80)
    @board.draw
  end

  def key_bindings  
    @root.bind('u', lambda {@board.rotate_half_circle}) 
    @root.bind('c', lambda {@board.cheat_next_time}) 
    super
  end
end

############################################################################
# 附加题的分割线
############################################################################

class MyTetrisRootChallenge < TetrisRoot
  def initialize
    @root = TkRoot.new(
      'height' => 600,
      'width' => 800,
      'background' => '#333',
      'resizable' => [false, false],
    ) {title "方块"}    
  end

  def bind(char, callback)
    @root.bind(char, callback)
  end

  attr_reader :root
end

class MyTetrisTimerChallenge < TetrisTimer
  def initialize
    @timer = TkTimer.new
  end

  def stop
    @timer.stop
  end

  def start(delay, callback)
    @timer.start(delay, callback)
  end
end

class MyTetrisCanvasChallenge < TetrisCanvas
  def initialize
    @canvas = TkCanvas.new(@root) {
      background 'black'
      borderwidth 0
    }
  end

  def place(height, width, x, y)
    @canvas.place(
      'height' => height,
      'width' => width,
      'x' => x,
      'y' => y
    )
  end

  def unplace
    @canvas.unplace
  end

  def delete
    @canvas.delete
  end

  attr_reader :canvas
end

class MyTetrisLabelChellenge < TetrisLabel
  def initialize(wrapped_root, &options)
    unwrapped_root = wrapped_root.root
    @label = TkLabel.new(unwrapped_root, &options)
  end

  def place(height, width, x, y)
    @label.place('height' => height, 'width' => width, 'x' => x, 'y' => y)
  end

  def unplace()
    @label.unplace
  end

  def text(str)
    @label.text(str)
  end
end

class MyTetrisRectChallenge < TetrisRect
  def initialize(wrapped_canvas, a, b, c, d, color)
    unwrapped_canvas = wrapped_canvas.canvas
    @rect = TkcRectangle.new(unwrapped_canvas, a, b, c, d, 
                             'outline' => 'black', 'fill' => color)
  end

  def remove
    @rect.remove
  end

  def move(dx, dy)
    @rect.move(dx, dy)
  end
end

class MyPieceChallenge < MyPiece

  def initialize (point_array, color, board)
    @all_rotations = point_array
    @rotation_index = (0..(@all_rotations.size-1)).to_a.sample
    @color = color
    @base_position = [5, 0]
    @board = board
    @moved = true
  end
  
  def self.next_piece (board)
    shape, color = All_My_Pieces_Challenge.sample
    MyPieceChallenge.new(shape, color, board) 
  end
  
  # 只要正经的七种
  All_My_Pieces_Challenge = [
    [[[[0, 0], [1, 0], [0, 1], [1, 1]]], "#4040ff"],  # 口
    [rotations([[0, 0], [-1, 0], [1, 0], [0, -1]]), "#888800"], # 丄
    [[[[0, 0], [-1, 0], [1, 0], [2, 0]], # 丨
     [[0, 0], [0, -1], [0, 1], [0, 2]]], "#e05000"],
    [rotations([[0, 0], [0, -1], [0, 1], [1, 1]]), "#f060ff"], # 乛
    [rotations([[0, 0], [0, -1], [0, 1], [-1, 1]]), "#ffb43a"],  # 乚
    [[[[0, 0], [-1, 0], [0, -1], [1, -1]], # 乙
     [[0, 0], [0, -1], [1, 0], [1, 1]]], "#4090c0"],
    [[[[0, 0], [1, 0], [0, -1], [-1, -1]], # 丂
     [[0, 0], [0, -1], [-1, 0], [-1, 1]]], "#48a810"]
  ]

  def move (delta_x, delta_y, delta_rotation)
    return true if super(delta_x, delta_y, delta_rotation)
    return false if delta_rotation == 0
    return true if super(delta_x + 1, delta_y, delta_rotation)
    return true if super(delta_x - 1, delta_y, delta_rotation)
    false
  end

end

class MyBoardChallenge < MyBoard

  def block_size
    25
  end

  def num_rows
    20
  end

  def lines
    @lines
  end

  def level
    @level
  end

  def delay
    (572 * (7.0 / 8) ** @level).ceil
  end

  def run
    ran = @current_block.drop_by_one
    if !ran
      store_current
      if game_over?
        show_game_over
      else
        next_piece
      end
    end
    @game.update_score
    draw
  end

  def initialize (game, level = 0)
    @grid = Array.new(num_rows) {Array.new(num_columns)}
    next_piece
    @score = 0
    @lines = 0
    @level = level
    @game = game
    @cheat_enabled = false
  end

  def move_down
    if !game_over? and @game.is_running?
      run
    end
    draw
  end

  def show_game_over
    (2..(@grid.size-1)).each{|num|
      (0..(num_columns-1)).each{|index|
        next if not @grid[num][index]
        @game.draw_brick([index, num], '#333')
      }
    }
  end

  def drop_all_the_way
    return if game_over? or !@game.is_running?
    ran = @current_block.drop_by_one
    while ran
      @current_pos.each{|block| block.remove}
      ran = @current_block.drop_by_one
    end
    draw
    store_current
    if !game_over?
      next_piece
    end
    @game.update_score
    draw
  end
   
  def next_piece
    @current_block = @next_block
    @next_block = MyPieceChallenge.next_piece(self)
    @cheat_enabled = false
    @current_pos = nil
    @game.show_next(@next_block) if @current_block
    @next_block
  end

  def store_current
    maxy = 0
    locations = @current_block.current_rotation
    displacement = @current_block.position
    locations.size.times{|index| 
      current = locations[index];
      @grid[current[1]+displacement[1]][current[0]+displacement[0]] = 
      @current_pos[index]
      maxy = [maxy, current[1]+displacement[1]].max
    }
    @score += (@level + 1) * (num_rows - maxy) * 2
    remove_filled
  end

  def remove_filled
    line = 0
    (2..(@grid.size-1)).each{|num|
      row = @grid.slice(num);
      if @grid[num].all?
        (0..(num_columns-1)).each{|index|
          @grid[num][index].remove;
          @grid[num][index] = nil
        }
        ((@grid.size - num + 1)..(@grid.size)).each{|num2|
          @grid[@grid.size - num2].each{|rect| rect && rect.move(0, block_size)};
          @grid[@grid.size-num2+1] = Array.new(@grid[@grid.size - num2])
        }
        @grid[0] = Array.new(num_columns);
        line += 1
      end
    }
    added_line = @lines + line
    moder = added_line > 150 ? 50 : 30
    if added_line / moder != @lines / moder and @level < 17
      @level += 1
    end
    @lines += line
    @score += 100 * line ** 2
    self
  end


end

class MyTetrisChallenge < MyTetris

  def initialize
    @root = MyTetrisRootChallenge.new
    @timer = MyTetrisTimerChallenge.new

    key_bindings
    menu
    infoarea

    set_board(0)
    update_score
    @running = true
    run_game

    @board.next_piece
    show_next(@board.next_piece)

    pauselabel

  end

  def pause
    if @running
      return if @board.game_over?
      @running = false
      @timer.stop
      @pausel.place(@board.block_size * @board.num_rows + 3,
                    @board.block_size * @board.num_columns + 6,
                    400, 30)
    else
      @pausel.unplace
      @running = true
      self.run_game
    end
  end

  def run_game
    if !@board.game_over? and @running
      @timer.stop
      @timer.start(@board.delay, (lambda {|x| @board.run; run_game}))
    end
  end

  def set_board(level = 0)
    @canvas = MyTetrisCanvasChallenge.new
    @board = MyBoardChallenge.new(self, level)
    @canvas.place(@board.block_size * @board.num_rows + 3,
                  @board.block_size * @board.num_columns + 6,
                  400, 30)
  end

  def key_bindings  
    @root.bind('F2', lambda {self.new_game(@board.level)}) 
    @root.bind('Escape', lambda {exitProgram})
    @root.bind('Return', lambda {self.pause}) 
    
    @root.bind('a', lambda {@board.move_left})
    @root.bind('Left', lambda {@board.move_left}) 
    
    @root.bind('e', lambda {@board.move_right})
    @root.bind('d', lambda {@board.move_right})
    @root.bind('Right', lambda {@board.move_right}) 

    @root.bind('o', lambda {@board.move_down})
    @root.bind('s', lambda {@board.move_down})
    @root.bind('Down', lambda {@board.move_down})

    @root.bind(';', lambda {@board.rotate_counter_clockwise})
    @root.bind('z', lambda {@board.rotate_counter_clockwise})
    @root.bind('h', lambda {@board.rotate_counter_clockwise})
    @root.bind('j', lambda {@board.rotate_counter_clockwise})
    @root.bind('Prior', lambda {@board.rotate_counter_clockwise})

    @root.bind('q', lambda {@board.rotate_clockwise})
    @root.bind('x', lambda {@board.rotate_clockwise})
    @root.bind('t', lambda {@board.rotate_clockwise})
    @root.bind('k', lambda {@board.rotate_clockwise})
    @root.bind('Next', lambda {@board.rotate_clockwise})
    
    @root.bind('space' , lambda {@board.drop_all_the_way}) 
  end

  def new_game(level)
    @canvas.unplace
    @canvas.delete
    set_board(level)
    @board.next_piece
    show_next(@board.next_piece)
    update_score
    @running = true
    run_game
  end

  def show_about
    old_running = false
    if @running
      old_running = true
      @running = false
      @timer.stop
    end
    msgBox = Tk.messageBox(
      'type'    => "ok",  
      'icon'    => "info", 
      'title'   => "关于",
      'message' => ""
    )
    if old_running
      @running = true
      run_game
    end
  end

  def show_keys
    old_running = false
    if @running
      old_running = true
      @running = false
      @timer.stop
    end
    msgBox = Tk.messageBox(
      'type'    => "ok",  
      'icon'    => "info", 
      'title'   => "按键",
      'message' =>
<<-KEYS
F2：重新开始
Esc：退出
Enter：暂停/恢复

A、左箭头：向左移动
E、D、右键头：向右移动
O、S、下箭头：向下移动
;、Z、H、J、上翻页：逆时针转动
Q、X、T、K、下翻页：顺时针转动
空格：落到最下
KEYS
    )
    if old_running
      @running = true
      run_game
    end
  end

  def update_score
    @score.text(@board.score)
    @lines.text(@board.lines)
    @level.text(@board.level)
  end

  def menu
    menu_bar = TkMenu.new
    level_menu = TkMenu.new(@root.root)
    level_menu.add('command',
                   'label'     => "当前等级",
                   'command'   => lambda {self.new_game(@board.level)},
                   'underline' => 0)
    10.times { |level|
      level_menu.add('command',
                     'label'     => "等级%d" % [level],
                     'command'   => lambda {self.new_game(level)},
                     'underline' => 0)
    }
    game_menu = TkMenu.new(@root.root)
    game_menu.add('cascade',
              'label'     => "新建",
              'menu'      => level_menu,
              'underline' => 0)
    game_menu.add('command',
              'label'     => "暂停",
              'command'   => lambda {self.pause},
              'underline' => 0)
    game_menu.add('command',
              'label'     => "退出",
              'command'   => lambda {exitProgram},
              'underline' => 0)
    menu_bar.add('cascade',
             'menu'  => game_menu,
             'label' => "游戏",
             'underline' => 0)
    help_menu = TkMenu.new(@root.root)
    help_menu.add('command',
              'label'     => "按键",
              'command'   => lambda {self.show_keys},
              'underline' => 0)
    help_menu.add('command',
              'label'     => "关于",
              'command'   => lambda {self.show_about},
              'underline' => 0)
    menu_bar.add('cascade',
             'menu'  => help_menu,
             'label' => "帮助",
             'underline' => 0)
    @root.root.menu(menu_bar)
  end

  def infolabel(title, index)
    label = MyTetrisLabelChellenge.new(@root) do
      font ["Microsoft YaHei UI", 16]
      text title
      fg 'white'
      bg 'black'
      justify 'right'
    end
    label.place(50, 100, 100, 250 + 50 * index)
    value = MyTetrisLabelChellenge.new(@root) do
      font ["Microsoft YaHei UI", 16]
      fg 'white'
      bg 'black'
      justify 'left'
      borderwidth 5
    end
    value.place(50, 100, 200, 250 + 50 * index)    
    value
  end

  def infoarea
    @level = infolabel('等级', 1)
    @score = infolabel('得分', 2)
    @lines = infolabel('行数', 3)
    (MyTetrisLabelChellenge.new(@root) do
      font ["Microsoft YaHei UI", 16]
      text '接下来'
      fg 'white'
      bg 'black'
      justify 'right'
    end).place(50, 200, 100, 50)    
  end

  def pauselabel
    @pausel = MyTetrisLabelChellenge.new(@root) do
      font ["Microsoft YaHei UI", 16]
      text '暂停' 
      fg 'white'
      bg 'black'
      justify 'right'
    end
  end

  def show_next(piece)
    if @nextc
      @nextc.unplace
      @nextc.delete
    end
    @nextc = MyTetrisCanvasChallenge.new
    @nextc.place(200, 200, 100, 100)    
    blocks = piece.current_rotation
    blocks.map{|block| 
      draw_brick([3.5 + block[0], 3.5 + block[1]], piece.color, @nextc)
    }
  end

  def draw_brick (position, color, canvas = @canvas) 
    x, y = position
    s = @board.block_size
    TetrisRect.new(canvas, x * s + 3, y * s, (x + 1) * s + 3, (y + 1) * s, color)
  end

  def draw_piece (piece, old=nil, canvas = @canvas)
    if old != nil and piece.moved
      old.each{|block| block.remove}
    end
    blocks = piece.current_rotation
    start = piece.position
    blocks.map{|block| 
      draw_brick([start[0] + block[0], start[1] + block[1]], piece.color, canvas)
    }
  end

end

