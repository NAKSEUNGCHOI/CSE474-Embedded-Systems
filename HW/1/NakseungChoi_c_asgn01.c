/*c_asgn01.c
 *
 *      EE474 introductory C programming assignment #1
 *      Spring 2021
 *
 *     Student Name: Nakseung Choi and ID# 1572578
 *
 */

/*  Objectives:
       Gain experience and proficiency with C programming.


Format of assignment:
  1)  make a backup copy of this file
  2)  enter your name into the space above
  3)  Read through the comments below and
edit code into this file to solve the problems.
  4)  check your results against posted solution

Notes:   You are not required to use #defines for all constants
in this assignment but please do so for problem 3.

Printing:   Use the following functions to print unless
directed not to do so:

  print_int(int x)                prints out an integer
  print_usi(unsigned int x)       prints out an unsigned int
  print_newl()                    prints a "newline" character
  print_str(char *x)                 prints a string (pointed to by x)
  print_dble(double)              prints out a double precision floating point number

These functions are at the bottom of this file.  Feel free to study
them to see how they work.
*/
/*  #include some system library function prototypes */
      //  we will use printf()
#include <stdio.h>
//  also get math function
#include <math.h>
#include <string.h>    // for strlen (ONLY)
#include <stdlib.h>    // for random()


#define LINUX            1
//#define VISUALC          1

#define         CARD_ERROR      (char) 0xFF  //  a value that cannot be a card
#define         N_DECK          52           // standard # of cards in a deck
// data types
typedef int           shuffle;
//typedef unsigned char hand[7];


/*  function prototypes for simplified printing functions */
void print_int(int);
void print_usi(unsigned int);
void print_newl();
void print_str(char*);
void print_chr(char);
void print_dble(double);
char * length_pad(char *string, int n);


// solution functions

/**********************************************************************
                 All variable declarations
**********************************************************************/

int i,j,k=0;      // these make good loop iterators.
int card, suit;

//  Random number seed.
//      try  43227 (player 1 has 4 10s)
int seed = 43227;

// Part 1.1
int d;

// Part 1.2
int t1, t2;


// Part 2.0

shuffle cards[N_DECK][2];

//  Part 2.2

unsigned char testhand[7]={0};


//Part 3.0

// Part 3.1


// Part 3.2

int dealer_deck_count = 0;  // how many cards have been dealt from a deck


// Part 3.3


/***********************************************************************
                      Function prototypes
**************************************************************************/
char* length_pad(char*, int);     // prob 1.4
int randN(int);                   // 2.1
void fill(shuffle[N_DECK][2]);             // 2.1
unsigned char convert(int card, int suit);  // 2.2
int valid_card(unsigned char card); // 2.2
int gcard(unsigned char card);   // 2.2
int gsuit(unsigned char card);   // 2.2
void names(int n, int j, char *answer);          // 2.3
void deal(int M, unsigned char hand[7], shuffle deck[N_DECK][2]);    // 3.2
void printhand(int M, unsigned char* hand, char* buff1);

int pairs(int M, unsigned char hand[]);
int trip_s(int M, unsigned char hand[]);
int fourk(int M, unsigned char hand[]);


