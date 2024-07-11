#include <stdbool.h>
#include <string.h>

#define MAX_ADDRESS_LENGTH 100

bool isValidArgc(int argc)
{
    return argc <= 1;
}

bool isValidArgLength(char* arg)
{
    return strlen(arg) <= MAX_ADDRESS_LENGTH;
}

int main()
{
    return 0;
}