#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_ADDRESS_STR_LENGTH 100
#define MAX_ADDRESS_BUFFER_LENGTH (MAX_ADDRESS_STR_LENGTH + 1) // + '\0'

#define ASCII_TABLE_SIZE 128
#define NON_PRINTABLE_ASCII_CHARS_COUNT 32

typedef struct {
    char first_matched_address[MAX_ADDRESS_BUFFER_LENGTH];
    bool possible_chars[ASCII_TABLE_SIZE - NON_PRINTABLE_ASCII_CHARS_COUNT];
    int matched_addresses_count;
} Virtual_keyboard_stats;

void strToUpper(char* str);

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

void getSearchedAddress(int arg_count, char** argv, char* searched_address)
{
    if(arg_count != 0)
    {
        strcpy(searched_address, argv[1]);
        strToUpper(searched_address);
    }
}

bool addressContainsAlphaChar(char* address, int address_str_length)
{
    for(int i = 0; i < address_str_length; ++i)
    {
        if(isalpha(address[i]))
        {
            return true;
        }
    }

    return false;
}

bool isValidInputAddress(char* address)
{
    int address_str_length = strlen(address);
    
    if(!addressContainsAlphaChar(address, address_str_length) || (address_str_length == 101 && address[address_str_length - 1] != '\n'))
    {
        return false;
    }
    
    address[address_str_length - 1] = '\0';
    return true;
}

void strToUpper(char* str)
{
    for(int i = 0; str[i] != '\0'; ++i)
    {
        str[i] = toupper(str[i]);
    }
}

bool isFullyMatchedAddress(char* address_from_database, int searched_address_str_length)
{
    return address_from_database[searched_address_str_length - 1] != '\0' &&
           address_from_database[searched_address_str_length] == '\0';
}

void printPossibleChars(bool* possible_chars)
{
    printf("Enable: ");
    for(int i = 0; i < ASCII_TABLE_SIZE - NON_PRINTABLE_ASCII_CHARS_COUNT; ++i)
    {
        if(possible_chars[i])
        {
            putchar(i + NON_PRINTABLE_ASCII_CHARS_COUNT);
        }
    }

    putchar('\n');
}

bool runVirtualKeyboard(char* searched_address)
{
    Virtual_keyboard_stats stats = {};

    char current_address[MAX_ADDRESS_BUFFER_LENGTH + 1] = {'\0', };

    int searched_address_str_length = strlen(searched_address);

    while(fgets(current_address, MAX_ADDRESS_BUFFER_LENGTH + 1, stdin) != NULL)
    {
        if(!isValidInputAddress(current_address))
        {
            return false;
        }

        strToUpper(current_address);

        if(strncmp(searched_address, current_address, searched_address_str_length) == 0)
        {
            if(stats.matched_addresses_count == 0)
            {
                strcpy(stats.first_matched_address, current_address);
            }

            if(!isFullyMatchedAddress(current_address, searched_address_str_length))
            {
                stats.possible_chars[current_address[searched_address_str_length] - NON_PRINTABLE_ASCII_CHARS_COUNT] = true;
            }
            else
            {
                printf("Found: %s\n", searched_address);
            }

            ++(stats.matched_addresses_count);
        }
    }
    
    if(stats.matched_addresses_count > 1)
    {
        printPossibleChars(stats.possible_chars);
    }
    else if(stats.matched_addresses_count == 1)
    {
        if(!isFullyMatchedAddress(stats.first_matched_address, searched_address_str_length))
        {
            printf("Found: %s\n", stats.first_matched_address);
        }
    }
    else
    {
        printf("Not found\n");
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
    getSearchedAddress(argc - 1, argv, searched_address);

    if(!runVirtualKeyboard(searched_address))
    {
        fprintf(stderr, "Error! Too many input addresses provided or invalid input address provided.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}