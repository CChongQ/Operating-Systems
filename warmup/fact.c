#include "common.h"
#include <ctype.h>
#include <stdbool.h>

int factorial(int num){
    if(num==1){
        return num;
    } else{
        return num*factorial(num-1);
    }
}

bool isPositiveNumber(char number[])
{
    int i = 0;

    //checking for negative numbers or zero
    if (number[0] == '-' || number[0] == '0'){
        return false;
    }
    for (; number[i] != 0; i++)
    {
        //if (number[i] > '9' || number[i] < '0')
        if (!isdigit(number[i]))
            return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    if (argc > 2){
        printf("Huh? more than one arg");
    }
    else if(!isPositiveNumber(argv[1])){
        printf("Huh?\n");
    }
    else{
        char *ptr;
        long num = strtol(argv[1], &ptr, 10);
        if(num>12){
            printf("Overflow\n");
        } else{
            printf("%d\n", factorial(num));
        }
    }
    return 0;
}
