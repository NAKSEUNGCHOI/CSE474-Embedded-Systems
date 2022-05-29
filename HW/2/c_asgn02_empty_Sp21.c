/* 
 *
 *      ECE474 introductory C programming assignment  #2
 *      Spring 2021
 *
 *     Student Name: ___STUDENT_NAME___
 *
 */

/*  Objectives:
       Gain experience and proficiency with C programming.


Format of assignment:
  1)  make a backup copy of this file
  2)  enter your name and ID# into the space above
  3)  Read through the comments below and
edit code into this file to solve the problems.
  4)  Run the tests in main();
  4)  check your results against posted solution
  
#define constants are NOT required for this assignment but MAY be used.

Printing:  make your own "printf()" statements in this assignment.
 

*/
/*  #include some system library function prototypes */
      //  we will use printf()
#include <stdio.h>
//  also get math function
#include <math.h>
#include <string.h>    // for strlen
#include <malloc.h>    // for malloc(!)
#include <stdlib.h>    // for random()


/*************************************************************************
   Part 2.1:  Bitwise operations   
*************************************************************************/

/* 
 * The function mangle will take your student ID and alter it in
 * ways that depend on the binary representation of it as an integer.
 * 
 * Specifications: 
 *      1) right shift the ID by 2 positions
 *      2) clear bit 7 (counting from the LSB)
 *      3) complement bit 4
 */

int mangle(int sid){ 
}

// Part 2.1.1  More bit manipulation
/* The function bit_check(int data, int bits_on, int bits_off), will check an int to see if a
 * specific mask of  bits is set AND that another bit mask is clear.  Returns 1 if matches
 * and 0 if not.  Return -1 for the degenerate case of testing for BOTH off and on for any bit
 *
 *  Examples (Using 4-bit #'s for clarity)
 *
 *  int d = binary( 1100)  // pseudocode
 *  int onmask1 =  binary(0011)  // will fail
 *  int offmask1 = binary(0100)  // will fail
 *  int onmask2 =  binary(0100)  // succeed
 *  int offmask2 = binary(0001)  // succeed
 * 
 *  bit_check(d,onmask1,offmask1) --> 0
 *  bit_check(d,onmask1,offmask2) --> -1
 *  bit_check(d,onmask2,offmask2) --> 1 
 *  bit_check(d,offmask1,offmask1) --> -1 // contradictory
 */ 

int  bit_check(int data, int bits_on, int bits_off){ 
}


