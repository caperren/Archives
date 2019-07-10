/*
 * Original Author: Corwin A. Perren (perrenc) and Will H. Seble (seblew)
 * Contain code by: D. Kevin McGrath (dmcgrath)
 * File: perrenc_hw2.h
 * Created: 2012 March 16, 20:26 by perrenc and seblew
 * Last Modified: 2012 March 16, 20:26 by perrenc and seblew
 *
 * This file includes all of the functions necessary to make a gradebook.
 * It is called from perrenc_hw2.c
 *
 */
 
/* Includes */
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <string.h>
#include <math.h>

#ifndef perrenc_hw2
#define perrenc_hw2

/* Defines */

/* --------------------Begin dmcgrath Functions-------------------- */
/*These were copied directly off of the 311 code page*/

struct assignment{
	char *name;     /*!< \brief string to hold the name of the assignment */
	double value;	/*!< \brief hold the given score of the assignment, normalized */
};

struct student{
	
	struct student *next;
	/*! \brief self referential pointer to link to nodes behind */
	struct student *previous;

	/*! \brief string to hold the first name of the student */
	char *first_name;
	/*! \brief string to hold the last name of the student */
	char *last_name;

	/*! \brief pointer to the dynamic assignments array */
	struct assignment *assignments;
	/*! \brief keep a count of the number of assignments */
	long int num_assignments;
};

struct stats{
	double mean;
	double median;
	double stddev;
};	

int compare(const void *s, const void *t){
	int comp = 1;
	
	double is = *(double *)s;
	double it = *(double *)t;
	
	if (is < it){
		comp = -1;
	}else if (is == it){
		comp = 0;
	}
	
	return comp;
}

double median(double *list, long int length){
	double med = 0.0;
	
	/* use the built in quicksort function to sort the list to extract the median */
	/* requires the use of a function pointer */
	qsort(list, length, sizeof(double), compare);
	
	if (length % 2 == 0){
		/* if the list is an even length, return average of middle 2 elements*/
		med = list[length/2 - 1] + list[length/2];
		med /= 2.0;
	}else{
		/* otherwise return the middle element (correct due to 0 offset) */
		med = list[length/2];
	}
	
	return med;
}

double mean(double *list, long int length){
	double total = 0.0;
	long int i;	
	
	for (i = 0; i < length; ++i){
		total += list[i];
	}
	
	return total / length;
} 

double stddev(double *list, long int length){
	double mu = mean(list, length);
	double sigma_2 = 0.0;
	double square = 2.0;
	int i;

	/* sum the deviants (squares of the differences from the average) */
	for (i = 0; i < length; ++i){
		sigma_2 += pow(list[i] - mu, square);
	}
	
	sigma_2 /= (length - 1);
	
	return sqrt(sigma_2);
}

/* --------------------End dmcgrath Functions-------------------- */

/* --------------------Begin perrenc Functions-------------------- */

/*This function is used to let a user manually add a student*/
struct student* temp_student_userin(void){
	int loop = 0; 		//Used for looping

	system("clear"); 	//Clears screen
	
	struct student *tmp;	//Tmp stuct to hold student
	
	/*Allocate space*/
	tmp = (struct student*) malloc(sizeof(struct student));
	tmp->first_name = (char *) malloc(100*sizeof(char));
	tmp->last_name = (char *) malloc(100*sizeof(char));
	
	/*Get name of student*/
	printf("Please enter user's first name...\n");
	scanf("%s", tmp->first_name);
	printf("Please enter user's last name...\n");
	scanf("%s", tmp->last_name);
	
	system("clear");	//Clear Screen

	/*Print student name, ask for number of assignments*/
	printf("The name you entered was: %s %s\n\n", tmp->first_name, tmp->last_name);
	printf("How many assignments would you like to add for this user: ");
	scanf("%d", &tmp->num_assignments);
	printf("\n\n");

	/*Allocate space for assignments*/
	tmp->assignments = (struct assignment*) malloc(tmp->num_assignments * sizeof(struct assignment));
	
