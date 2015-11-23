/*  This file is part of Jinx originally written by Shashank gandham.

    Timetable Generator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Timetable Generator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Jinx.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "teacher.h"
#include "subject.h"
#include "batch.h"
#include "room.h"
#include <stdio.h>
#include <string.h>
#include <menu.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

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
	char database[128];	
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
	return 1;
}

void new_database_form() {
	refresh();
	char name[128];
	echo();
	WINDOW *win;
	int y,x;	
	start_color();
	getmaxyx(stdscr,y,x);
	win = newwin(6, 40, y/3, x/3);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	box(win, 0, 0);
	print_in_middle(win, 1, 0, 40, "Enter the name of database", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 38);
	mvwaddch(win, 2, 39, ACS_RTEE);
	wrefresh(win);
	move(y/3 + 3,x/3 + 2);
	scanw(" %[^\n]s",name);
	new_database(name);
	refresh();
	endwin();
	clear();
	return;
}
int show_database_menu(char *database) {		
	int i, c;
	int n = 8;
	ITEM **items;
	MENU *menu;
	WINDOW *win;
	char *choices[] =  {
		"Teachers",
		"Subjects",
		"Classes",
		"Rooms",
		"Slots",
		"Make Timetable",
		"Remove Database",
		"Back",
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
	set_menu_format(menu,x - 4, 1);
	set_menu_mark(menu, " * ");
	box(win, 0, 0);
	print_in_middle(win, 1, 0, x, database, COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
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
				break;
			
			default:
				break;
		}
		if(c == 10)
			break;
		wrefresh(win);
	}	
	return choice;
}
void database_menu(char *database) {
	int n = 8, choice;
	while(1){
		choice = show_database_menu(database);
		if(choice == 0)
			start_teacher(database);
		if(choice == 1)
			start_subject(database);
		if(choice == 2)
			start_batch(database);
		if(choice == 3)
			start_room(database);
		if(choice == n - 1)
			return;
		if(choice == n - 2) {
			remove_database(database);
			return;
		}
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
	set_menu_format(menu,x - 4, 1);
	set_menu_mark(menu, " * ");
	box(win, 0,0);
	print_in_middle(win, 1, 0, x, "My Menu", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	mvwprintw(win,y - 2, 2,"N:New Database\tQ:Quit");	
	refresh();
	int choice = 0;
	if(n) {
		post_menu(menu);
		wrefresh(win);
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
					break;
				default:
					break;
			}
			wrefresh(win);
		}	
	}
	else {
		
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
			wrefresh(win);
		}
	}
		return -1;
}
int main() {
	int choice, n;
	char database[128];
	initscr();
	start_color();
	cbreak();
	noecho();
	while(1) {
		n = database_number();
		choice = main_menu();
		if(choice == n + 2) {
			endwin();
			return 0;
		}
		else if(choice == n + 1)
			new_database_form();
		else {
			get_database(choice, database);
			database_menu(database);
		}
	}
	return 0;
}
