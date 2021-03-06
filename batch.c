/*
	MIT LICENSE Copyright (c) 2016 Shashank Gandham
*/

#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int add_batch(char *database, batch *xbatch) {
	FILE *fp;
	int n;
	char file[64];
	n = batch_number(database);
	if(n) {
		batch ybatch;
		ybatch = get_batch(database, n-1);
		xbatch -> index = ybatch.index + 1;
	}
	else
		xbatch->index = 0;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_batch");
	fp = fopen(file,"a");
	fprintf(fp,"%d %d %d %s\n",xbatch->index,xbatch->parent,xbatch->strength,xbatch->name);
	fclose(fp);
	return 0;
}

int remove_batch(char *database, int index){
	FILE *fp;
	int n, i;
	char file[64],temp_file[64];
	n = batch_number(database);
	batch xbatch;
	strcpy(temp_file, "Database/");
	strcat(temp_file, ".");
	strcat(temp_file, database);
	strcat(temp_file, "_batch");
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_batch");
	fp = fopen(temp_file,"w");
	for(i = 0; i < n; i++) {
		if(i != index) {
			xbatch = get_batch(database, i);
			fprintf(fp,"%d %d %d %s\n",xbatch.index,xbatch.parent,xbatch.strength ,xbatch.name);
		}
	}
	remove(file);
	rename(temp_file,file);
	fclose(fp);
	return 0;
}
int edit_batch(char *database, int index, batch *xbatch){
	FILE *fp;
	int n, i;
	char file[64], temp_file[64];
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_batch");
	strcpy(temp_file, "Database/");
	strcat(temp_file, ".");
	strcat(temp_file, database);
	strcat(temp_file, "_batch");
	fp = fopen(temp_file,"a");
	n = batch_number(database);
	batch ybatch;
	for(i = 0; i < n; i++) {
		ybatch = get_batch(database, n-1);
		if(i!=index)
			fprintf(fp,"%d %d %d %s\n",ybatch.index,ybatch.parent,ybatch.strength, ybatch.name);
		else
			fprintf(fp,"%d %d %d %s\n",xbatch->index,xbatch->parent,xbatch->strength, xbatch->name);
	}
	remove(file);
	rename(temp_file,file);
	fclose(fp);
	return 0;
}
batch get_batch(char *database, int index) {
	FILE *fp;
	int n = 0;
	char file[64];
	batch xbatch;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_batch");
	fp = fopen(file,"r");
	while(fscanf(fp,"%d %d %d %[^\n]s",&xbatch.index, &xbatch.parent,&xbatch.strength,xbatch.name) != EOF) {
		if(n == index)
			break;
		n++;
	}
	fclose(fp);
	return xbatch;
}
int batch_number(char *database) {
	FILE *fp;
	int n = 0;
	char file[64];
	batch xbatch;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_batch");
	fp = fopen(file,"r");
	if(fp == NULL) {
		fp = fopen(file,"w");
		fclose(fp);
		return 0;
	}
	while(fscanf(fp,"%d %d %d %[^\n]s",&xbatch.index, &xbatch.parent, &xbatch.strength, xbatch.name) != EOF)
		n++;
	fclose(fp);
	return n;
}
int sort_batch(char *database , int(*compare)(const void *x ,const void *y)){
	return 0;
}
int show_batch_info(char *database, int choice) {
	int c;
	batch xbatch;
	xbatch = get_batch(database,choice);
	refresh();
	noecho();
	curs_set(0);
	WINDOW *win;
	int y,x;
	start_color();
	getmaxyx(stdscr,y,x);
	win = newwin(0, 0, 0, 0);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	box(win, 0, 0);
	print_in_middle(win, y/4 + 1, 0, x, "Info", COLOR_PAIR(1));
	mvwhline(win, y/4, x/4, ACS_HLINE, x/2);
	mvwhline(win, y/4 + 2, x/4, ACS_HLINE, x/2);
	mvwhline(win, y/2, x/4, ACS_HLINE, x/2);
	mvwvline(win, y/4 + 1, x/4 , ACS_VLINE, y/4 - 1);
	mvwaddch(win, y/4, x/4 , ACS_ULCORNER);
	mvwaddch(win, y/2, x/4 , ACS_LLCORNER);
	mvwaddch(win, y/4, 3*x/4 , ACS_URCORNER);
	mvwaddch(win, y/2, 3*x/4 , ACS_LRCORNER);
	mvwvline(win, y/4 + 1, 3*x/4, ACS_VLINE, y/4 - 1);
	mvwaddch(win, y/4 + 2, 3*x/4 , ACS_RTEE);
	mvwaddch(win, y/4 + 2, x/4 , ACS_LTEE);
	wrefresh(win);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	move(y/4 + 3,x/3 + 2);
	printw("Name - %s",xbatch.name);
	move(y/4 + 5,x/3 + 2);
	printw("Strength - %d",xbatch.strength);
//	move(y/4 + 7,x/3 + 2);
//	printw("Parent - %d",xbatch.parent);
	mvwprintw(win,y - 2, 2,"A:Add ALlocation\t\tB:Back\tQ:Quit");
	refresh();
	while((c = wgetch(win))){
		switch(c) {
			case KEY_DOWN:
			case KEY_UP:
				return 0;
			case 'B':
			case 'b':
				curs_set(1);
				return 1;
			case 'Q':
			case 'q':
				curs_set(1);
				return INT_MIN;
			case 'A':
			case 'a':
				curs_set(1);
				return 2;
			default:
				break;
		}
	}
	return 0;
}
int start_batch(char *database){
	int choice, n, sub_choice;
	while(1) {
		choice = batch_menu(database);
		n = batch_number(database);
		if(choice == n + 1)
			break;
		else if(choice == n + 2)
			batch_form(database);
		else if(choice == INT_MIN)
			return INT_MIN;
		else if(choice == -1)
			continue;
		else {
			while(1) {
				sub_choice = show_batch_info(database,choice);
				if(sub_choice == 1)
					break;
				if(sub_choice == INT_MIN)
					return INT_MIN;
				if(sub_choice == 2)
					alloc_form_batch(database,choice);
			}
		}
	}
	return 0;
}
int batch_menu(char *database){
	int i, c, n, choice = 0;
	batch *xbatch;
	WINDOW *win;
	ITEM **items;
	MENU *menu;
	n = batch_number(database);
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	xbatch = (batch *)malloc(sizeof(batch) * (n + 1));
	items = (ITEM **)calloc(n + 3, sizeof(ITEM *));
	for(i = 0; i < n; ++i) {
		xbatch[i] = get_batch(database,i);
		items[i] = new_item(xbatch[i].name, NULL);
	}
	menu = new_menu((ITEM **)items);
	win = newwin(0, 0, 0, 0);
	int y,x;
	getmaxyx(win,y,x);
	keypad(win, TRUE);
	set_menu_win(menu, win);
	set_menu_sub(menu, derwin(win, y - 5, 38, 5, 0.4*x));
	set_menu_format(menu,x - 4, 1);
	set_menu_mark(menu, " * ");
	set_menu_spacing(menu, 0, 2, 0);
	box(win, 0, 0);
	print_in_middle(win, 1, 0, x, "Batchs" , COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	refresh();
	if(n) {
		if(n > 1)
			mvwprintw(win,y - 2, 2,"N:New Batch\t\tR:Remove Batch\tS:Sort\t\tB:Back\tQ:Quit");
		else
			mvwprintw(win,y - 2, 2,"N:New Batch\t\tR:Remove Batch\tB:Back\tQ:Quit");
		for(i = 0; i < choice; i++)
			menu_driver(menu, REQ_DOWN_ITEM);
		post_menu(menu);
		wrefresh(win);
		while((c = wgetch(win))){
			switch(c) {
				case KEY_DOWN:
					menu_driver(menu, REQ_DOWN_ITEM);
					if(choice != n -1)
						choice++;
					break;
				case KEY_UP:
					menu_driver(menu, REQ_UP_ITEM);
					if(choice != 0)
						choice--;
					break;
				case 10: /* Enter */
					remove_menu(menu,items,n);
					return choice;
				case 'R':
				case 'r':
					remove_batch(database, choice);
					remove_menu(menu,items,n);
					return -1;
				case 'B':
				case 'b':
					remove_menu(menu,items,n);
					return n + 1;
				case 'N':
				case 'n':
					remove_menu(menu,items,n);
					return n + 2;
				case 'Q':
				case 'q':
					remove_menu(menu,items,n);
					return INT_MIN;
				default:
					break;
			}
			wrefresh(win);
		}
	}
	else {
		mvwprintw(win,y - 2, 2,"N:New Batch\t\tB:Back\t\tQ:Quit");
		mvwprintw(win,5,3*x/7,"No Batchs found :(\n");
		wrefresh(win);
		curs_set(0);
		while((c = wgetch(win)))
		{
			switch(c) {
				case 'n':
				case 'N':
					remove_menu(menu,items,n);
					curs_set(1);
					return n+2;
				case 'b':
				case 'B':
					remove_menu(menu,items,n);
					curs_set(1);
					return n+1;
				case 'Q':
				case 'q':
					remove_menu(menu,items,n);
					curs_set(1);
					return INT_MIN;
				default:
					break;
			}
			wrefresh(win);
		}
	}
	free(xbatch);
	return -1;
}
int batch_form(char *database){
	refresh();
	batch xbatch;
	noecho();
	WINDOW *win;
	int y,x;
	start_color();
	getmaxyx(stdscr,y,x);
	win = newwin(0, 0, 0, 0);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	box(win, 0, 0);
	print_in_middle(win, y/4 + 1, 0, x, "Enter the Name of Batch", COLOR_PAIR(1));
	print_form_str(win, y,x);	
	scanstr(win,xbatch.name,x/3 - 1);
	clear();
	refresh();
	xbatch.parent = -1;
	box(win, 0, 0);
	print_in_middle(win, y/4 + 1, 0, x, "Enter the strength of the batch", COLOR_PAIR(1));
	print_form_str(win, y,x);	
	xbatch.strength = scanint(win);
	add_batch(database, &xbatch);
	refresh();
	endwin();
	clear();
	return 0;
}
