/*
    Author: Ryan Curry
    NetID: rmc580
    Date: 1/31/2023
*/

#include <unistd.h>
#include <stdio.h>

int counter()
{
    // init
    int count = 0;

    if(fork() == 0) // is child
    {
        while(count < 20)
        {
            count++;
            printf("child:%d\n", count);
        }
    }

    else // is parent
    {
        while(count < 20)
        {
            count++;
            printf("parent:%d\n", count);
        }
    }

    return 0;
}

int main(int argc, char **argv) 
{
    counter();
    return 0;
}