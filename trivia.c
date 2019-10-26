#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <menu.h>
#include <time.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4
WINDOW *GameWindow, *TabWindow;
MENU *my_menu,*game_menu;
WINDOW *my_menu_win;
ITEM **my_items, **my_answers;
FILE *DATA;
int Scor = 0,Highscor,skips = 2, fiftys = 2, GameX,GameY,nr_corect=0,nr_gresit=0;
char Scor_curent[50];
int active_game = 0;
char QandA[6][50];
char *choices[] = {
	"New Game",
	"Resume Game",
	"Quit",
	"",
	(char *)NULL,
};
char *TestQ[] = {"Esti gata sa incepi?", "A)DA", "B)NU", "C)NU", "D)NU", (char *)NULL, };

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
int NewGameWin();
int GameWin();
int NextQuestion(int skip);
void ReadQ();
void RefreshQ();
int Tabs();
void FinalQ();
int updateTime();
void cutIcon();
void answers2();
void cleanUp();

void title()
{
	//Print one of the title variants
	char line[200];
	int tit;
	FILE *logo1;
	srand(time(NULL));
	tit = rand();
	tit= tit %4;
	switch(tit)
	{
		case 0:
		logo1 = fopen("Grafica/logo1.txt", "r");
		while(fgets(line, 200, logo1) != NULL)
		{
			printw("%s",line);
		}
		break;
		case 1:
		logo1 = fopen("Grafica/logo2.txt", "r");
		while(fgets(line, 200, logo1) != NULL)
		{
			printw("%s",line);
		}
		break;
		case 2:
		logo1 = fopen("Grafica/logo3.txt", "r");
		while(fgets(line, 200, logo1) != NULL)
		{
			printw("%s",line);
		}
		break;
		case 3:
		logo1 = fopen("Grafica/logo4.txt", "r");
		while(fgets(line, 200, logo1) != NULL)
		{
			printw("%s",line);
		}
		break;
	}
}

int menuSelect()
{
	wrefresh(my_menu_win);
	int c;
	while(c = wgetch(my_menu_win))
		{
			switch(c)
			{	
				//moving in the main menu
				case KEY_DOWN:
					updateTime();
					menu_driver(my_menu, REQ_DOWN_ITEM);
					break;
				case KEY_UP:
					updateTime();
					menu_driver(my_menu, REQ_UP_ITEM);
				break;
					case 10:
					updateTime();
					//Quit
					if(current_item(my_menu) == my_items[2])
					{
						return -1;
					}
					else
					{
						//New Game
						if(current_item(my_menu) == my_items[0])
						{
							if(NewGameWin())
							{
								return -1;
							}
						}
						else
							//Resume Game
							if(active_game)
								if(GameWin())
									return -1;
					}
					break;
			}
			wrefresh(my_menu_win);
		}
	return 0;
}

int NewGameWin()
{
	//initializes the game window and then switches to it
	skips = 2;
	fiftys = 2;
	fseek( DATA, 0, SEEK_SET );
	char c;
	fscanf(DATA, "%d", &Highscor);
	fscanf(DATA, "%c", &c);
	ReadQ();
	nr_corect = 0;
	nr_gresit = 0;
	GameWindow = newwin(GameY -22, GameX -22, 12, 22);
	wrefresh(GameWindow);
	Scor = 0;
	box(GameWindow, 0, 0);
	active_game = 1;
	RefreshQ();
	keypad(GameWindow, TRUE);
	Tabs();
	if(GameWin())
		return 1;
	return 0;
}

int GameWin()
{
	int c;
	int copy_cor,copy_gre;
	if(current_item(my_menu) != my_items[1])
		menu_driver(game_menu, REQ_DOWN_ITEM);
	wrefresh(GameWindow);
	while(c = wgetch(GameWindow))
	{
		switch(c)
		{	
			//moving between response variants
			case KEY_DOWN:
			updateTime();
			menu_driver(game_menu, REQ_DOWN_ITEM);
			break;
			case KEY_UP:
			updateTime();
			if(current_item(game_menu) != my_answers[1])
				menu_driver(game_menu, REQ_UP_ITEM);
			break;
			case 'q':
			//returning to the menu
			updateTime();
			return 0;
			break;
			case 10:
			updateTime();
			if(NextQuestion(0) == 0)
			{
				active_game = 0;
				wclear(GameWindow);
				wrefresh(GameWindow);
				return 1;
			}
			break;
			case 's':
			//to skip the question
			updateTime();
			skips--;
			if(skips == 1)
			{
				cutIcon();		
				if(NextQuestion(skips) == 0)
				{
					active_game = 0;
					wclear(GameWindow);
					wrefresh(GameWindow);
					return 1;
				}
			}
			break;
			case 'f':
			//destroy half the answers
			copy_cor = nr_corect;
			copy_gre = nr_gresit;
			updateTime();
			if(fiftys == 2)
				answers2();
			fiftys--;
			if(fiftys == 1)
				cutIcon();
			nr_gresit = copy_gre;
			nr_corect = copy_cor;
			break;
		}
		wrefresh(GameWindow);
	}
	return 0;
}

