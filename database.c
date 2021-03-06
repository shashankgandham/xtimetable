/*
	MIT LICENSE Copyright (c) 2016 Shashank Gandham
*/
#include "library.h"
#include <stdio.h>
#include <string.h>
#include <menu.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <limits.h>

int new_database(char *name) {
	FILE *fp, *gp;
	char database[128];
	int d;
	fp = fopen("Database/database.txt","r");
	gp = fopen("Database/.database_temp.txt","w");
	fprintf(gp,"%s\n",name);
	while((d = fscanf(fp," %[^\n]s",database)) && d != EOF ) {
		fprintf(gp,"%s\n",database);
	}
	fclose(fp);
	fclose(gp);
	remove("Database/database.txt");
	rename("Database/.database_temp.txt","Database/database.txt");
	return 1;
}
int remove_database(char *data) {
	FILE *fp, *gp;
	char database[128],file[256];
	int d, i = 0;
	fp = fopen("Database/database.txt","r");
	gp = fopen("Database/.database_temp.txt","w");
	while((d = fscanf(fp," %[^\n]s",database)) && d != EOF ) {
		if(strcmp(database,data))
			fprintf(gp,"%s\n",database);
		i++;
	}
	fclose(fp);
	fclose(gp);
	remove("Database/database.txt");
	rename("Database/.database_temp.txt","Database/database.txt");
	strcpy(database,"Database/");
	strcat(database,data);
	strcpy(file,database);
	strcat(file,"_teacher");
	remove(file);
	strcpy(file,database);
	strcat(file,"_subject");
	remove(file);
	strcpy(file,database);
	strcat(file,"_batch");
	remove(file);
	strcpy(file,database);
	strcat(file,"_room");
	remove(file);
	return 1;
}
void new_database_form() {
	refresh();
	char name[128];
	noecho();
	WINDOW *win;
	int y,x;
	start_color();
	getmaxyx(stdscr,y,x);
	win = newwin(0, 0, 0, 0);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	box(win, 0, 0);
	print_in_middle(win, y/4 + 1, 0, x, "Enter the Name of Database", COLOR_PAIR(1));
	print_form_str(win,y,x);
	scanstr(win,name, x/3 - 1);
	new_database(name);
	refresh();
	endwin();
	clear();
	return;
}
int show_database_menu(char *database) {
	int i, c;
	int n = 6;
	ITEM **items;
	MENU *menu;
	WINDOW *win;
	char *choices[] =  {
		"Teachers",
		"Subjects",
		"Batches",
		"Rooms",
		"Slots",
		"Timetable",
	};
	int x, y;
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	items = (ITEM **)calloc(n + 2, sizeof(ITEM *));
	for(i = 0; i < n; ++i) {
		items[i] = new_item(choices[i], NULL);
	}
	menu = new_menu((ITEM **)items);
	win = newwin(0, 0, 0, 0);
	getmaxyx(win,y,x);
	keypad(win, TRUE);
	set_menu_win(menu, win);
	set_menu_sub(menu, derwin(win, y - 5, 38, 5, 0.4*x));
	set_menu_format(menu,y/2, 1);
	set_menu_mark(menu, " * ");
	set_menu_spacing(menu, 0 , 2, 0);
	box(win, 0, 0);
	print_in_middle(win, 1, 0, x, database, COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	mvwprintw(win,y - 2, 2,"B:Back\t\tQ:Quit");
	refresh();
	post_menu(menu);
	wrefresh(win);
	int choice = 0;
	while((c = wgetch(win)))
	{       switch(c)
		{	case KEY_DOWN:
			menu_driver(menu, REQ_DOWN_ITEM);
			if(choice != n - 1)
				choice++;
			break;
			case KEY_UP:
			menu_driver(menu, REQ_UP_ITEM);
			if(choice != 0)
				choice--;
			break;
			case 10: /* Enter */
				remove_menu(menu, items, n);
				return choice;
				break;
			case 'B':
			case 'b':
				remove_menu(menu, items, n);
				return n+1;
			case 'q':
			case 'Q':
				remove_menu(menu, items, n);
				return INT_MIN;
			default:
			break;
		}
		if(c == 10)
			break;
		wrefresh(win);
	}
	return -1;
}
int database_menu(char *database) {
	int n = 6, choice, exit;
	while(1){
		choice = show_database_menu(database);
		if(choice == 0)
			exit = start_teacher(database);
		if(choice == 1)
			exit = start_subject(database);
		if(choice == 2)
			exit = start_batch(database);
		if(choice == 3)
			exit = start_room(database);
		if(choice == 4)
			exit = start_alloc(database);
		if(choice == 5)
			exit = start_timetable(database);
		if(choice == n + 1)
			return 0;
		if(exit == INT_MIN || choice == INT_MIN)
			return INT_MIN;
	}
}
void get_database(int choice, char *choices) {
	FILE *fp;
	fp = fopen("Database/database.txt","r");
	int i;
	i = 0;
	while(fscanf(fp," %[^\n]s",choices)!= EOF) {
		if(i == choice)
			break;
		i++;
	}
	fclose(fp);
}
int database_number() {
	FILE *fp;
	mkdir("Database",S_IRWXU);
	fp = fopen("Database/database.txt","r");
	int i;
	char choices[128];
	if(fp == NULL) {
		fp = fopen("Database/database.txt","w");
		fclose(fp);
		return 0;
	}
	fp = fopen("Database/database.txt","r");
	i = 0;
	while(fscanf(fp," %[^\n]s",choices)!= EOF) {
		i++;
	}
	return i;
	fclose(fp);
}
int main_menu() {
	int i, c, x, y, n;
	int choice = 0;
	n = database_number();
	char **choices;
	choices = (char **)malloc(sizeof(char *) * (n + 3));
	for(i = 0; i < n; i++) {
		choices[i] = (char *)malloc(sizeof(char) *128);
		if(i < n)
			get_database(i, choices[i]);
	}
	MENU *menu;
	n = i;
	WINDOW *win;
	ITEM **items;
begin_main_menu:
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	items = (ITEM **)calloc(n + 1, sizeof(ITEM *));
	for(i = 0; i < n; i++) {
		items[i] = new_item(choices[i], NULL);
	}
	menu = new_menu((ITEM **)items);
	getmaxyx(stdscr,y,x);
	win = newwin(y, x, 0, 0);
	keypad(win, TRUE);
	set_menu_win(menu, win);
	set_menu_sub(menu, derwin(win, y - 7, 38, 5, 0.4*x));
	set_menu_format(menu,y - 4, 1);
	set_menu_mark(menu, " * ");
	set_menu_spacing(menu, 0 , 2, 0);
	box(win, 0,0);
	print_in_middle(win, 1, 0, x, "My Menu", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	refresh();
	if(n) {
		if(n > 1)
			mvwprintw(win,y - 2, 2,"N:New Database\tR:Remove Database\tS:Sort\t\tQ:Quit");
		else
			mvwprintw(win,y - 2, 2,"N:New Database\tR:Remove Database\tQ:Quit");
		post_menu(menu);
		wrefresh(win);
		for(i = 0; i < choice; i++)
			menu_driver(menu, REQ_DOWN_ITEM);
		while((c = wgetch(win)))
		{
			switch(c)
			{	case KEY_DOWN:
					menu_driver(menu, REQ_DOWN_ITEM);
					if(choice != (n - 1))
						choice++;
					break;
				case KEY_UP:
					menu_driver(menu, REQ_UP_ITEM);
					if(choice != 0)
						choice--;
					break;
				case 10:
					remove_menu(menu, items, n);
					return choice;
					break;
				case 'n':
				case 'N':
					remove_menu(menu,items, n);
					return n+1;
				case 'q':
				case 'Q':
					remove_menu(menu,items, n);
					return n+2;
				case 's':
				case 'S':
					remove_menu(menu,items, n);
					return n+3;
				case 'r':
				case 'R':
					remove_menu(menu,items,n);
					remove_database(choices[choice]);
					return -1;
				default:
					break;
			}
			wrefresh(win);
			if(signal(SIGWINCH, NULL) != SIG_ERR) {
				remove_menu(menu,items, n);
				wrefresh(win);
				goto begin_main_menu;
			}
		}
	}
	else {
		mvwprintw(win,y - 2, 2,"N:New Database\tQ:Quit");
		mvwprintw(win,5,3*x/7,"No database found :(\n");
		wrefresh(win);
		curs_set(0);
		while((c = wgetch(win)))
		{
			switch(c) {
				case 'n':
				case 'N':
					remove_menu(menu,items,n);
					curs_set(1);
					return n+1;
				case 'q':
				case 'Q':
					remove_menu(menu,items,n);
					curs_set(1);
					return n+2;
				default:
					break;
			}
			if(signal(SIGWINCH, NULL) != SIG_ERR) {
				wrefresh(win);
				goto begin_main_menu;
			}
			wrefresh(win);
		}
	}
	return -1;
}
int compare(const void *p, const void *q) {
	char *a, *b;
	a = (char *)p;
	b = (char *)q;
	return strcmp(a,b);
}
void sort_database() {
	char database[128][128];
	int i, n;
	n = database_number();
	for(i = 0; i < n; i++)
		get_database(i,database[i]);
	qsort(database,n,128,compare);
	remove("Database/database.txt");
	FILE *fp;
	fp = fopen("Database/database.txt","w");
	for(i = 0; i < n; i++)
		fprintf(fp,"%s\n",database[i]);
	fclose(fp);
}
int main() {
	signal(SIGINT, SIG_IGN);
	int choice, n;
	char database[128];
	initscr();
	start_color();
	cbreak();
	noecho();
	while(1) {
		n = database_number();
		choice = main_menu();
		if(choice == -1)
			continue;
		if(choice == n + 2) {
			endwin();
			return 0;
		}
		else if(choice == n + 1)
			new_database_form();
		else if(choice == n + 3)
			sort_database();
		else {
			get_database(choice, database);
			if(database_menu(database) == INT_MIN) {
				endwin();
				return 0;
			}
		}
	}
	return 0;
}
