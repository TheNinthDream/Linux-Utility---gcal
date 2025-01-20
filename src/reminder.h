#ifndef REMINDER_H
#define REMINDER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STR_LEN 100

struct Node {
    char reminder[MAX_STR_LEN];
    struct Node* next;
};

struct Month{
    int month_idx;
    int month_days;
    int start_day;
    struct Node* reminderList[32];
};

extern struct Month currentMonth;

//Set the month struct values
void initializeMonth(void);

//Free memory
void freeMonth(void);

//Modification of Lab3, displays calendar

void calendar();

/*
Takes a reminder string and a day number and inserts the reminder into the calendar.
Also, sets the reminder flag to true for that day.
 */

void insert_to_calendar(int day, const char* value) ;


//Takes the input from the user when
int reader(char *str, int n) ;

/*
Prints the calendar and the reminders for each day.
 */
void print_calendar();

//prints all reminders in a linked list for a day
void printDay(int day);

//Delete from calendar
void delReminder(int day, int index, const char *fileName);

/*
 * reminders.txt
 * - Stores the reminders in the format "D<day>: <reminder>"
 * - This file must be in the same directory as the code file
 * It will automatically be created after the first normal run of the program, by storeMonth
 */
//store to file
void storeMonth(const char* filename);

//load to file
int loadMonth(const char* filename);

#endif
