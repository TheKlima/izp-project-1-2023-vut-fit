#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_ADDRESS_STR_LENGTH 100
#define MAX_ADDRESS_BUFFER_LENGTH (MAX_ADDRESS_STR_LENGTH + 1) // + '\0'

#define MAX_INPUT_ADDRESSES_COUNT 1000

bool isValidArgCount(int arg_count)
{
    return arg_count <= 1;
}

bool isValidArgLength(char* arg)
{
    return strlen(arg) <= MAX_ADDRESS_STR_LENGTH;
}

bool isValidArg(int arg_count, char* arg)
{
    return isValidArgCount(arg_count) && (arg_count == 0 || isValidArgLength(arg));
}

bool isValidInputAddress(char* address)
{
    bool contains_alpha_char = false;
    int address_str_length = strlen(address);
    
    for(int i = 0; i < address_str_length; ++i)
    {
        if(isalpha(address[i]))
        {
            contains_alpha_char = true;
            break;
        }
    }
    
    if(!contains_alpha_char || (address_str_length == 101 && address[address_str_length - 1] != '\n'))
    {
        return false;
    }
    
    address[address_str_length - 1] = '\0';
    return true;
}

bool parseInputAddresses(char input_addresses[][MAX_ADDRESS_BUFFER_LENGTH + 1], int* input_addresses_count)
{
    while(fgets(input_addresses[*input_addresses_count], MAX_ADDRESS_BUFFER_LENGTH + 1, stdin) != NULL)
    {
        ++(*input_addresses_count);
    }
    
    return true;
}

int main(int argc, char** argv)
{
    if(!isValidArg(argc - 1, argv[1]))
    {
        fprintf(stderr, "Error! Wrong number of program arguments (expected 0 or 1) or invalid length of first argument.\n");
        return EXIT_FAILURE;
    }
    
    char searched_address[MAX_ADDRESS_BUFFER_LENGTH] = {'\0', };
    if(argc - 1 != 0)
    {
        strcpy(searched_address, argv[1]);
    }
    
    char input_addresses[MAX_INPUT_ADDRESSES_COUNT][MAX_ADDRESS_BUFFER_LENGTH + 1]; // + '\n'
    int input_addresses_count = 0;
    
    if(!parseInputAddresses(input_addresses, &input_addresses_count))
    {
        // TODO print
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}