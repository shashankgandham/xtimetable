#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char command[1024];
	system("git grep -I --name-only -z -e '' | xargs -0 sed -i 's/[ \t]\+\(\r\?\)$/\1/'");
	/*system("git add *.c *.h makefile README.md LICENSE");*/
	/*strcpy(command,"git commit -m ");*/
	/*strcat(command,"\"");*/
	/*strcat(command,argv[1]);*/
	/*strcat(command,"\"");*/
	/*system(command);*/
	/*system("git push origin master");*/

	return 0;
}