void answers2()
{
	//choose two of the wrong variants
	int corect,i,corect2;
	fscanf(DATA, "%d",&corect);
	fseek(DATA, -1, SEEK_CUR);
	srand(time(NULL));
	corect2 = rand()%4 + 1;
	while(corect2 == corect)
	{
		corect2 = rand()%4 + 1;		
	}
	//to be cut by half
	for(i=1; i <= 4; i++)
	{
		if((i != corect2) && (i != corect))
		{
			sprintf(QandA[i],"-)-----------");
		}
	}

	wrefresh(GameWindow);
	RefreshQ();
	menu_driver(game_menu, REQ_DOWN_ITEM);
}

int NextQuestion(int skip)
{
	int corect,copy_Scor,copy_cor,copy_gre;
	char buffer[100];
	fscanf(DATA, "%d", &corect);
	fscanf(DATA,"%c",&buffer[0]);
	wrefresh(GameWindow);
	copy_Scor = Scor;
	copy_cor = nr_corect;
	copy_gre = nr_gresit;
	//check if the answer given is wrong or correct
	if(corect)
	{
		if(corect > 4)
		{
			corect = corect -4;
			FinalQ();
		}
		else
		{
			if(current_item(game_menu) == my_answers[corect])
			{
				Scor = Scor + 100 * (Scor/500 + 1);
				nr_corect++;
			}
			else
			{
				if(Scor)
					Scor = Scor - 50;
				nr_gresit++;
			}
		}
		if(skip)
		{
			Scor = copy_Scor;
			nr_gresit = copy_gre;
			nr_corect = copy_cor;
		}
	}
	else
		return 0;
	//and move to the next question
	ReadQ();
	wrefresh(GameWindow);
	RefreshQ();
	menu_driver(game_menu, REQ_DOWN_ITEM);
	wrefresh(GameWindow);
	return 1;
}

void RefreshQ()
{
	//redo the menu with questions
	int i,n_answers = 6;
	wclear(GameWindow);
	box(GameWindow,0,0);
	unpost_menu(game_menu);
	wrefresh(GameWindow);
	my_answers = (ITEM **)calloc(n_answers, sizeof(ITEM *));
	for(i = 0; i < n_answers; ++i)
		my_answers[i] = new_item(QandA[i], TestQ[5]);
	game_menu = new_menu((ITEM **)my_answers);
	wbkgd(GameWindow,COLOR_PAIR(1));
	set_menu_win(game_menu, GameWindow);
	set_menu_sub(game_menu, derwin(GameWindow, 6, GameX -24, 3, 1));
	set_menu_mark(game_menu, " * ");
	//and repost it with the next question
	sprintf(Scor_curent, "Suma actuala este %d$", Scor);
	print_in_middle(GameWindow, 1, 1, 44, Scor_curent, COLOR_PAIR(1));
	mvwaddch(GameWindow, 2, 0, ACS_LTEE);
	mvwhline(GameWindow, 2, 1, ACS_HLINE, GameX -24);
	mvwaddch(GameWindow, 2, GameX -23, ACS_RTEE);
	refresh();
	post_menu(game_menu);
	wrefresh(GameWindow);
}

void ReadQ()
{
	int i, j;
	char buffer[100];
	for (i = 0; i <=4; ++i)
	{
		fgets(buffer,100,DATA);
		for(j=0;j<strlen(buffer)-1;j++)
			QandA[i][j]=buffer[j];
	}
}

