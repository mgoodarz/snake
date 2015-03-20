/*
  Snake Game
  by : Morteza Goodarz
*/

#include <graphics.h>
#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <dos.h>

const	int	esc			= 27,
			space		= 32,
			left		= 75,
			right		= 77,
			up			= 72,
			down		= 80,
			step		= 20;
			
int			interval	= 150;

class part
{
public:

	int	x,
		y,
		dir;

	void move(int dir, int w, int h)
	{
		w++;
		h++;
		switch (dir)
		{
			case left:
				x -= step;
				x += w;
				x %= w;
				break;
			case right:
				x += step;
				x %= w;
				break;
			case up:
				y -= step;
				y += h;
				y %= h;
				break;
			case down:
				y += step;
				y %= h;
				break;
		}
	}
	
	void draw()
	{
		rectangle(x + 1, y + 1, x + step - 1, y + step - 1);
		//circle(x + step / 2, y + step / 2, step / 2 + 1);

	}
};

void sound_feed()
{
	for (int i = 20; i < 35; i++)
	{
		sound(i * i);
		delay(5);
	}
	nosound();
}

void sound_lost()
{
	sound(50);
	delay(400);
	nosound();
}

void draw_border(int w, int h)
{
	//setviewport(0, 0, w, h+1, 1);
	setcolor(LIGHTRED);
	line(0, 0, w, 0);
	line(0, 0, 0, h + 20);
	line(w, h + 20, w, 0);
	line(w, h + 20, 0, h + 20);
	line(0, h + 1, w, h + 1);
}

void draw_food(int x, int y, int food_color)
{
	setcolor(food_color);
	setfillstyle(1, food_color);
	//circle(x + step / 2, y + step / 2, step / 2 - 1);
	rectangle(x + 1, y + 1, x + step - 1, y + step - 1);
	floodfill(x + step / 2, y + step / 2, food_color);
}

void draw_snake(int n, part *snake)
{
	char s[2];

	setcolor(YELLOW);
	for (int i = 0; i < n; i++)
		snake[i].draw();

	//draws arrow
	switch (snake[0].dir)
	{
		case up:
			s[0] = 24;
			break;
		case down:
			s[0] = 25;
			break;
		case right:
			s[0] = 26;
			break;
		case left:
			s[0] = 27;
			break;
	}

	s[1] = NULL;
	outtextxy(snake[0].x + step / 2, snake[0].y + step / 2 + 1, s);
}

void draw_score(int score, int w, int h)
{
	char	t[14] = "Score : ",
			u[6];
			
	//writes score
	itoa(score, u, 10);
	strcat(t, u);
	//setviewport(0, 0, w, h+1, 0);
	outtextxy(w / 2, h + 11, t);

	//writes option
	outtextxy(60, h + 11, "Pause : Space");
	outtextxy(w - 50, h + 11, "Quit : Esc");
}

void draw(int n, int score, int x, int y, int w, int h, int food_color, part *snake)
{
	cleardevice();

	//draws border
	draw_border(w, h);

	//draws food
	draw_food(x, y, food_color);

	//draws snake
	draw_snake(n, snake);

	//writes score
	draw_score(score, w, h);
}

void move(int n, int cur_dir, part *snake, int w, int h)
{
	for (int i = n - 1; i > 0; i--)
		snake[i].dir = snake[i - 1].dir;

	if ((snake[0].dir == right && cur_dir != left) || (snake[0].dir == left && cur_dir != right) || (snake[0].dir == up && cur_dir != down) || (snake[0].dir == down && cur_dir != up))
		snake[0].dir = cur_dir;

	for (int j = 0; j < n; j++)
		snake[j].move(snake[j].dir, w, h);
}

void feed(int n, int &x, int &y, int &food_color, int &f, int w, int h, part *snake)
{
	while (f)
	{
		time_t t;

		f = 0;
		srand((unsigned) time(&t));
		x = step * (rand() % (w / step));
		y = step * (rand() % (h / step));
		//randomize();
		//x=step * random(w / step);
		//y=step * random(h / step);
		food_color = random(7) + 9;

		for (int j = 0; j < n; j++)
			if ((snake[j].x == x) && (snake[j].y == y))
				f = 1;
	}
}