	/*Add assignments with names and grades*/
	for(loop = 0 ; loop < tmp->num_assignments ; loop++){
		
		tmp->assignments[loop].name = (char *) malloc(100 * sizeof(char));

		printf("Please enter the assignment %d name with no spaces: ", loop+1);
		scanf("%s", tmp->assignments[loop].name);
		printf("\n");

		printf("Please enter the assignment grade: ");
		scanf("%lf", &tmp->assignments[loop].value);;
		printf("\n");

		printf("You added assignment %s with grade %.2lf\n", tmp->assignments[loop].name
								, tmp->assignments[loop].value);
		printf("\n\n");
	}
	return tmp;	//Return new student
}

/*This function is called to manually add a new student*/
struct student* add_student(struct student *head){
	
	struct student *tmp; 		//tmp student struct
	struct student *cursor = head;	//cursor struct

	tmp = temp_student_userin ();	//gets new student

	/*insert sorts the new student into the gradbook*/
	while(1){
		if(head == NULL){
			head = tmp;
			print_student(head);
			pause();
			return head;
		}else if((strcmp(tmp->last_name, cursor->last_name) < 0) | 
			(strcmp(tmp->last_name, cursor->last_name) == 0)){
		
			if(cursor->previous == NULL){
				tmp->previous = NULL;
				tmp->next = cursor;
				cursor->previous = tmp;
				head = tmp;
				return head;
			}else if(cursor->previous != NULL){
				tmp->previous = cursor->previous;
				tmp->next = cursor;
				cursor->previous->next = tmp;
				tmp->next->previous = tmp;
				return head;
			}	

		}else if(strcmp(tmp->last_name, cursor->last_name) > 0){
	
			if(cursor->next != NULL){
				cursor = cursor->next;
			}else{
				tmp->previous = cursor;
				tmp->next = NULL;
				cursor->next = tmp;
				return head;
			}	
		}
	}	
}

/*This function resets pointers on surround students when one is removed*/
struct student* remove_reset_pointers(struct student *cursor, struct student *head){
	int loop;	//For loops
	
	/*Resets pointers on surrounding students depeind on whether the one that was 
	 * removed was at the head/tail/or middle
	 */
	if((cursor->next == NULL) & (cursor->previous == NULL)){
		head = NULL;
	}else if(cursor->next == NULL){
		cursor->previous->next = NULL;
	}else if(cursor->previous == NULL){
		head = cursor->next;
		cursor->next->previous = NULL;
	}else{
		cursor->previous->next = cursor->next;
		cursor->next->previous = cursor->previous;
	}
	
	/*Frees the deleted student assignments*/
	for(loop = 0 ; loop < cursor->num_assignments ; loop++){
		free(cursor->assignments[loop].name);
	}	
	
	/*Frees the student struct entries and struct itself*/
	free(cursor->first_name);
	free(cursor->last_name);
	free(cursor->assignments);
	free(cursor);

	return head; 	//Returns the new head
}

/*This function is used to remove a student from the gradebook*/
struct student* remove_student(struct student *head){
	struct student *cursor = head;				//cursor struct
	char *firstname = (char *) malloc(100 * sizeof(char));	//For user input
	char *lastname = (char *) malloc(100 * sizeof(char));	//For user imput
	int loop;						//For looping

	system("clear");					//Clear Screen

	/*Asks the user which student they want to delete*/
	printf("Please enter student's first name to delete: ");
	scanf("%s", firstname);
	printf("\n");
	printf("Please enter student's last name to delete: ");
	scanf("%s", lastname);
	printf("\n\n");
	
	/*Finds the student in the gradebook by name seach*/
	while(1){
		if(strcmp(cursor->last_name, lastname) != 0 ){
			if(cursor->next != NULL){
				cursor = cursor->next;
			}else{
				system("clear");
				printf("No user by that name was found\n");
				pause();
				return NULL;
			}	
		}else if(strcmp(cursor->last_name, lastname) == 0){
			if(strcmp(cursor->first_name, firstname) != 0){
				cursor = cursor->next;
			}else if(strcmp(cursor->first_name, firstname) == 0){
				head = remove_reset_pointers(cursor, head);
				return head;
			}	
		}	
		
	}		


}	