int Tabs()
{
	//creates the window at the bottom of the screen
	FILE *icon;
	char line[100];
	int i = 3 ;
	TabWindow = newwin(10, GameX - 2, GameY-10, 2);
	box(TabWindow, 0, 0);
	icon = fopen("Grafica/icon1.txt", "r");
	while(fgets(line, 100, icon) != NULL)
	{
		mvwprintw(TabWindow, i, 2,line);
		i++;
	}
	sprintf(line, "Current Highscor is :%d$",Highscor);
	mvwprintw(TabWindow, i+1, 2,line);
	updateTime();
	return 0;
}

void cutIcon()
{
	//graphically cut the icons when using special variants
	if(skips != 2)
	{
		if(fiftys != 2)
		{
			mvwprintw(TabWindow, 5, 2,"@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@     ");
		}
		else
		{
			mvwprintw(TabWindow, 5, 2,"@@@@@@@@@@@@@@@@@      |_ | 0 |//|_ | 0 | - F");	
		}
	}
	else
	{
		if(fiftys != 2)
		{
			mvwprintw(TabWindow, 5, 2,"|_ ||  ( | ||  _/ - S  @@@@@@@@@@@@@@@@@@     ");
		}
	}
	box(TabWindow,0,0);
	wrefresh(TabWindow);
}

void FinalQ()
{
	//Post the number of incorrect questions, correct questions and score after the last question
	char aux[100];
	strcpy(aux,"");
	sprintf(aux,"%s%d", QandA[1], nr_corect);
	strcpy(QandA[1], aux);
	sprintf(aux,"%s%d", QandA[2], nr_gresit);
	strcpy(QandA[2], aux);
	sprintf(aux,"%s%d$", QandA[3], Scor);
	strcpy(QandA[3], aux);
	wrefresh(GameWindow);
}

int updateTime()
{
	//change date and time with current system
	if(!(active_game))
		return 0;
	char string[50];
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime ( &rawtime);
	sprintf(string, "Curent local time and date: %s", asctime(timeinfo));
	print_in_middle(TabWindow, 1, 1, GameX-1, string, COLOR_PAIR(1));
	mvwaddch(TabWindow, 2, 0, ACS_LTEE);
	mvwhline(TabWindow, 2, 1, ACS_HLINE, GameX -2);
	mvwaddch(TabWindow, 2, GameX -1, ACS_RTEE);
	box(TabWindow,0,0);
	wrefresh(TabWindow);
	return 0;
}


int main(int argc, char const *argv[])
{
	DATA = fopen(argv[1], "r+");
	int n_choices, i;
	//initializes the game screen (color title)
	initscr();
	start_color();
	cbreak();
	noecho();
	getmaxyx(stdscr,GameY,GameX);
	keypad(stdscr, TRUE);
	title();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	wbkgd(stdscr,COLOR_PAIR(1));

	//Initializes the menu buttons
	n_choices = ARRAY_SIZE(choices);
	my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
	for(i = 0; i < n_choices; ++i)
		my_items[i] = new_item(choices[i], choices[4]);

	//create the menu
	my_menu = new_menu((ITEM **)my_items);
	my_menu_win = newwin(GameY - 22, 20, 12, 2);
	keypad(my_menu_win, TRUE);
	wbkgd(my_menu_win,COLOR_PAIR(1));
	//divides it into two spaces
	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, derwin(my_menu_win, 6, 18, 3, 1));
	set_menu_mark(my_menu, " * ");
	//finish the menu by adding the box and subtitle
	box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 20, "Menu", COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 18);
	mvwaddch(my_menu_win, 2, 19, ACS_RTEE);
	refresh();
	//post the menu and start the game
	post_menu(my_menu);
	wrefresh(my_menu_win);
	menuSelect(my_menu_win,my_menu, my_items);
	if(Scor > Highscor)
	{
		fseek(DATA, 0,SEEK_SET);
		fprintf(DATA, "%d\n", Scor);
	}
	cleanUp();
	endwin();
	return 0;

}
void cleanUp()
{
	//releases the memory allocated to the game
	int i;
	unpost_menu(game_menu);
	free_menu(game_menu);
	delwin(GameWindow);
	unpost_menu(my_menu);
	free_menu(my_menu);
	for(i = 0; i < 4; ++i)
		free_item(my_items[i]);
	delwin(my_menu_win);
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
	float temp;
	// the function prints in the middle of a row and is used for writing separately in its own borders
	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}