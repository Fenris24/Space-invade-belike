#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

void def_fun();
void colors();

WINDOW *new_menu(int *select);
WINDOW *new_title();
WINDOW *new_play(int *select);
WINDOW *new_quit(int *select);
void clear_w(WINDOW *win, int h, int l);

WINDOW *start_game();
void _ship(WINDOW *win, int x, int y, int ok);
void _ufo(WINDOW *win, int x, int y, int ok);
void _laser(WINDOW *win, int x, int y, int ok);
int main()
{
		int Mx, My, input, *select, s;
		WINDOW *menu, *game;
		getmaxyx(stdscr, Mx, My);
		select = &s;
		*select = 0;

		initscr();
		def_fun();
		colors();
		menu = new_menu(select);
		while((input = getch()) != KEY_F(4))
		{
				if(input == KEY_DOWN && *select < 1)
						*select += 1;
				if(input == KEY_UP && *select > 0)
						*select -= 1;
				if(input == 10 && *select == 0)
						*select = 3;
				if(input == 10 && *select == 1)
						*select = 4;
				menu = new_menu(select);
				if(*select == 3)
				{
						game = start_game();
				}
				if(*select == 4)
						break;
				halfdelay(1);
		}
		refresh();
		endwin();
}

void def_fun()
{
		curs_set(0);
		cbreak();
		noecho();
		nodelay(stdscr, TRUE);
		keypad(stdscr, TRUE);
}
void colors()
{
		if(has_colors() == FALSE)
		{	
				endwin();
				printf("Your terminal does not support color\n");
				exit(1);
		}	
		start_color();
		init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(2, COLOR_RED, COLOR_BLACK);
		init_pair(3, COLOR_YELLOW, COLOR_BLACK);
		init_pair(4, COLOR_GREEN, COLOR_BLACK);
		init_pair(5, COLOR_CYAN, COLOR_BLACK);
		init_pair(6, COLOR_BLUE, COLOR_BLACK);
		init_pair(7, COLOR_WHITE, COLOR_BLACK);
		init_pair(8, COLOR_MAGENTA, COLOR_WHITE);
		init_pair(9, COLOR_RED, COLOR_WHITE);
		init_pair(10, COLOR_YELLOW, COLOR_WHITE);
		init_pair(11, COLOR_GREEN, COLOR_WHITE);
		init_pair(12, COLOR_CYAN, COLOR_WHITE);
		init_pair(13, COLOR_BLUE, COLOR_WHITE);
		init_pair(14, COLOR_BLACK, COLOR_WHITE);
	//const int aura_s = 200, aura_f = 300;
		//static int i = aura_s, p = 10;
		//init_color(COLOR_BLACK, i, i, i);
		//init_pair(7, COLOR_WHITE, COLOR_BLACK);
		//i += p;
		//if(i == aura_s || i == aura_f)
		//		p *= -1;
}
WINDOW *new_menu(int *select)
{
		int Mx, My;
		getmaxyx(stdscr, Mx, My);
		WINDOW *win = newwin(Mx, My, 0, 0);
		WINDOW *title, *play, *quit;
		title = new_title();
		play = new_play(select);
		quit = new_quit(select);
		if(*select == 3)
		{
				clear_w(title, 7, 103);
				clear_w(play, 3, 21);
				clear_w(quit, 3, 17);
				wrefresh(win);
				delwin(win);
		}
		return win;
}
WINDOW *new_title()
{
		WINDOW *win = newwin(7, 103, 5, 16);
		char title[7][103] = {
		" ######  #######     #     ######     #####   ######    #####         #  #######    ####   #######    ",
		"#        #  #  #    # #    #     #    #    #  #     #  #     #        #  #         #    #  #  #  #    ",
		"##          #       # #    #    #     #    #  #    #   #     #        #  #        #           #       ",
		" #####      #      #   #   #   #      #    #  #   #    #     #        #  # ####   #           #       ",
		"     ##     #      # # #    ###       #####    ###     #     #        #  #        #           #       ",
		"      #     #     #  #  #  #   #      #       #   #    #     #  #     #  #         #    #     #       ",
		"######      #     #     #  #    ##    #       #    ##   #####    #####   #######    ####      #       "};
		int i, j, k, x;
		static int c = 1;
		for(k = 0; k < 11; ++k)
		{
				if(k < 4)
						x = k * 9;
				else
						x = k * 9 + 1;
				for(i = 0; i < 7; ++i)
				{
						wattron(win, COLOR_PAIR(c));
						for(j = x; j < x + 8; ++j)
						{
								mvwaddch(win, i, j, title[i][j]);
						}
						wattroff(win, COLOR_PAIR(c));
				}
				c ++;
				if(c == 8)
						c = 1;
		}
		wrefresh(win);
		return win;
}
WINDOW *new_play(int *select)
{
		WINDOW *win;
		win = newwin(3, 21, 20, 55);
		char play[3][21] = {
		"  ## ###  #  ##  ### ",
		"  #   #  # # # #  #  ",
		" ##   #  # # # #  #  "};
		int i, j;
		if(*select == 0)
				wattron(win, A_BLINK);
		for(i = 0; i < 3; ++i)
				for(j = 0; j < 21; ++j)
						mvwaddch(win, i, j, play[i][j]);
		wattron(win, A_BLINK);
		wrefresh(win);
		return win;
}
WINDOW *new_quit(int *select)
{
		WINDOW *win;
		win = newwin(3, 17, 27, 57);
		char quit[3][17] = {
		"  #  # #  #  ### ",
		" # # # #  #   #  ",
		"  ## ###  #   #  "};
		int i, j;
		if(*select == 1)
				wattron(win, A_BLINK);
		for(i = 0; i < 3; ++i)
				for(j = 0; j < 17; ++j)
						mvwaddch(win, i, j, quit[i][j]);
		wattroff(win, A_BLINK);
		wrefresh(win);
		return win;
}
void clear_w(WINDOW *win, int h, int l)
{
		int i, j;
		for(i = 0; i < h; ++i)
				for(j = 0; j < l; ++j)
						mvwaddch(win, i, j, ' ');
		wrefresh(win);
}
WINDOW *start_game()
{
		int input, Mx, My, nr_ufo = 9;
	    _Bool d_ufo[10];
		getmaxyx(stdscr, Mx, My);
		int x = Mx - 6, y = My / 2;

		WINDOW *game;
		game = newwin(Mx, My, 0, 0);
		_ship(game, x, y, 1);
		wrefresh(game);

		int i, j = 0, d = 12, p = 3, np = 12;
		for(i = 1; i <= nr_ufo; ++i)
		{
				_ufo(game, 1, (i * d) + j, 1);
		}
		wrefresh(game);
		while((input = getch()) && nr_ufo != 0)
		{
				if(input == KEY_LEFT && y > 1)
				{
						_ship(game, x, y, 0);
						_ship(game, x, y -= 2, 1);
				}
				if(input == KEY_RIGHT && y < My - 7)
				{
						_ship(game, x, y, 0);
						_ship(game, x, y += 2, 1);
				}
				if(input == ' ')
				{
						int k, lx = x - 2, ly = y + 2, dp = 1;
						while(mvwinch(game, lx, ly) == ' ')
								lx --;
						while(ly == 1)
						{
								if(mvwinch(game, lx, ly) == ' ' && mvwinch(game, lx, --ly) != ' ')
										dp ++;
						}
						d_ufo[dp] = 1;
						for(k = 0; k < 2; ++k)
						{
								i = x - 2;
								while(i > 0)
								{
										_laser(game, i, y + 2, 1);
										napms(5);
										_laser(game, i, y + 2, 0);
										i -= 2;
								}
						}
				}
				napms(50);
				for(i = 1; i < nr_ufo; ++i)
				{
						if(d_ufo == 0)
								_ufo(game, 1, (i * d) + j, 0);
				}
				j += p;
				for(i = 1; i < nr_ufo; ++i)
				{
						if(d_ufo == 0)
								_ufo(game, 1, (i * d) + j, 1);
				}
				if(j == -3 || j == np)
						p *= -1;
				napms(50);
		}
		return game;
}
void _ship(WINDOW *win, int x, int y, int ok)
{
		char ship1[6][6] = {
		" +  + ",
		" |  | ",
		" ]##[ ",
		"<####>",
		" ]##[ ",
		"  ^^  "};
		//char ship2[][];
		int i, j;
		wattron(win, COLOR_PAIR(5));
		if(ok == 1)
		{
				for(i = 0; i < 6; ++i)
						for(j = 0; j < 6; ++j)
								mvwaddch(win, x + i, y + j, ship1[i][j]);
		}
		else
		{
				for(i = 0; i < 6; ++i)
						for(j = 0; j < 6; ++j)
								mvwaddch(win, x + i, y + j, ' ');
		}
		wattroff(win, COLOR_PAIR(5));
		wrefresh(win);
}
void _ufo(WINDOW *win, int x, int y, int ok)
{
		char ufo[3][5] = {
		" _@_ ",
		"<###>",
		" \" \" "};
		int i, j;
		wattron(win, COLOR_PAIR(4));
		if(ok == 1)
		{
				for(i = 0; i < 3; ++i)
						for(j = 0; j < 5; ++j)
								mvwaddch(win, x + i, y + j, ufo[i][j]);
		}
		else
		{
				for(i = 0; i < 3; ++i)
						for(j = 0; j < 5; ++j)
								mvwaddch(win, x + i, y + j, ' ');
		}
		wattroff(win, COLOR_PAIR(4));
		wrefresh(win);
}
void _laser(WINDOW *win, int x, int y, int ok)
{
		char laser[2][1] = {
		"<",
		">"};
		int i, j;
		wattron(win, COLOR_PAIR(3));
		if(ok == 1)
				for(i = 0; i < 2; ++i)
						for(j = 0; j < 1; ++j)
								mvwaddch(win, x + i, y + i, laser[i][j]);
		else
				for(i = 0; i < 2; ++i)
						for(j = 0; j < 1; ++j)
								mvwaddch(win, x + i, y + i, ' ');
		wattroff(win, COLOR_PAIR(3));
		wrefresh(win);
}