/**********************************************************************
                          main()
***********************************************************************/
int main()
{
/************************************************************************
                    Start code here                                     */

/************************************************************************
 *
 * PROBLEM 1,  C-Basics
 *
 */

// initialize random numbers.
//  change the seed for testing.  Use this value to compare results with
#ifdef VISUALC
srand(seed);
#endif
#ifdef LINUX
// srandom(seed);
#endif

print_str("Random seed: "); print_int(seed); print_newl();


// 1.1  Write a code to print out the 10 integers between 1 and 10 on separate lines:
print_str("Problem 1.1 Test Results: \n");

//  1.1 test code here ...

for (d = 1; d < 11; ++d)
{
    printf("%d \n", d);
}


// 1.2 Modify 1.1 to print (after the 10 ints):
//   a) sum of the ints.  b) sum of the ints squared.

print_str("Problem 1.2 Test Results: \n");

//  1.2 code here ...

t1 = 0 ; t2 = 0;

for (d = 1; d < 11; ++d)
{
    printf("%d \n", d);
    t1 = t1 + d;
    t2 = t2 + (d * d);
}

//printf("TST: t1-%d\n", t1);

print_int(t1) ;  print_int(t2) ;   print_newl();


print_str("Problem 1.3 Test Results:\n");

// you code for 1.3 here ...
char *l1 = "sum: ";
char *l2 = "sum of squares: ";

print_str(l1) ; print_int(t1) ; print_newl();
print_str(l2) ; print_int(t2) ; print_newl();


//  1.4  Write a fcn, char* length_pad(char *st, int n), which takes a string and adds
//       the right amount of spaces so that it's length is n.  It should return a pointer
//       the new string.  If the length of st is < n, use malloc() to get memory for a new,
//       longer string.
//

//  Add your length_pad function below main.

//   Modify 1.3 to use length_pad() so that the numbers are all starting in col 21
//      (i.e. all labels have length 20).

print_str("Problem 1.4 Test Results: \n");

print_str(length_pad(l1,20)) ; print_int(t1) ; print_newl();
print_str(length_pad(l2,20)) ; print_int(t2) ; print_newl();





/************************************************************************
 * 2.0   Card games
 *
 *  A 'shuffle' is an array of N_DECK pairs of integers.   The first of the pair is the card type
(0-13 representing ace, 2, 3, .... King) and the second representing the suit (hearts, diamonds, clubs, spades).   Thus a pair of numbers (1-13)(1-4) describes a unique card in the deck.
 */




// 2.1 Write a function to fill a shuffle with N_DECK random integer pairs, BUT,
//     as with your playing cards, there must be exactly one of each pair in the
//     shuffle.
//    Use your function to print out all the "cards" of the shuffle, 7 cards per line.

print_str("Problem 2.1 Test Results: ");    ; print_newl();

fill(cards);


// print out the random cards.
for(i=0;i<N_DECK;i++) {
    print_str(" [");
    print_int(cards[i][0]); print_str(" "); print_int(cards[i][1]) ;
    print_str("] ");
    if(!((i+1)%7)) print_newl();
    }
print_newl();

// this is an example for pointers
//int x = 1, y = 2, z[10];
//int *ip;
///
//ip = &x;
//print_str("Hey this one: ");
//print_int(ip); // so ip stores the address of x.
//y = *ip;
//print_str("\nHey this one: ");
//print_int(y);
//*ip = 0;
//ip = &z[0];
//print_str(*ip);


// 2.2    A 'hand' is an array of seven unsigned chars.  Each char represents one card.  We use a four bit field in the char for each of the two numbers above:  the four most significant bits [7...4] represent the card number (1-13) and the lower four [3...0] represent the suit.


//        Write functions to:  a)  convert two integers (from  a shuffle for example) into a char as above.
//                                    if the ints are invalid (e.g. convert(294802984,138142) is not a card)
//                                    return CARD_ERROR
//                             b)  test if a char equals a valid integer pair
//                             c)  get the integer suit from a char
//                             d)  get the integer card from a char

//        Both functions a and b must return CARD_ERROR if they get invalid input (such as suit > 4).

//        Write code which verifies these functions work properly and prints results.
//

//

print_str("Problem 2.2  Test Results: ");  print_newl();

//  your code for convert() (2.2.a) below main.
print_str("   2.2.a"); print_newl();
// to test, lets fill a hand up with some cards:
testhand[0] = convert(5,2);
testhand[1] = convert(15,2); // Oops!   15 is invalid card #
testhand[2] = convert(12,1);
testhand[3] = convert(6,3);
testhand[4] = convert(-1,7);  // Hey!  both -1 and 7 are invalid
testhand[5] = convert(10,4);
testhand[6] = convert(3,3);

// Let's check them carefully!
for (i =0; i<7; i++){
    if (testhand[i] == CARD_ERROR) {
        print_str(" check 1 card error: ");
        print_int(i);
        print_newl();
    }
}



// new test hand
testhand[0] = convert(1,1);  // Ace of Hearts
testhand[1] = convert(1,2);  // Ace of Diamonds
testhand[2] = convert(1,3);  // Ace of Clubs
testhand[3] = convert(1,4);  // Ace of Spades
testhand[4] = convert(11,2); // Jack of Diamonds
testhand[5] = convert(12,2); // Queen of Diamonds
testhand[6] = convert(13,4); // King of Spades

int i;
for (i=0; i<7; i++){
    if (testhand[i] == CARD_ERROR) {
        print_str(" check 2 card error: ");
        print_int(i); print_newl();
        exit(0);
    }
}


//  your code for convert() (2.2.a) below main.
print_str("   2.2.b"); print_newl();

unsigned char card1, card2;

card1 = 0xFF;  // an invalid card
card2 = convert(1,4); // ace of spades

if (valid_card(card1) == CARD_ERROR)
    print_str("0xFF is not a valid card.\n");
else print_str("Somethings wrong with 2.2.b\n");
if (valid_card(card2) == CARD_ERROR)
    print_str("Somethings wrong with 2.2.b\n");
else print_str("8-bit version of Ace of Spades is a VALID card.\n");

//  your code for convert() (2.2.a) below main.
print_str("   2.2.c,d"); print_newl();


for(i=0;i<7;i++) {
   int card = gcard(testhand[i]);  //    This part tests 2.2c,d
   int suit = gsuit(testhand[i]);
   print_str("card: "); print_int(card); print_str("   suit: "); print_int(suit);print_newl();
}

/////////////////  Part 3

char buffer[] = "   *empty*       ";
//print_str("buffer: ");
//print_str(buffer[0]);


//  your code for convert() (2.2.a) below main.
print_str("Part 3.1 Test Results:");print_newl();

for(i=0;i<7;i++) {
   card = gcard(testhand[i]);  //    This part tests 2.2c,d
   suit = gsuit(testhand[i]);
   print_str(" >>>>");
   print_int(i); print_str(": "); print_int(card);print_int(suit) ;
   names(card,suit,buffer); // convert card,suit to string name in buffer
   print_str(buffer);
   print_newl();
   }
print_newl();

/*
 *     Test Part 3.2
 */

print_str("Part 3.2 Test Results:");print_newl();
print_str("Test the deal() function\n");
char buff[20]="";
for (i=0;i<3;i++) {
    print_str("\n\n----testing deal: hand: "); print_int(i);print_newl();
    print_str("Deck count: ");print_int(dealer_deck_count); print_newl();
    deal(7,testhand,cards); // see Prob 2.1 for "cards"
    print_str("--------dealt hand: \n");
    printhand(7,testhand, buff);
    print_newl();
    }


/*
 *     Test Part 3.3
 */

print_str("Part 3.3 Test Results:");print_newl();
print_str("Test the finding pokerhands function(s).\n");
unsigned char hands[10][7];   //array of 10 hands
unsigned char ranks[10][3];

dealer_deck_count = 0; // reset to full deck (hands should reappear)

int n_hands = 7;

for (i=0;i<n_hands;i++) {
    deal(7,hands[i],cards);
}
print_str(" I have ");print_int(n_hands); print_str(" new hands\n");
for (i=0;i<n_hands;i++) {
    int pct = pairs(7,hands[i]);
    int trips = trip_s(7,hands[i]);
    int fourk = four_kind(7,hands[i]);
    printf("I found %d pairs in hand %d\n",pct,i);
    printf("I found %d three-of-a-kinds in hand %d\n",trips,i);
    printf("I found %d four-of-a-kinds in hand %d\n",fourk,i);
    printf("Hand %d:\n",i);
    printhand(7,hands[i],buff);
    print_newl();
}
for(i=0; i <n_hands; i++){
    //rank(7, hands[i]);
}

#ifdef VISUALC
getchar();
return 0;
#endif


//****************************************************************************
}  //  END OF MAIN

