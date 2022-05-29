#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main()
{
    int age = 32;
    char name[] = "Nakseung Choi"; //
    //printf("%c\n", name[0]); // print a letter, N.
    float weight = 68.5;
    bool isMarried = true; // true == 1. false == 0.
    char gender = 'M'; // Make sure it is '' not "".
    static int agee; // 0
    int hex = 0x1A; // 0001 1010 // 10 + 16 = 26
    int octal = 016; // 6*1 + 1*8 = 14;
    int x = -1;
    unsigned int x1 = x;
    printf("HoHoHo: %u\n", x1);
    printf("My age is %d.\n", age);
    printf("My name is %s.\n", name);
    printf("My age is %.2f.\n", weight);
    printf("Am I married? %d.\n", isMarried);
    printf("My gender is %c.\n", gender);
    printf("hello: %d\n", agee);
    printf("hello: %d\n", hex);
    printf("Hello: %d\n", octal);
    //scanf(agee);
    //printf("%d\n", sizeof(age));
    //printf("%d\n", (age & 0b11111111));
    //printf("%d", (age >> 2));
    hello(age, name);
    //0000 0000
    //003216 8421
    //0010 0000
    //1000 0000
    int a1, a2;
    printf("Enter first number: \n");
    scanf("%d", &a1); // scanf wants you to provide an address of the variable.
    printf("%d", a1); // print a1.
    return 0;
}
void hello(int age, char name[]){
    int result = age;
    //char names[] = name;
    printf("My age is %d.\n", result);
    printf("My name is %s.\n", name);
    //printf("%c\n", name);
}
