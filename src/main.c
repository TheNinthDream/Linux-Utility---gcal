#include "reminder.h"
#include <signal.h>
#include <time.h>
#include <ctype.h>

//signal handling function
void signal_handler(int sig) {
    if (sig == SIGINT || sig == SIGTERM || sig == SIGSEGV) {
        printf("\nSIGNAL %d: SAVING REMINDERS TO FILE AND TERMINATING\n", sig); //log sig num
        storeMonth("reminder.txt");
        freeMonth();
        exit(sig);
    }
}

struct Month currentMonth;

void printUsage() {
    printf("Invalid arguments\n");
    printf("Usage: ./gcal [view | add <day> <reminder> | remove <day> <index> | today]\n");
}

void misc() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGSEGV, signal_handler);

    initializeMonth(); //load month struct

    loadMonth("reminder.txt");
}

int dayCheck(int day) {
    if (day < currentMonth.start_day || day > currentMonth.month_days) {
        printUsage();
        return 1;
    }
    return 0;
}


int isNumber(const char *str) {
    // Check if string is not empty and contains only digits
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0; // Not a number
        }
    }
    return 1; // Is a number
}

int main (int argc, char *argv[]) {
    misc();


    if (argc < 2) {
        printUsage();
        exit(1);
    }

    if (strcmp(argv[1], "view") == 0) {
        calendar();
    }
    else if (strcmp(argv[1], "add") == 0) {
        if (argc < 4) {
            printf("Usage: add <day> <reminder>\n");
            return 1;
        }
        if (strlen(argv[3]) == 0) {
            printf("Reminder text cannot be empty.\n");
            return 1;
        }
        if (!isNumber(argv[2])) {
            printf("Invalid day\n");
	    return 1;
        }

        int day = atoi(argv[2]);
        if (dayCheck(day)) {
            exit(1); //invalid day
        }

	char reminder[MAX_STR_LEN] = "";
    	for (int i = 3; i < argc; i++) {
        	strcat(reminder, argv[i]);
        	if (i < argc - 1) strcat(reminder, " "); // Add a space between words
    	}

        insert_to_calendar(day, reminder);
        storeMonth("reminder.txt");
        calendar();

    }
    else if (strcmp(argv[1], "remove") == 0) {
        if (argc < 4) {
            printf("Usage: remove <day> <index>\n");
            return 1;
        }
        int day = atoi(argv[2]);
        int index = atoi(argv[3]);
        if (dayCheck(day)) {
            exit(1); //invalid day
        }
        delReminder(day, index, "reminder.txt"); //built in store month
        calendar();
    }
    else if (strcmp(argv[1], "today") == 0) {
        time_t now = time(NULL);
        struct tm *today = localtime(&now);
        int day = today->tm_mday;
        printDay(day);
    }
    else {
    	printUsage();
    }
    return 0;
}
