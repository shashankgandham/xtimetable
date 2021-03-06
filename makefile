# MIT LICENSE Copyright (c) 2016 Shashank Gandham


personal : xtimetable
	@echo "The Program was compiled successfully and its details are :"
	@echo "Lines Bytes Filename"
	@wc -lc *.c *.h

xtimetable: .teacher.o .batch.o .subject.o .room.o .alloc.o .database.o .timetable.o .guio.o
	@gcc -o xtimetable .batch.o .teacher.o .subject.o .room.o .alloc.o .database.o .timetable.o .guio.o -lncurses -lmenu -g

.teacher.o: teacher.c
	@gcc -c -Wall teacher.c -o .teacher.o -g

.batch.o: batch.c
	@gcc -c -Wall batch.c -o .batch.o -g

.guio.o: guio.c
	@gcc -c -Wall guio.c -o .guio.o -g

.timetable.o: timetable.c
	@gcc -c -Wall timetable.c -o .timetable.o -g

.subject.o: subject.c
	@gcc -c -Wall subject.c -o .subject.o -g

.room.o: room.c
	@gcc -c -Wall room.c -o .room.o -g

.alloc.o: alloc.c
	@gcc -c -Wall alloc.c -o .alloc.o -g

.database.o: database.c
	@gcc -c -Wall database.c -o .database.o -g

clean:
	rm .*.o
	rm .*.swp
