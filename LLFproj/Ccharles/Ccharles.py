import pygame
from pygame import *

pygame.init()

#播放背景音乐
bgm='music/foolish_game.mp3'
pygame.mixer.init()
pygame.mixer.music.load(bgm)
pygame.mixer.music.play(-1)

#加载碰到星星的音效
starsound='music/starget.wav'
pygame.time.delay(1000)#等待1秒让mixer完成初始化
soundwav=pygame.mixer.Sound(starsound) 

#加载gameover的音效
#gameoversound='music/gameover.wav'
#soundoverwav=pygame.mixer.Sound(gameoversound) 

#窗口
xmax = 640
ymax = 480
screen = pygame.display.set_mode((xmax,ymax))

#计时器
clock = pygame.time.Clock()

#加载图片
background = pygame.image.load('image/background.bmp').convert()
gameover = pygame.image.load('image/game_over.bmp').convert()
mouse_cursor=pygame.image.load('image/plane.bmp').convert()
mouse_cursor.set_colorkey((255,255,255))#去掉背景色

#球的出发点
ball_begin = pygame.image.load('image/ball_begin.bmp').convert()
ball_begin.set_colorkey((255,255,255))
ball_begin.set_alpha(100)#设置透明度
#获取坐标
xball_begin = ball_begin.get_height()
yball_begin = ball_begin.get_width()

#第一个球 黄球
ball1 = pygame.image.load('image/ball1.bmp').convert()
ball1.set_colorkey((255,255,255))
xball1 = ball1.get_height()
yball1 = ball1.get_width()
#第二个球 绿球
ball2 = pygame.image.load('image/ball2.bmp').convert()
ball2.set_colorkey((255,255,255))
xball2 = ball2.get_height()
yball2 = ball2.get_width()
#第三个球 橙球
ball3 = pygame.image.load('image/ball3.bmp').convert()
ball3.set_colorkey((255,255,255))
xball3 = ball3.get_height()
yball3 = ball3.get_width()
#第四个球 蓝球
ball4 = pygame.image.load('image/ball4.bmp').convert()
ball4.set_colorkey((255,255,255))
xball4 = ball4.get_height()
yball4 = ball4.get_width()
#第五个球 星星
ball5 = pygame.image.load('image/ball5.bmp').convert()
ball5.set_colorkey((255,255,255))
xball5 = ball5.get_height()
yball5 = ball5.get_width()

#设置球的列表ball_list [ x, y, step_x, step_y, height, width, surface, rect]

ball1_list = [100,200,1,1, xball1, yball1, ball1, pygame.Rect((100,200),(xball1,yball1))]
ball2_list = [100,200,1,1, xball2, yball2, ball2, pygame.Rect((100,200),(xball2,yball2))]
ball3_list = [100,200,1,1, xball3, yball3, ball3, pygame.Rect((100,200),(xball3,yball3))]
ball4_list = [100,200,1,1, xball4, yball4, ball4, pygame.Rect((100,200),(xball4,yball4))]
ball5_list = [100,200,-1,-1, xball5, yball5, ball5, pygame.Rect((100,200),(xball5,yball5))]

#设置球的顺序（字典）
balls_dict = {'1':ball1_list, '2':ball2_list, '3':ball3_list, '4':ball4_list, 'star ball':ball5_list}
#设置球的衔接
sprites_dict = {'ball1':ball4_list, 'star ball': ball5_list}

#初始化游戏数据
n = 1#初始球 
n_ball = 1 #当前球
tp_ball = 1 #下一个球
level = 1 #级别
t = 2000 #计时间隔
seconds = 0 #时间
scores = 0 #分数
catch_star_ball = 0 #是否碰到星星

#游戏逻辑
#状态量转为字符以便应用字典
def one_num_to_str(n):
    if n == 0:
        return '0'
    elif n == 1:
        return '1'
    elif n == 2:
        return '2'
    elif n == 3:
        return '3'
    elif n == 4:
        return '4'
    elif n == 5:
        return '5'
    elif n == 6:
        return '6'
    elif n == 7:
        return '7'
    elif n == 8:
        return '8'
    elif n == 9:
        return '9'

#def函数定义

#整型转换为字符
def int_to_str(n):
    a = one_num_to_str(n/1000) + one_num_to_str((n/100)%10) + one_num_to_str((n/10)%10) + one_num_to_str(n%10)
    return a

#转换球
def change_ball(tp_ball, n_ball, n, special_mode = 0):#判断是否是星星球
    a = 1
    b = 1    
    if special_mode:
        key = 'star ball'
        key2 = 'star ball'
        a = -a
        if (seconds % 2):
            b = -b
    else:
        key = one_num_to_str(tp_ball)
        key2 = 'ball' + int_to_str(n_ball)