//  1.4  Write a fcn, char* length_pad(char *st, int n), which takes a string and adds
//       the right amount of spaces so that it's length is n.  It should return a pointer
//       the new string.  If the length of st is < n, use malloc() to get memory for a new,
//       longer string.

// Part 1.4
//  your function length_pad() here...

char * length_pad(char *st, int n)
{
    int len = strlen(st);
    char *new_str = (char *)malloc(n+1);
    for (i = 0; i < n; i++){
        if(i<len){
            new_str[i] = st[i];
        }else{
            new_str[i] = ' ';
        }
    }
    new_str[n] = '\0';
    return new_str;
}


//Part 2.1
// your function fill() here ...
void fill(shuffle deck[N_DECK][2]) {
    card = 13;
    suit = 4;
    int r;
    shuffle temp, temp1;
    for(i = 0; i < N_DECK; i++){
        deck[i][0] = i % card + 1;
        deck[i][1] = i % suit + 1;
    }
    for(i = 0; i < N_DECK; i ++){
        r = rand() % N_DECK;
        //print_int(r);
        temp = deck[i][0];
        temp1 = deck[i][1];
        deck[i][0] = deck[r][0];
        deck[i][1] = deck[r][1];
        deck[r][0] = temp;
        deck[r][1] = temp1;
    }
}