void grow(int &n, int &score, int &f, part *snake)
{
	sound_feed();

	snake[n].dir = snake[n - 1].dir;

	switch (snake[n].dir)
	{
		case left:
			snake[n].x = snake[n - 1].x + step;
			snake[n].y = snake[n - 1].y;
			break;
		case right:
			snake[n].x = snake[n - 1].x - step;
			snake[n].y = snake[n - 1].y;
			break;
		case up:
			snake[n].x = snake[n - 1].x;
			snake[n].y = snake[n - 1].y + step;
			break;
		case down:
			snake[n].x = snake[n - 1].x;
			snake[n].y = snake[n - 1].y - step;
			break;
	}

	score++;
	n++;
	interval -= 1;
	f = 1;
}

void check(int &n, int &score, int x, int y, int &f, /*int w, int h,*/ int &win, part *snake)
{
	//if ((snake[0].x < 0) || (snake[0].x > w) || (snake[0].y < 0) || (snake[0].y > h)) win = 0;

	for (int k = 1; k < n; k++)
		if ((snake[0].x == snake[k].x) && (snake[0].y == snake[k].y))
			win = 0;

	if ((snake[0].x == x) && (snake[0].y == y))
		grow(n, score, f, snake);
}

void initialize(int n, int score, int x, int y, int w, int h, int food_color, part *snake)
{
	for (int i = 0; i < n; i++)
	{
		snake[i].x = (((w + 1) / step + n) / 2 - (i + 1)) * step;
		snake[i].y = (((h + 1) / step) / 2) * step;
		snake[i].dir = right;
	}

	settextjustify(CENTER_TEXT, CENTER_TEXT);
	draw(n, score, x, y, w, h, food_color, snake);

	char	s_ldr[6],
			s_u[2];

	outtextxy(w / 2, (h / 2) - 20, "Snake");
	outtextxy(w / 2, (h / 2) + 40, "by : Morteza Goodarz");
	outtextxy(w / 2, h - 30, "Press       to start...");
	s_ldr[0] = 27;
	s_ldr[1] = ' ';
	s_ldr[2] = 25;
	s_ldr[3] = ' ';
	s_ldr[4] = 26;
	s_ldr[5] = NULL;
	outtextxy((w - 184) / 2 + 68, h - 30, s_ldr);
	s_u[0] = 24;
	s_u[1] = NULL;
	outtextxy((w - 184) / 2 + 68, h - 42, s_u);
}

void main()
{
	int gdriver = DETECT, gmode, errorcode;

	initgraph(&gdriver, &gmode, "");
	errorcode = graphresult();
	if (errorcode != grOk)
	{
		cout << "Graphics error: " << grapherrormsg(errorcode) << "\n";
		cout << "Press any key to halt:";
		getch();
		exit(1);
	}

	const	int	h			= getmaxy() - 20,
				w			= getmaxx();

	int 		x			= w,
				y			= h,
				score		= 0,
				food_color	= 0,
				win			= 1,
				pause		= 0,
				quit		= 0,
				n			= 4,
				f			= 1,
				cur_dir,
				dir;

	part		*snake		= new part[n];

	//initializes snake
	initialize(n, score, x, y, w, h, food_color, snake);

	while (cur_dir = getch()) /* do nothing */;

	while (win)
	{
		while ((win) && ((!kbhit()) || (dir = getch())))
		{
			if (dir == esc) quit = 1;
			if (quit) break;

			if (dir == space)
			{
				if (pause)
				{
					pause = 0;
					dir = cur_dir;
				}
				else
				{
					pause = 1;
					dir = cur_dir;

					outtextxy(w / 2, (h / 2) + 25, "Pause !");
					outtextxy(w / 2, h - 50, "Press Space to continue...");
				}
			}
			if (pause == 1) break;

			if (dir == right || dir == left || dir == up || dir == down)
				cur_dir = dir;

			move(n, cur_dir, snake, w, h);
			feed(n, x, y, food_color, f, w, h, snake);
			check(n, score, x, y, f, /*w, h,*/ win, snake);
			draw(n, score, x, y, w, h, food_color, snake);
			delay(interval);
		}

		if (quit) break;
	}

	if (quit) outtextxy(w / 2, (h / 2) - 20, "Quit !");
	else outtextxy(w / 2, (h / 2) - 20, "Game Over !");

	outtextxy(w / 2, h - 30, "Press any key to exit...");

	sound_lost();

	getch();
}