#设置二维列表存储八个方向
    sprites_dict[key2] = [0,0,0,0,0,0,0,0]
    sprites_dict[key2][0] = (100 + n)
    sprites_dict[key2][1] = (200 + n/3)
    sprites_dict[key2][2] = balls_dict[key][2] * a
    sprites_dict[key2][3] = balls_dict[key][3] * b
    sprites_dict[key2][4] = balls_dict[key][4]
    sprites_dict[key2][5] = balls_dict[key][5]
    sprites_dict[key2][6] = balls_dict[key][6]
    sprites_dict[key2][7] = pygame.Rect((100,200),(balls_dict[key][4],balls_dict[key][5]))
    tp_ball += 1
    if tp_ball == 5:
        tp_ball = 1
    return tp_ball

#游戏结束
def game_over():
    #soundoverwav.play(1)
    size = 48
    font = pygame.font.Font(None, size)
    text = font.render('Oops! GAME OVER!', 1, (240,128,128))
    text_pos = text.get_rect(centerx = screen.get_width()/2, centery = (screen.get_height()/2 - size))
    gameover.set_alpha(180)#设置透明度
    screen.blit(gameover,(0,0))
    screen.blit(text, text_pos)
    show_level(level)
    show_time(seconds)
    show_scores(scores)
    pygame.display.flip()
    h = 1
    while h:
       for e in pygame.event.get():
           if e.type == QUIT:
               exit()
           if e.type == MOUSEBUTTONDOWN:#重新开始游戏
               h = 0

#打印级别
def show_level(level):
    size = 36
    font = pygame.font.Font(None, size)
    text = font.render('level: ' + int_to_str(level), 1, (135,206,250))
    screen.blit(text, ((xmax-165), 8))

#打印时间
def show_time(seconds):
    size = 36
    font = pygame.font.Font(None, size)
    text = font.render('time: ' + int_to_str(seconds), 1, (135,206,250))
    screen.blit(text, ((xmax-165), 40))

#打印分数
def show_scores(scores):
    size = 36
    font = pygame.font.Font(None, size)
    text = font.render('scores: ' + int_to_str(scores), 1, (135,206,250))
    screen.blit(text, ((xmax-165),72))

#建立计时器
pygame.time.set_timer(USEREVENT, t) 

pygame.time.set_timer(USEREVENT+1, 1000) 

while 1:
    #clock.tick(100)
    
    screen.blit(background, (0,0))
#获得鼠标的xy坐标
    xy = pygame.mouse.get_pos()
    #mousex=pygame.mouse.get_pos()
    #mousey=pygame.mouse.get_pos()
#隐藏鼠标
    pygame.mouse.set_visible(False)
#获取光标图片尺寸
    #mousex -= mouse_cursor.get_width() / 2
    #mousey -= mouse_cursor.get_height() / 2
#图片光标代替鼠标
    screen.blit(mouse_cursor, xy)
    
    for key in sprites_dict:#设置方向转换

        sprite = sprites_dict[key]

        if (sprite[0] + sprite[4])>xmax or sprite[0]<0:
            sprite[2] = -sprite[2]
        if (sprite[1] + sprite[5])>ymax or sprite[1]<0:
            sprite[3] = -sprite[3]

        sprite[0] += (sprite[2])
        sprite[1] += (sprite[3])

        screen.blit(sprite[6], (sprite[0],sprite[1]))
    
        sprite[7] = pygame.Rect((sprite[0], sprite[1]),(sprite[4],sprite[5]))

        if sprite[7].collidepoint(xy):
            if key == 'star ball':#得到星星球加五分
                scores += 5
                catch_star_ball = 1
                soundwav.play()
            else:
                game_over()
                level = 1
                n_ball = 1
                seconds = 0
                scores = 0
                sprites_dict = {'ball1':ball1_list, 'star ball': ball5_list}
                pygame.time.set_timer(USEREVENT, t)
                break

    if catch_star_ball:
        del sprites_dict['star ball']
        catch_star_ball = 0
        change_ball(0, 0, n, 1)

    for e in pygame.event.get():
        if e.type == QUIT:
            exit()
        if e.type == USEREVENT:
            n_ball += 1
            level += 1
            n += 10
            if n > (xmax - 100 - xball_begin) :
                n = 0
            pygame.time.set_timer(USEREVENT, t)
            tp_ball = change_ball(tp_ball, n_ball, n)
        if e.type == USEREVENT+1:
            scores += 1
            seconds += 1
            
    show_level(level)

    show_time(seconds)

    show_scores(scores)

    screen.blit(ball_begin, ((10+ n),(50 + n)))

    pygame.display.flip()

      
