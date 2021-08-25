/**************************************************************************
Student name: Younes Boutaleb
Student number: 041019068
Assignment number (and name if any): 

Lab 05 (Part II): Transaction Processing Program using Random Access Files 
                                                      
Course name and number: CST8234 C Language
Lab section number: 013
Professor’s name: Natalie Gluzman
Due date of assignment: August 13th, 2021
Submission date of assignment: August 10th, 2021
List of source and header files in the project: 
                          
Lab05_Part2.c Lab05_Part2.h
***************************************************************************/

/**************************************************************************
Declare include files
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "Lab05_Part2.h"

/**************************************************************************
Declare struct clientInfo structure
***************************************************************************/
struct clientInfo
{
  int AccNo;
  char lastName[20];
  char firstName[20];
  double accBalance;
};

/**************************************************************************
 * Main function
 *The main function implements a program manage a banking system 
 * the program insert, update, and delete bank account
 * It also saves the data to a text file to be printed  
 * It loops until the user exits
 * Version: 1.0
 * Author/Student Name: Younes Boutaleb
 **************************************************************************/
int main()
{

  /*this is the action chosen by the user from the main menu*/
  int action;

  action = 0;
  while (action != 5)
  {
    /*Display menu*/
    menu();

    /*Prompt for the action to be performed and validates the input */
    action = checkIntegerInput();

    /**************************************************************************
   Define all actions
  ***************************************************************************/

    /*This switch statements executes the chosen action */
    switch (action)
    {

    /*Save the bank accounts information to a text file*/
    case 1:

      textFile();
      break;

    /*Update bank accounts informatio*/
    case 2:
      updateRecord();
      break;

    /*Insert new bank accounts in the system*/
    case 3:
      newRecord();
      break;

    /*Delete bank accounts from the system*/
    case 4:
      deleteRecord();
      break;

    /*Exit the program*/
    case 5:
      puts("Have a nice day!");
      break;

    /*Print error message and Re-prompt for a valid action*/
    default:

      printf("Error: unknown action: %d\nTry again...\n", action);
      break;

    } /*end switch*/

  } /*end while*/

  return EXIT_SUCCESS;

} /*end main*/

/**************************************************************************
  Check for integer inputs
***************************************************************************/

int checkIntegerInput()
{

  /*This is the user input */
  int number;
  /*This the return code */
  int rc = 0;
  /*This the next char after scanf() terminates */
  char nextChar;

  /*Loops untill the user enter a valid input */
  while (rc != 1)
  {
    /*it takes 1 if scanf reads an integer*/
    rc = scanf(" %d", &number);
    /*Read the next character in stdin after scanf is executed*/
    nextChar = getchar();

    /*Checks for clrt-d input which stands for the end of input EOF
    Exits the game with failure if the user enters the EOF character*/
    if (nextChar == -1)
    {
      puts("Program shutdown.\n");
      exit(EXIT_FAILURE);
    } /*end if*/

    /*Prints an error message if the user enters an invalid input (strings/float/negative integer)
    and goes back to the while loop*/
    else if (rc != 1 || nextChar != '\n')
    {
      rc = 0;
      printf("Error - enter an integer number: ");

      /*if not a valid input clear the stdin*/
      if (nextChar != '\n')
      {

        while (getchar() != '\n')
        {
          /*chomp stdin*/
        } /*end while*/
      }   /*end  if*/
    }     /*end else if*/
  }

  /*return valid input*/
  return number;

} /* end checkIntegerInput */