/*This function is used by the read_file function to sort and place students in the
 * gradebook.
 */
struct student* file_insert(struct student *head, struct student *tmp){
	
	struct student *cursor = head;	//cursor struct
	
	/*
	 * Determines wherer in the gradebook the student needs to be places, and puts them
	 * there.
	 */
	while(1){
		if(head == NULL){
			head = tmp;
			return head;
		}else if((strcmp(tmp->last_name, cursor->last_name) < 0) | 
			(strcmp(tmp->last_name, cursor->last_name) == 0)){
		
			if(cursor->previous == NULL){
				tmp->previous = NULL;
				tmp->next = cursor;
				cursor->previous = tmp;
				head = tmp;
				return head;
			}else if(cursor->previous != NULL){
				tmp->previous = cursor->previous;
				tmp->next = cursor;
				cursor->previous->next = tmp;
				tmp->next->previous = tmp;
				return head;
			}	

		}else if(strcmp(tmp->last_name, cursor->last_name) > 0){
	
			if(cursor->next != NULL){
				cursor = cursor->next;
			}else{
				tmp->previous = cursor;
				tmp->next = NULL;
				cursor->next = tmp;
				return head;
			}	
		}
	}	
}	

/*
 * This function opens a file and turns it's input into structs to be placed in the 
 * gradbook, then places them there
 */
struct student* read_from_file(struct student *head){

	struct student *tmp;			//Tmp struct
	FILE *gradebook;			//File input stream
	int loop;				//For loops

	gradebook = fopen("gradebook.db", "r");	//Points file to stream
	
	/*
	 * Creates structs for each student in the file until a null character is recieved
	 * and then puts the students in the gradbook
	 */
	while(!feof(gradebook)){
		tmp = (struct student*) malloc(sizeof(struct student));	//Make space for student
		tmp->first_name = (char *) malloc(100*sizeof(char));	//Make space for student
		tmp->last_name = (char *) malloc(100*sizeof(char));	//Make space for student
		
		/*Fills struct with student data and assignment data*/
		if(fscanf(gradebook, "%s\t%s\t%d", tmp->first_name, tmp->last_name, 
							&tmp->num_assignments) == 3){
			tmp->assignments = (struct assignment*) 
				malloc(tmp->num_assignments * sizeof(struct assignment));
	
			for(loop = 0 ; loop < tmp->num_assignments ; loop++){
	
				tmp->assignments[loop].name = (char *) 
						malloc(100 * sizeof(char));
				fscanf(gradebook, "%s", tmp->assignments[loop].name);
				fscanf(gradebook, "%lf", &tmp->assignments[loop].value);
			}
			head = file_insert(head, tmp);
		}else{
			free(tmp->first_name);
			free(tmp->last_name);
			free(tmp);
		}	
	}	
	
	fclose(gradebook);	
	return head;
}

/*This function writes the current gradebook to a file to be opened later*/
int write_to_file(struct student *head){

	struct student *cursor = head;		//cursor struct
	int loop;				//for loops

	FILE *gradebook;			//file stream
	gradebook = fopen("gradebook.db", "w");	//points file to stream

	/*Properly formatas and places students into the file*/
	while(cursor != NULL){
	
		fprintf(gradebook, "%s\t%s\t%d\t", cursor->first_name, cursor->last_name,
								cursor->num_assignments);
		for(loop = 0 ; loop < cursor->num_assignments ; loop++){
			if(loop < (cursor->num_assignments - 1)){
				fprintf(gradebook, "%s\t%lf\t", 
							cursor->assignments[loop].name,
							cursor->assignments[loop].value);
			}else{
				fprintf(gradebook, "%s\t%lf\n", 
							cursor->assignments[loop].name, 
							cursor->assignments[loop].value);
			}
		}
		cursor = cursor->next;
	}
	fclose(gradebook);
}

