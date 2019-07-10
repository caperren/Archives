/*
 * Original Authors: Corwin A. Perren (perrenc) and Will H. Seble (seblew)
 * File: perrenc_hw2.c
 * Created: 2012 March 16, 22:20 by perrenc and seblew
 * Last Modified: 2012 March 16, 22:20 by perrenc and seblew
 *
 * This is the main dot c file for a gradebook. It gives the user a menu and calls 
 * necessary funtions.
 *
 */
 
/* Includes */
#include <stdio.h>
#include <math.h>
#include "perrenc_hw2.h"

int main(int argc, char **argv){
	
	int menuloop = 1; 				//Used to loop the menu
	struct student *head = NULL;			//Creates head node struct

	while(menuloop != 0){
		int menuchoice; 			// Used for selecting a menu item
		
		system("clear"); 			//Clears the screen
		
		/* Prints a pretty menu */
		printf("----------Gradebook Menu----------\n\n");
		printf("Please note: Some functions will require pressing enter ");
		printf("multiple times\nin order to return to this menu.\n\n");
		printf("------------Read Write------------\n");
		printf("1 -- Load Gradebook from File\n");
		printf("2 -- Write Current Gradebook to File\n\n");
		printf("-------View Grades Statistics-----\n");
		printf("3 -- View Entire Gradebook\n");
		printf("4 -- View the First Student's Information/Assignments\n");
		printf("5 -- View the Last Student's Information/Assignments\n");
		printf("6 -- View a Student's Information/Assignments by Name Search\n");
		printf("7 -- View the n'th Student's Information/Assignments\n");
		printf("8 -- View a specific Student's Statistics\n");
		printf("9 -- View a specific  Assignment's Statistics\n\n");
		printf("------------Add Grades------------\n");
		printf("10-- Add Assignment/s & Grade/s to Student\n\n");
		printf("---------Add Remove Students------\n");
		printf("11-- Add a New Student\n");
		printf("12-- Remove a Student\n\n");
		printf("-----------Extra Functions--------\n");
		printf("13-- Display the Number of Students in the Class\n");
		printf("14-- Exit Program\n\n");
		printf("Enter Selection: ");
		scanf("%d", &menuchoice);
		
		/*Allows the user to choose a function to run*/
		switch(menuchoice){
			case 1:
				head = read_from_file(head);
				class_print(head);
				break;
			case 2:
				write_to_file(head);
				break;
			case 3:
				class_print(head);
				break;
			case 4:
				print_head(head);
				break;
			case 5:
				tail_print(head);
				break;
			case 6:
				name_search_print(head);
				break;
			case 7:
				print_nth(head);
				break;
			case 8:
				student_statistics(head);
				break;
			case 9:
				assignment_statistics(head);
				break;
			case 10:
				update_assignments(head);
				break;
			case 11:
				head = add_student(head);
				break;
			case 12:
				head = remove_student(head);
				break;
			case 13:
				num_students(head);
				break;
			case 14:
				menuloop = 0;		//Ends the program
				break;
		}
	}

	printf("\n");		//Prints a newline to make things nice
	system("clear");	//Clears the screen
	return 0;
}
