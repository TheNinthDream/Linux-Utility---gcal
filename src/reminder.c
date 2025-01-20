#include <assert.h>
#include "reminder.h"
#include <string.h>
#include <time.h>

void initializeMonth(void) {
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	currentMonth.month_idx = t->tm_mon;
	t->tm_mday = 1;
	mktime(t);
	currentMonth.start_day = t->tm_wday;
	currentMonth.month_days = t->tm_mday;
	while (t->tm_mon == currentMonth.month_idx){
		currentMonth.month_days = t->tm_mday;
		t->tm_mday++;
		mktime(t);
	}

	for (int i = 0; i < 32; i++) {
		currentMonth.reminderList[i] = NULL;  // Initialize each list head to NULL
	}
}

void freeMonth(void) {
	//go through all valid days (not including 0), for each create a head node
	//till it's null, create a temp node to hold the current node, and free it
	for (int i = 1; i < currentMonth.month_days + 1; i++) {
		struct Node *currentNode = currentMonth.reminderList[i];
		while (currentNode != NULL) {
			struct Node *temp = currentNode;
			currentNode = currentNode->next;
			free(temp);
		}
		currentMonth.reminderList[i] = NULL; //explicitly
	}
}

//helper function
void printDay(int day) {
	printf("Day %2d:: ", day);
	struct Node *temp = currentMonth.reminderList[day];
	int c = 1;
	while (temp != NULL) {
		if (c == 1) {printf("(%d) %s\n", c, temp->reminder);}
		else {printf("         (%d) %s\n", c, temp->reminder);}
		temp = temp->next;
		c += 1;
	}
}

void calendar() { //change name to calendar
	int month_days = currentMonth.month_days;
	int start_day = currentMonth.start_day;
	int row = (month_days + start_day - 1) / 7 + 1, col = 7;

    int calendar[10][10]; // Don't want to allocate dynamically if I can help it
    int count = 1;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            	calendar[i][j] = 0;
        }
   	}

    for (int i = 0; i < row; i++) {
        for (int j = (i == 0 ? start_day - 1 : 0); j < col; j++) {
            calendar[i][j] = count;
            count += 1;
            if (count > month_days) break;
        }
    }

	printf("SUN MON TUE WED THU FRI SAT\n");

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			int temp = calendar[i][j];
			if (currentMonth.reminderList[temp] != NULL) {
				printf("(%2d)", temp);
			}
			else if (temp == 0) {printf("    ");}
			else {
				printf(" %2d ", temp);
			}
		}
		printf("\n");
	}

	int m = currentMonth.month_idx;
	switch (m) {
   	 		case 0: printf("January reminders: \n"); break;
   	 		case 1: printf("February reminders: \n"); break;
   	 		case 2: printf("March reminders: \n"); break;
   	 		case 3: printf("April reminders: \n"); break;
   	 		case 4: printf("May reminders: \n"); break;
   	 		case 5: printf("June reminders: \n"); break;
   	 		case 6: printf("July reminders: \n"); break;
   	 		case 7: printf("August reminders: \n"); break;
   	 		case 8: printf("September reminders: \n"); break;
   	 		case 9: printf("October reminders: \n"); break;
   	 		case 10: printf("November reminders: \n"); break;
   	 		case 11: printf("December reminders: \n"); break;
   	 		default:
   	 	}
   	 for (int k = 0; k < 32; k++) {
	   	 if (currentMonth.reminderList[k] != NULL) {
	   	 	printDay(k);
	   	 }
   	 }
}


void insert_to_calendar(int day, const char* value) {
	struct Node* newNode = malloc(sizeof(struct Node));
	strncpy(newNode->reminder, value, MAX_STR_LEN - 1);
	newNode->reminder[strlen(value)] = '\0';
	newNode->next = NULL;

	if (currentMonth.reminderList[day] == NULL) {
		currentMonth.reminderList[day] = newNode;
	}
	else {
		struct Node* temp = currentMonth.reminderList[day];
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = newNode;
	}
}

void delReminder(int day, int index, const char *fileName) {
	struct Node* temp = currentMonth.reminderList[day];
	struct Node* temp2 = NULL;

	index--; //decrement the index to match the normal numbering system
		 //if index is 1, I expect to remove the first reminder not the second
	
	//base case
	if (index == 0) {
		if (temp != NULL) {
			currentMonth.reminderList[day] = temp->next;
			free(temp);
		}
	}
	else {
		while (index > 0) {
			if (temp == NULL) {
				printf("Invalid Index");
				return;
			}
			temp2 = temp;
			temp = temp->next;
			index -= 1;
		}

		if (temp2 != NULL) {
			temp2->next = temp->next;
			free(temp);
		}
	}

	//update the list
	storeMonth(fileName);
}

void storeMonth(const char *fileName) {
	FILE* file = fopen(fileName, "w");
	if (file == NULL) {
		printf("Could not open file\n");
		return;
	}
	//loop through valid days (exclude 0)
	for (int i = 1; i < currentMonth.month_days; i++) {
		struct Node* temp = currentMonth.reminderList[i];
		while (temp != NULL) {
			fprintf(file, "D%d: %s\n", i, temp->reminder);
			temp = temp->next;
		}
	}
	fclose(file);
}

int loadMonth(const char *fileName) {
	int load = 0;
	freeMonth(); //avoid duplication
	FILE* file = fopen(fileName, "r");
	if (file == NULL) {
		printf("Could not open file\n");
		return load;
	}

	char str[500];
	int day;

	while (fgets(str, 500, file) != NULL) {
		char reminder[MAX_STR_LEN];
		if (sscanf(str, "D%d: %[^\n]", &day, reminder) == 2) { //finally using regex
			insert_to_calendar(day, reminder);
			load = 1; //acts as bool flag
		}
		else {return load;}

	}
	fclose(file);
	return load;
}