/*This function searches for the student the user specifies, then prints out their data*/
int name_search_print(struct student *head){

	struct student *cursor = head;				//cursor struct
	char *firstname = (char *) malloc(100 * sizeof(char));	//space for name
	char *lastname = (char *) malloc(100 * sizeof(char));	//space for name
	int loop;

	system("clear");					//clears screen

	/*Asks the user the student they'd like to print*/
	printf("Please enter student's first name: ");
	scanf("%s", firstname);
	printf("\n");
	printf("Please enter student's last name: ");
	scanf("%s", lastname);
	printf("\n\n");

	/*finds student and prints them*/
	while(1){
		if(strcmp(cursor->last_name, lastname) != 0 ){
			if(cursor->next != NULL){
				cursor = cursor->next;
			}else{
				system("clear");
				printf("No user by that name was found\n");
				pause();
				return 1;
			}	
		}else if(strcmp(cursor->last_name, lastname) == 0){
			if(strcmp(cursor->first_name, firstname) != 0){
				cursor = cursor->next;
			}else if(strcmp(cursor->first_name, firstname) == 0){
				system("clear");
				
				printf
				("The searched user \"%s %s\" has %d assignment/s:\n\n",
				cursor->first_name, cursor->last_name, 
				cursor->num_assignments);

				printf("Assignment Number\tName\t\tGrade %\n\n");
				for(loop = 0 ; loop < cursor->num_assignments ; loop++){
					printf("\t%d\t\t%s\t\t%.2lf\n", loop+1, 
						cursor->assignments[loop].name, 
						cursor->assignments[loop].value);
				}
				printf("\n\n");
				pause();
				return 0;
			}	
		}	
	}		
}

/*This function prints the nth student*/
int print_nth(struct student *head){

	struct student *cursor = head;	//cursor struct
	int nth;			//for user input
	int loop;			//for looping
	
	system("clear");		//clears screen

	/*asks which number the user would like to print*/
	printf("Which number in the list would you like to print?\n");
	printf("Lower numbers are last names closer to a...\n\n");
	printf("Please enter a number: ");
	scanf("%d", &nth);

	/*moves cursor to that location*/
	for(loop = 0 ; loop < (nth-1) ; loop++){
		cursor = cursor->next;
	}

	/*prints out that student*/
	system("clear");
	printf("\"%s %s\" has %d assignment/s and is user #%d:\n\n", cursor->first_name, 
		cursor->last_name, cursor->num_assignments, nth);
	
	printf("Assignment Number\tName\t\tGrade %\n\n");
	for(loop = 0 ; loop < cursor->num_assignments ; loop++){
		printf("\t%d\t\t%s\t\t%.2lf\n", loop+1, cursor->assignments[loop].name, 
							cursor->assignments[loop].value);
	}
	printf("\n\n");
	pause();
	
}

/*prints the last item in the gradebook*/
int tail_print(struct student *head){
	int loop;			//for looping
	struct student *cursor = head;	//cursor struct

	system("clear");		//clears the screen
		
	/*moves the cursor to the last position and prints it*/	
	while(1){
		if(cursor->next != NULL){
			cursor = cursor->next;
		}else{
			printf("The tail/last user \"%s %s\" has %d assignment/s:\n\n", 
			cursor->first_name, cursor->last_name, cursor->num_assignments);
			printf("Assignment Number\tName\t\tGrade %\n\n");
			for(loop = 0 ; loop < cursor->num_assignments ; loop++){
				printf("\t%d\t\t%s\t\t%.2lf\n", loop+1, 
				cursor->assignments[loop].name, 
				cursor->assignments[loop].value);
			}
			printf("\n");
			pause();
			return 0;
		}	
	}	
}