// Part 2.2
//  your code for convert() here

unsigned char convert(int card, int suit)
{
    int result = 0x00;
    if(card > 13 || suit > 4){
        printf("I don't know how to handle card = %d \n", card);
        return CARD_ERROR;
    }
    else
    {
        card = card << 4;
        return result = card|suit;
    }

        return result;
}

// Test if a card byte is a valid card
int valid_card(unsigned char card){
    unsigned char byte = CARD_ERROR;// Read from file
    unsigned char mask = 1; // Bit mask
    unsigned char bits[8];
    int result = 0;


    for (int i = 0; i < 8; i++) {
        bits[i] = (byte >> i) & mask;
    }
    for (int i = 0; i < 8; i++) {
        if((int)bits[i] == 1){
            result = result + pow(2, i);
        }
    }
    int *a = card;
    if(a == result){
        //printf("I understand");
        return CARD_ERROR;
    }else{
        if(a != CARD_ERROR)
            return result;
    }


}
// your code for gsuit and gcard here

int gcard(unsigned char card)
{
    card = card >> 4;
    //print_int(card);
    return card;
}

int gsuit(unsigned char card)
{
    suit = card & 0x0F;
    //print_int(suit);
    return suit;
}



//Part 3.1:
//  Your code for names() here

char * card_names[]={"Ace","2","3","4","5","6","7","8","9","10","Jack","Queen","King"};
char * suit_names[]={"Hearts","Diamonds","Clubs","Spades"};

void names(int card, int suit, char *answer){
    char *p1, *p2, *p4, *p3;
    char *hey = " of ";
    int *a;
    //answer = (char*)malloc(20);
    p1 = card_names[card-1];        // put the string of the card into p1.
    p2 = answer;                    // set answer = p2.
    p3 = hey;                       // put " of " into p3.
    p4 = suit_names[suit-1];        // put the string of the suit into p4.
    //strncpy(answer, card_names[card-1], len);
    //print_str(answer);
    //answer = card_names[card-1];
    //print_str(p1);
    while(*p1 != '\0'){
        *p2 = *p1;
        p1++;
        p2++;
    }
    while(*p3 != '\0'){
        *p2 = *p3;
        p3++;
        p2++;
    }
    while(*p4 != '\0'){
        *p2 = *p4;
        p4++;
        p2++;
    }
    *p2 = '\0'; //set the last one as Null.
}

void deal(int M, unsigned char hand[7], int deck[N_DECK][2]){
    int *card_location;
    int *suit_location;
    for(i = 0; i < M; i++){
        //r = rand() % N_DECK;
        card_location = &deck[dealer_deck_count][0];
        suit_location = &deck[dealer_deck_count][1];
        hand[i] = convert(deck[dealer_deck_count][0], deck[dealer_deck_count][1]);
        dealer_deck_count++;
    }
    print_int(dealer_deck_count); print_newl();
}