/*************************************************************************
   Part 2.2:  Basic Pointer declaration and usage   
*************************************************************************/
char a_array[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

/* pmatch()
 * This function will return a pointer to the element in a pre-defined
 * array which matches it's character arg. (accepts capital letters only).
 * If the character cannot be found, return the NULL pointer. * 
 * 
 */
char * pmatch(char c){ 
    }
    
    
    

/*************************************************************************
   Part 2.3:  Pointer Arithmetic 
*************************************************************************/


/* 2.3.1 Next letter:
 * char nlet(char * ptr) 
 * ptr is a pointer returned by pmatch()
 * This function will return the next letter of the alphabet (Not
 * a pointer to the next letter of the alphabet) unless 
 * the pointer points to 'Z'. In that case it will return -1.
 * If the argument does not point to a capital letter A-Y, return -1.
 */

char nlet(char *ptr){ 
}

/*
 * 2.4 Pointer math II
 * 
 * ldif(c1,c2)    Find the alphabet distance between two letters using pointers. 
 * Example:     Ldif('A','E') -> 4
 * 
 * if either character is not a capital letter, return a negative number < -26
 */

int ldif(char c1, char c2){ 
    }

/*************************************************************************
   Part 2.5   struct and sizeof
*************************************************************************/

/*
 * Define a typedef struct  called Person containing:
 *     1) 20 char string: FirstName
 *     2) 30 char string: LastName
 *     3) 80 char string: StreetAddr
 *     4)  5 char string: ZipCode
 *     5)    double: Height  // height in meters
 *     6)    float: Weight  // weight in kg
 *     7) long int: DBirth  // birthday (days since 1-Jan-1900)
 * 
 * Part 2.4.2
 * Return the number of bytes required to hold person struct
 * 
 * Part 2.4.3
 * Write a function: per_print(person * p)  which prints out a formatted person.
 * If the person's address is longer than 60 characters, truncate it to 60 characters
 * but do not erase memory of the last 20 characters if present.
 */

typedef struct Person { 
} Person ;


void per_print(Person *p){
     
}

 
/*************************************************************************
main() run all demos and tests of the parts of assignment 2.

Use these to test your code.  Do not modify the code below.  See sample output file. 
*************************************************************************/


void main(){
    int sid;   // student ID #
   
    // Part 2.1
   printf( "Part 2.1: Enter your UW Student ID number:");
   scanf("%d", &sid);
   printf(" You entered %d\n",sid);
   int msid =  mangle(sid);
   printf(" your mangled SID is %d\n\n", msid);
   
   
   // Part 2.1.2
   printf("Part 2.1.2: Check the bit_check() function\n");
   int onm1 = 3;
   int ofm1 = 4;
   int onm2 = 4;
   int ofm2 = 1; // same as example above
   int d = 0x14; // (1100)
   
   // The examples
   printf("d = %d Hex: %x, Octal: %o\n", d,d,d);
   printf("bit_check(d,onm1,ofm1): %d\n", bit_check(d,onm1,ofm1));  // example 1
   printf("bit_check(d,onm1,ofm2): %d\n", bit_check(d,onm1,ofm2));
   printf("bit_check(d,onm2,ofm2): %d\n", bit_check(d,onm2,ofm2));
   printf("bit_check(d,ofm1,ofm1): %d\n", bit_check(d,ofm1,ofm1));  // example 4
   printf("bit_check(129753, 129753, ~129753): %d\n", bit_check(129753, 129753, ~129753)); // 1

   d = 32768+4096+64;
   printf("d = %d Hex: %x, Octal: %o\n", d,d,d);
   printf("bit_check(32768+4096+64, 4096+64, 16384): %d\n", bit_check(32768+4096+64, 4096+64, 16384)); // 1
   printf("bit_check(32768+4096+64, 4096+64, 32768): %d\n", bit_check(32768+4096+64, 4096+64, 32768)); // 0
   printf("\n");
   
   // Part 2.2
   char ctest;
   printf(" Part 2.2.1: Enter a capital letter: ");
   scanf(" %c", &ctest); 
   char * ptest = pmatch(ctest);
   if (ptest != NULL){
       printf(" you entered: %1c\n", *ptest);
       int nl = nlet(ptest);
       if (nl != -1){
              printf (" Part 2.2.2: The next letter after %1c is %c\n\n", *ptest, nlet(ptest));
              }
   }
   else printf(" you did NOT enter a capital letter!\n\n");
   ctest = 'Z';
   ptest = &ctest;
   printf (" Part 2.2.2: The next letter after %1c is %d\n\n", *ptest, nlet(ptest));
   printf (" Part 2.2.3: M and Q are %d positions apart\n", ldif('M', 'Q'));
   printf (" Part 2.3.3: x and Q are %d positions apart\n\n", ldif('x', 'Q'));

    
   //Part 
   // Part 2.4   Struct and Sizeof
   
   Person testp;
   int persize = sizeof(testp);
   printf ("Part 2.4.1:  Size of a Person is %d\n",persize);
   strcpy(testp.FirstName, "Blake");
   strcpy(testp.LastName, "Hannaford");
   strcpy(testp.StreetAddr, "124 N. Anystreet / Busytown, WA");
   strcpy(testp.ZipCode, "99499");
   testp.Height = 1.97;
   testp.Weight = 180.0/2.2;  // 180 lbs in kg
   // born 24-Mar-1995 (see  https://www.timeanddate.com/date/duration.html)
   testp.DBirth = 34780;  
   
   persize = sizeof(testp);
   printf ("Part 2.4.1:  Size of a Person is STILL %d\n",persize);
   printf ("Part 2.4.2:  Our test person is:\n");
   per_print(&testp);
   
   // Now we move to a place w/ really long address!
   strcpy(testp.StreetAddr,"12903010542039840 NW Antidisetablishmentarianism Street, Apt.5/AnyTown, U.S.A.");
   printf("\n The new Address is 79 characters!  Too long for 60\n");
   per_print(&testp);
   
   
}