/*Prints the entire gradebook*/
int class_print(struct student* head){
	struct student *cursor = head;	//cursor struct
	int loop;			//looping

	system("clear");		//clears the screen

	/*prints a header and all students and assignments in the class*/
	printf("--------- Class Gradebook ----------\n\n");
	while(cursor != NULL){
		printf("%s, %s\t\t",cursor->last_name, cursor->first_name); 	
		for(loop = 0 ; loop < cursor->num_assignments ; loop++){
			printf("%s\t%.2lf\t", cursor->assignments[loop].name, 
			cursor->assignments[loop].value);
		}
		printf("\n");
		cursor = cursor->next;
	}
	printf("\n");
	pause();
}

//prints out a students information after they've been entered manually*/
int print_student(struct student *tmp){
	int loop;		//for looping

	system("clear");	//clears the screen

	/*prints out all user's information and assignments*/
	printf("The user \"%s %s\" has %d assignment/s:\n\n", tmp->first_name, 
					tmp->last_name, tmp->num_assignments);
	printf("Assignment Number\tName\t\tGrade %\n\n");
	for(loop = 0 ; loop < tmp->num_assignments ; loop++){
		printf("\t%d\t\t%s\t\t%.2lf\n", loop+1, tmp->assignments[loop].name, 
							tmp->assignments[loop].value);
	}
	printf("\n");
	pause();
}		

/*prints the student at the beginning of the list*/
int print_head(struct student *head){
	int loop;		//for looping

	system("clear");	//clears the screen

/*prints out that user's information and assignments*/
	printf("The head/first user \"%s %s\" has %d assignment/s:\n\n", head->first_name, 
						head->last_name, head->num_assignments);
	printf("Assignment Number\tName\t\tGrade %\n\n");
	for(loop = 0 ; loop < head->num_assignments ; loop++){
		printf("\t%d\t\t%s\t\t%.2lf\n", loop+1, head->assignments[loop].name, 
							head->assignments[loop].value);
	}
	printf("\n");
	pause();
}

/*This function lets a user add an assignment to a user*/
int update_assignments(struct student *head){
	struct student *cursor = head;				//cursor struct
	char *firstname = (char *) malloc(100 * sizeof(char));	//space for name
	char *lastname = (char *) malloc(100 * sizeof(char));	//space for name
	int loop = 0;						//for looping

	

	system("clear");					//clears the screen

	/*asks the user for the student's name*/
	printf("Please enter student's first name: ");
	scanf("%s", firstname);
	printf("\n");
	printf("Please enter student's last name: ");
	scanf("%s", lastname);
	printf("\n\n");

	/*finds the student and lets the user enter a new assignment and grade*/
	while(1){
		if(strcmp(cursor->last_name, lastname) != 0 ){
			if(cursor->next != NULL){
				cursor = cursor->next;
			}else{
				system("clear");
				printf("No user by that name was found\n");
				pause();
				return 1;
			}	
		}else if(strcmp(cursor->last_name, lastname) == 0){
			if(strcmp(cursor->first_name, firstname) != 0){
				cursor = cursor->next;
			}else if(strcmp(cursor->first_name, firstname) == 0){
				
				cursor->assignments[cursor->num_assignments].name = (
						char *) malloc(100 * sizeof(char));

				printf("Please enter the assignment name with no spaces: ");
				scanf("%s", cursor->assignments[cursor->num_assignments].name);
				printf("\n");

				printf("Please enter the assignment grade: ");
				scanf("%lf", &cursor->assignments[cursor->num_assignments].value);
				printf("\n");

				printf("You added assignment %s with grade %.2lf\n", 
				cursor->assignments[loop].name, cursor->assignments[loop].value);
				
				printf("\n\n");

				cursor->num_assignments = (1 + cursor->num_assignments);
				return 0;
			}	
		}	
	}
}	

/*determines and prints the numer of students in the gradebook*/
int num_students(struct student *head){

	struct student *cursor = head;	//cursor struct
	int numstudents = 0;		//for storing num of students

	/*loops through until null and incriments*/
	while(cursor != NULL){
		cursor = cursor->next;
		numstudents++;
	}

	system("clear");		//clears the screen

	/*prints out the number of students*/
	printf("There are %d students in the class.\n\n", numstudents);
	pause();
	return numstudents;
}