/**************************************************************************
 Read records from "credit.dat" file and and store them as a formated list
 in the "accounts.txt" file
***************************************************************************/
void textFile()
{
  /*Declare two file pointers*/
  FILE *fp1;
  FILE *fp2;
  /*Open "accounts.txt" for write and  "credit.dat" for read
    print error message if the one or both files couldn't be open  */
  if ((fp1 = fopen("accounts.txt", "w")) == NULL || (fp2 = fopen("credit.dat", "r")) == NULL)
  {
    printf("Files could not be opened");
  }/*end  if*/
  /*In case both files was opened successfully*/
  else
  {
    /*Define a clientInfo struct element and initialize its fields*/
    struct clientInfo client = {0, "", "", 0.0};
    /*Display the header of bank accounts listing to the text file*/
    fprintf(fp1, "%-15s %-15s %-15s %-15s\n", "AccNo", "LastName", "FirstName", "AccBalance");

    /*Iterate the records of "credit .dat" file*/
    while (fread(&client, sizeof(struct clientInfo), 1, fp2))
    {
      /*save non-empty records to the text file in a formatted way to create a list*/
      if (client.AccNo != 0)
      {
        fprintf(fp1, "%-15d %-15s %-15s %-15.2f\n", client.AccNo, client.lastName, client.firstName, client.accBalance);
      }/*end  if*/
    }/*end  while*/

    /*close both files*/
    fclose(fp1);
    fclose(fp2);

  } /*end else*/

} /*end textFile*/

/**************************************************************************
   Update only existing records
***************************************************************************/

void updateRecord()
{
  /*Declare two file pointer*/
  FILE *fp2;
  /*This is the account number entered by user*/
  int accountNumb = 0;
  /*This is the transaction amount entered by the user */
  double balance;

  /*Open "credit.dat" file in update mode
    print error message if the file couldn't be open  */
  if ((fp2 = fopen("credit.dat", "rb+")) == NULL)
  {
    printf("Files could not be opened\n");
  }/*end  if*/
  /*In case the files was opened successfully*/
  else
  {
    /*Define a clientInfo struct element and initialize its fields*/
    struct clientInfo client = {0, "", "", 0.0};
    /*this the return code*/
    int rc = 0;

    /*Promt for valid account number and validate input*/
    while (accountNumb > 100 || accountNumb < 1)
    {

      puts("Enter account number (1-100): ");
      accountNumb = checkIntegerInput();
    }/*end while*/

    /*move the file pointer to the position of the holding the entered bank account*/
    fseek(fp2, sizeof(struct clientInfo) * (accountNumb - 1), 0);
    /*read the bank account information*/
    fread(&client, sizeof(struct clientInfo), 1, fp2);
    /*Display error message if the record is empty*/
    if (client.AccNo == 0)
    {

      puts("The record is empty\n");
    }/*end if*/
    else
    {
      /*Print the account info to the console*/
      printf("%-15d %-15s %-15s %-15.2f\n", client.AccNo, client.lastName, client.firstName, client.accBalance);

      /*Prompt for the transaction amount and validate input*/
      while (rc != 1)
      {

        puts("Enter charge (+) or payment (-): ");
        rc = scanf("%lf", &balance);

        /*Shutdown the program if the user enters the EOF charachter*/
        if (rc == -1)
        {
          puts("Program terminated.");
          exit(EXIT_FAILURE);
        } /*end if*/

        while (getchar() != '\n')
        {
          /*chomp stdin*/
        } /*end while*/
      }/*end while*/

      /*Add the entered amount to the account balance*/
      client.accBalance += balance;
      /*Print the updated info to the console*/
      printf("%-15d %-15s %-15s %-15.2f\n", client.AccNo, client.lastName, client.firstName, client.accBalance);
      /*move the file pointer to the beginning of the file*/
      rewind(fp2);
      /*move the file pointer to the position of the holding the entered bank account*/
      fseek(fp2, sizeof(struct clientInfo) * (accountNumb - 1), 0);
      /*save the updated info to the system*/
      fwrite(&client, sizeof(struct clientInfo), 1, fp2);
    }/*end else*/

    /*Close the file*/
    fclose(fp2);
  }/*end else*/

} /*end updateRecord*/

/**************************************************************************
  Insert a new record in the system
***************************************************************************/

