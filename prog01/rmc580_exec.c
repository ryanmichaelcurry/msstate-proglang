/*
    Author: Ryan Curry
    NetID: rmc580
    Date: 1/31/2023
*/

#include <unistd.h>

int main(int argc, char **argv) 
{
    char* mycmd[] = {"ls", "-l", "/usr", NULL};
    execvp("ls", mycmd);
    return 0;
}