/*this prints the statistics for a specific assignment for all users in teh class*/
int assignment_statistics(struct student *head){
	
	struct student *cursor = head;	//cursor struct
	char *assignmentname;		//for user specification
	int loop;			//for looping
	int length = 0;			//num of values stored
	int numassignments = 0;

	/*determines max num of assignments*/
	while(cursor != NULL){
		cursor = cursor->next;
		numassignments++;
	}
	
	double stats[numassignments];	//makes array for values
		
	system("clear");		//clears the screen

	assignmentname = (char *) malloc(100*sizeof(char));

	/*asks for the assignment name*/
	printf("Please enter the name of the assignment you'd like statistics for: ");
	scanf("%s", assignmentname);

	/*compiles data into array*/
	cursor = head;
	while(cursor != NULL){
		for(loop = 0 ; loop < cursor->num_assignments ; loop++){
			if(strcmp(cursor->assignments[loop].name, assignmentname) == 0){
				stats[length] = cursor->assignments[loop].value;
				length++;
			}
		}
		cursor = cursor->next;
	}
	
	if(length == 0){
		system("clear");
		printf("There was no assignment by that name...");
		system("sleep 6");
	}
	
	/*prints out statistics after sending data and length tho those functions*/
	system("clear");
	printf("The mean for assignment \"%s\" is \"%.2lf%\".\n", assignmentname, 
								mean(stats, length)); 
	printf("The median for assignement\"%s\" is \"%.2lf%\".\n", assignmentname, 
								median(stats, length));
	printf("The standard deviation for assignment \"%s\" is \"%.2lf%\".\n", 
						assignmentname, stddev(stats, length));

	free(assignmentname);
	pause();
}

/*prints out statistics for a specifc user on all assignments*/
int student_statistics(struct student *head){
	struct student *cursor = head;				//cursor struct
	char *firstname = (char *) malloc(100 * sizeof(char));	//space for name
	char *lastname = (char *) malloc(100 * sizeof(char));	//space for name
	int loop;						//for loops
	int length = 0;						//for # of data
	double stats[50];					//for holding data

	system("clear");					//clears the scren

	/*asks for the student's name*/
	printf("Please enter student's first name: ");
	scanf("%s", firstname);
	printf("\n");
	printf("Please enter student's last name: ");
	scanf("%s", lastname);
	printf("\n\n");
	
	/*finds the student, compiles data, and prints out statistics*/
	while(1){
		if(strcmp(cursor->last_name, lastname) != 0 ){
			if(cursor->next != NULL){
				cursor = cursor->next;
			}else{
				system("clear");
				printf("No user by that name was found\n");
				pause();
				return 1;
			}	
		}else if(strcmp(cursor->last_name, lastname) == 0){
			if(strcmp(cursor->first_name, firstname) != 0){
				cursor = cursor->next;
			}else if(strcmp(cursor->first_name, firstname) == 0){
				for(loop = 0 ; loop < cursor->num_assignments ; loop++){
					stats[length] = cursor->assignments[loop].value;
					length++;
				}
				system("clear");
				printf("The mean for student \"%s %s\" is \"%.2lf%\".\n",
						firstname, lastname, mean(stats, length)); 
				printf("The median for student \"%s %s\" is \"%.2lf%\".\n"
					, firstname, lastname, median(stats, length));
				printf("The standard deviation for student \"%s %s\" is "
				, firstname, lastname);
				printf("\"%.2lf%\".\n", stddev(stats, length));
				pause();
				return 0;
			}	
		}	
	}
}
	/* Generic Functions */

/*pauses until user hits enter*/
int pause(void){
	printf("Press enter to return to the menu.");
	getchar();
	while('\n' != getchar()){
		;	//VOID
	}
}	

/* --------------------End perrenc Functions-------------------- */

#endif