void newRecord()
{
  /*Declare two file pointer*/
  FILE *fp2;
  /*This is the account number entered by user*/
  int accountNumb;

    /*Open "credit.dat" file for update
    print error message if the file couldn't be open  */
  if ((fp2 = fopen("credit.dat", "rb+")) == NULL)
  {
    printf("Files could not be opened\n");
  }/*end  if*/
  else
  {
    /*Define a clientInfo struct element and initialize its fields*/
    struct clientInfo client = {0, "", "", 0.0};
    /*this the return code*/
    int rc = 0;
    /*this is the accont information enetered by the user in one time*/
    char input[30];
    /*Promt for valid account number and validate input*/
    while (accountNumb > 100 || accountNumb < 1)
    {

      puts("Enter account number (1-100): ");
      accountNumb = checkIntegerInput();
    }/*end while*/
    /*move the file pointer to the position of the holding the entered bank account*/
    fseek(fp2, sizeof(struct clientInfo) * (accountNumb - 1), 0);
    /*read the bank account information*/
    fread(&client, sizeof(struct clientInfo), 1, fp2);
    /*Display error message if the record exists in the system*/
    if (client.AccNo != 0)
    {

      puts("Record already contains information\n");
    }/*end if*/
    else
    {
      client.AccNo = accountNumb;

      while (rc != 3)
      {
        /*Prompt for account information */
        puts("Enter lastName, firstName, balance: ");

        rc = scanf(" %[^\n]s", input);

        /*Shutdown the program if the user enters the EOF charachter*/
        if (rc == -1)
        {
          puts("Program terminated.");
          exit(EXIT_FAILURE);
        } /*end if*/
        /*Parse and validate user input to get account details*/
        rc = sscanf(input, " %s %s %lf", client.lastName, client.firstName, &client.accBalance);

        while (getchar() != '\n')
        {
          /*chomp stdin*/
        } /*end while*/

      } /*end while*/
      /*move the file pointer to the beginning of the file*/
      rewind(fp2);
      /*move the file pointer to the position of the holding the entered bank account*/
      fseek(fp2, sizeof(struct clientInfo) * (accountNumb - 1), 0);
      /*save the new rocord to the system*/
      fwrite(&client, sizeof(struct clientInfo), 1, fp2);
    }/*end else*/
    /*Close the file*/
    fclose(fp2);
  }/*end else*/

} /*end newRecord*/

/**************************************************************************
  Delete existing records in the system
***************************************************************************/

void deleteRecord()
{
  /*Declare two file pointer*/
  FILE *fp2;
  /*This is the account number entered by user*/
  int accountNumb = 0;

  /*Open "credit.dat" file in update mode
    print error message if the file couldn't be open  */
  if ((fp2 = fopen("credit.dat", "rb+")) == NULL)
  {
    printf("Files could not be opened\n");
  }/*end  if*/
  /*In case the files was opened successfully*/
  else
  {
    /*Define a clientInfo struct element and initialize its fields*/
    struct clientInfo client = {0, "", "", 0.0};
    /*Promt for valid account number and validate input*/
    while (accountNumb > 100 || accountNumb < 1)
    {

      puts("Enter account number (1-100): ");
      accountNumb = checkIntegerInput();
    }/*end while*/
    /*move the file pointer to the position of the holding the entered bank account*/
    fseek(fp2, sizeof(struct clientInfo) * (accountNumb - 1), 0);
    /*read the bank account information*/
    fread(&client, sizeof(struct clientInfo), 1, fp2);
    /*Display error message if the record is empty*/
    if (client.AccNo == 0)
    {

      puts("Account does not exist.\n");
    }/*end if*/
    else
    {
      /*move the file pointer to the beginning of the file*/
      rewind(fp2);
      /*move the file pointer to the position of the holding the entered bank account*/
      fseek(fp2, sizeof(struct clientInfo) * (accountNumb - 1), 0);
      /*Define a clientInfo struct element as empty element*/
      struct clientInfo client = {0, "", "", 0.0};
      /*Overwrite the account bank info with the empty element*/
      fwrite(&client, sizeof(struct clientInfo), 1, fp2);
    }/*end else*/
    /*Close the file*/
    fclose(fp2);
  }/*end else*/

} /*end newRecord*/

/**************************************************************************
   Display menu of actions
***************************************************************************/

void menu()
{
  printf("Please choose one of the following actions:\n");
  printf("1- Write record to a text file as a formatted list\n");
  printf("2- Update records\n");
  printf("3- Insert a new record\n");
  printf("4- Delete a record\n");
  printf("5- Quit\n");
  printf("Please enter action number: \n");
} /* end menu() */