/*
 * Print out a hand of cards
 */
void printhand(int M, unsigned char* hand, char* buff1){
    int *card_location;
    int *suit_location;
    //print_int(aaa);
    for(i = 0; i < M; i++){
        card_location = (hand[i] >> 4); // could simply do with gcard or gsuit, but wanted to do this way
        suit_location = hand[i] & 0x0F; // to have another chance to think.
        //print_int(suit_location);
        names(card_location, suit_location, buff1);
        print_str(buff1);
        print_str("\n");
    }
    return buff1;
}

int pairs(int M, unsigned char hand[]){
    int i, key, j, k = 0;
    int duplicate[7];
    int counts[13];
    //       Cited: https://www.geeksforgeeks.org/insertion-sort/ ///
    for (i = 1; i < M; i++) {
        key = hand[i];
        j = i - 1;
        while (j >= 0 && hand[j] > key) {
            hand[j + 1] = hand[j];
            j = j - 1;
        }
        hand[j + 1] = key;
        //print_int(hand[i]);
    }
    ////////////////////////////////////////////////////////////////
    for(i = 0; i < 14; i++){
        counts[i] = 0;;
    }
    for(i = 0; i < M; i++){
        duplicate[i] = gcard(hand[i]);
        counts[duplicate[i]] += 1;
    }
    for(i = 0; i < 14; i++){
        if(counts[i] > 1){
            k++;
        }
    }
    return k;

}

int trip_s(int M, unsigned char hand[]){
    int i, key, j, k = 0;
    int duplicate[7];
    int counts[13];
    //       Cited: https://www.geeksforgeeks.org/insertion-sort/ ///
    for (i = 1; i < M; i++) {
        key = hand[i];
        j = i - 1;
        while (j >= 0 && hand[j] > key) {
            hand[j + 1] = hand[j];
            j = j - 1;
        }
        hand[j + 1] = key;
        //print_int(hand[i]);
    }
    ////////////////////////////////////////////////////////////////
    for(i = 0; i < 14; i++){
        counts[i] = 0;;
    }
    for(i = 0; i < M; i++){
        duplicate[i] = gcard(hand[i]);
        //print_int(duplicate[i]);
        counts[duplicate[i]] += 1;
    }
    for(i = 0; i < 14; i++){
        if(counts[i] > 2){
            k++;
        }
    }
    return k;
}

int four_kind(int M, unsigned char hand[]){
    int i, key, j, k = 0;
    int duplicate[7];
    int counts[13];
    //       Cited: https://www.geeksforgeeks.org/insertion-sort/ ///
    for (i = 1; i < M; i++) {
        key = hand[i];
        j = i - 1;
        while (j >= 0 && hand[j] > key) {
            hand[j + 1] = hand[j];
            j = j - 1;
        }
        hand[j + 1] = key;
        //print_int(hand[i]);
    }
    ////////////////////////////////////////////////////////////////
    for(i = 0; i < 14; i++){
        counts[i] = 0;;
    }
    for(i = 0; i < M; i++){
        duplicate[i] = gcard(hand[i]);
        //print_int(duplicate[i]);
        counts[duplicate[i]] += 1;
    }
    for(i = 0; i < 14; i++){
        if(counts[i] > 3){
            k++;
        }
    }
    return k;
}

/**************************************************************************
  simplified I/O and other functions for this assignment
***************************************************************************/
void print_int(int x)
  {
  printf(" %d ",x);
  }
void print_usi(unsigned int x)
  {
  printf(" %d",x);
  }
void print_dble(double x)
  {
  printf(" %f",x);
  }
void print_newl()
  {
  printf("\n");
  }
void print_str(char *x)
  {
  printf("%s",x);
  }
void print_chr(char x)
  {
  printf("%c",x);
  }

int randN(int n)   //  return a random integer between 1 and n
  {
  double x;
  x = 1.0 + (double) n * rand() / RAND_MAX;
  return((int)x);
  }

