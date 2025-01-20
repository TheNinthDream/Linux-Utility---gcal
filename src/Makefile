#executable
gcal: main.o reminder.o
	gcc -o gcal main.o reminder.o

#main.c to main.o
main.o: main.c reminder.h
	gcc -c main.c

reminder.o: reminder.c reminder.h
	gcc -c reminder.c

#clear
clean:
	rm -f *.o gcal

