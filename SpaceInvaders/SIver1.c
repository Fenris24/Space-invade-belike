#include <stdio.h>
#include <ncurses.h>
#include <time.h>

#define H 5
#define W 5

typedef struct _AXIS
{
		int x, y;
		WINDOW *ufo;
} AXIS;

void def_fun();
WINDOW *new_exp(int x, int y);
void del_exp(WINDOW *local_win);
WINDOW *new_ufo(int x, int y);
void move_ufo(AXIS *ufo, int nr, int px, int py);
WINDOW *new_ship(int x, int y);
void del_obj(WINDOW *local_win);
WINDOW *new_laser(int x, int y);
void del_laser(WINDOW *local_win);

int main()
{
		initscr();
		def_fun();

		int Mx, My, win = 0;
		getmaxyx(stdscr, Mx, My);
		AXIS ship_c, ufo_c[10];
		int input;
		double sp = 0.1;
		clock_t t1 = clock(), t2;

		border('#', '#', '#', '#', '#', '#', '#', '#');
		printw("|%d|%d|", Mx, My);
		refresh();

		int px = 0, py = 14, m = 3, ufo_nr = 9;
		ufo_c[0].x = 2;
		ufo_c[0].y = 2;
		move_ufo(ufo_c, ufo_nr, px, py);
		for(int i = 0; i < ufo_nr; ++i)
		{
				ufo_c[i].ufo = new_ufo(ufo_c[i].x, ufo_c[i].y);
				refresh();
		}

		ship_c.x = Mx - H - 1;
		ship_c.y = My / 2;
		WINDOW *ship;
		ship = new_ship(ship_c.x, ship_c.y);
		refresh();

		WINDOW *laser;
		int ufo_ok[10], hit[130];
		for(int i = 0; i < ufo_nr; ++i)
				ufo_ok[i] = 1;
		for(int i = 1; i < 130; ++i)
				hit[i] = 0;
		WINDOW *exp;
		while((input = getch()) != KEY_F(4) &&  ufo_c[0].x != ship_c.x && win != 180)
		{
				t2 = clock();
				clock_t dif = (t2 - t1) / CLOCKS_PER_SEC;
				if(dif > sp)
				{
						t1 = t2;
						ufo_c[0].y += m;
						if(ufo_c[0].y == (m * 3) + 2 || ufo_c[0].y == 2)
						{
								ufo_c[0].x ++;
								m *= -1; 
						}
						move_ufo(ufo_c, ufo_nr, px, py);
						for(int i = 1; i < 130; ++i)
								hit[i] = 0;
						for(int i = 0; i < ufo_nr; ++i)
						{
								for(int j = ufo_c[i].y; j < ufo_c[i].y + 5; ++j)
										if(hit[j] == 0)
												hit[j] = 1;
										else
												hit[j] = 0;
								if(ufo_ok[i] == 1)
								{
										del_obj(ufo_c[i].ufo);
										ufo_c[i].ufo = new_ufo(ufo_c[i].x, ufo_c[i].y);
								}
						}
				}
				int spd = 2, i;
				switch(input)
				{
						case KEY_RIGHT:
								if(ship_c.y + spd < My - 5)
								{
										del_obj(ship);
										ship = new_ship(ship_c.x, ship_c.y += spd);
								}
								break;
						case KEY_LEFT:
								if(ship_c.y - spd > 0)
								{
										del_obj(ship);
										ship = new_ship(ship_c.x, ship_c.y -= spd);
								}
								break;
						case ' ':
								for(i = 2; i < Mx - 5; i += 2)
								{
										int k = -1;
										for(int j = 0; j < 100; ++j)
										{
										laser = new_laser(ship_c.x - i, ship_c.y + 2);
										del_laser(laser);
										laser = new_laser(ship_c.x - i, ship_c.y + 2);
										del_laser(laser);
										}
										//for(int l = 1; l < 130; ++l)
										//		mvprintw(1, l, "%d", hit[l]);
										if(hit[ship_c.y + 2] == 1)
										{
												for(int j = 1; j <= ship_c.y + 2; ++j)
														if(hit[j] == 1 && hit[j + 1] == 1 && hit[j + 2] == 1 && hit[j + 3] == 1 && hit[j + 4] == 1)
																k ++;
												if(k != -1)
												{
														win ++;
														mvprintw(2, 2, "%d", win);
														//mvprintw(0, 0,"%d", k);
														ufo_ok[k] = 0;
														del_obj(ufo_c[k].ufo);
														for(int j = 0; j < 200; ++j)
														{
																exp = new_exp(ufo_c[k].x + 1, ufo_c[k].y + 1);
																del_exp(exp);
														}
												}
										}
								}
								break;

				}	
		}
		refresh();
		getch();
		endwin();
		return 0;
}

void def_fun()
{
		curs_set(0);
		cbreak();
		noecho();
		nodelay(stdscr, TRUE);
		keypad(stdscr, TRUE);
}

WINDOW *new_exp(int x, int y)
{
		WINDOW *local_win = newwin(3, 3, x, y);
		wprintw(local_win, "\\|/");
		wprintw(local_win, "-*-");
		wprintw(local_win, "/|\\");
		wrefresh(local_win);
		return local_win;
}
void del_exp(WINDOW *local_win)
{
		int i, j;
		for(i = 0; i < 3; ++i)
				for(j = 0; j < 3; ++j)
						mvwprintw(local_win, i, j, " ");
		wrefresh(local_win);
		delwin(local_win);
}
WINDOW *new_ufo(int x, int y)
{
		WINDOW *local_win = newwin(H, W, x, y);
		wprintw(local_win, "     ");
		wprintw(local_win, "__ __");
		wprintw(local_win, "[#@#]");
		wprintw(local_win, " <#> ");
		wprintw(local_win, "     ");
		wrefresh(local_win);
		return local_win;
}
void move_ufo(AXIS *ufo, int nr, int px, int py)
{
		int i;
		for(i = 1; i < nr; ++i)
		{
				ufo[i].x = ufo[i - 1].x + px;
				ufo[i].y = ufo[i - 1].y + py;
		}
}
WINDOW *new_ship(int x, int y)
{
		WINDOW *local_win = newwin(H, W, x, y);
		wprintw(local_win, "  +  ");
		wprintw(local_win, "  |  ");
		wprintw(local_win, " ]#[ ");
		wprintw(local_win, "<###>");  
		wprintw(local_win, " ^ ^ ");
		wrefresh(local_win);
		return local_win;
}
WINDOW *new_laser(int x, int y)
{
		WINDOW *local_win = newwin(2, 1, x, y);
		wprintw(local_win, "<");
		wprintw(local_win, ">");
		wrefresh(local_win);
		return local_win;
}
void del_laser(WINDOW *local_win)
{
		int i, j;
		for(i = 0; i < 2; ++i)
				for(j = 0; j < 1; ++j)
						mvwprintw(local_win, i, j, " ");
		wrefresh(local_win);
		delwin(local_win);
}
void del_obj(WINDOW *local_win)
{
		int i, j;
		for(i = 0; i < H; ++i)
				for(j = 0; j < W; ++j)
						mvwprintw(local_win, i, j, " ");
		wrefresh(local_win);
		delwin(local_win);
}
