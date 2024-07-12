#include <stdbool.h>
#include <string.h>

#define MAX_ADDRESS_LENGTH 100

bool isValidArgCount(int arg_count)
{
    return arg_count <= 1;
}

bool isValidArgLength(char* arg)
{
    return strlen(arg) <= MAX_ADDRESS_LENGTH;
}

bool isValidArg(int arg_count, char* arg)
{
    return isValidArgCount(arg_count) && (arg_count == 0 || isValidArgLength(arg));
}

int main(int argc, char** argv)
{
    return 0;
}