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
    bool is_fully_matched;
} Virtual_keyboard_stats;

void strToUpper(char* str);

bool addressContainsAlphaChar(char* address);

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
    return isValidArgCount(arg_count) && (arg_count == 0 || (isValidArgLength(arg) && addressContainsAlphaChar(arg)));
}

void getSearchedAddress(int arg_count, char** argv, char* searched_address)
{
    if(arg_count != 0)
    {
        strcpy(searched_address, argv[1]);
        strToUpper(searched_address);
    }
}

bool addressContainsAlphaChar(char* address)
{
    for(int i = 0; address[i] != '\0'; ++i)
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
    
    if(!addressContainsAlphaChar(address) || (address_str_length == 101 && address[address_str_length - 1] != '\n'))
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

void updateVirtualKeyboardStats(Virtual_keyboard_stats* stats, char* current_address, char* searched_address, int searched_address_str_length)
{
    if(strncmp(searched_address, current_address, searched_address_str_length) == 0)
    {
        if(!isFullyMatchedAddress(current_address, searched_address_str_length))
        {
            stats->possible_chars[current_address[searched_address_str_length] - NON_PRINTABLE_ASCII_CHARS_COUNT] = true;
        }
        else
        {
            stats->is_fully_matched = true;
        }

        if(stats->matched_addresses_count == 0 && !stats->is_fully_matched)
        {
            strcpy(stats->first_matched_address, current_address);
        }

        ++(stats->matched_addresses_count);
    }
}

void printFoundAddress(char* address)
{
    printf("Found: %s\n", address);
}

void printVirtualKeyboardOutput(Virtual_keyboard_stats* stats, char* searched_address)
{
    if(stats->is_fully_matched)
    {
        printFoundAddress(searched_address);
    }
    
    if(stats->matched_addresses_count > 1)
    {
        printPossibleChars(stats->possible_chars);
    }
    else if(stats->matched_addresses_count == 1)
    {
        if(!stats->is_fully_matched)
        {
            printFoundAddress(stats->first_matched_address);
        }
    }
    else
    {
        printf("Not found\n");
    }
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
            fprintf(stderr, "Error! Invalid input address provided.\n");
            return false;
        }

        strToUpper(current_address);
        updateVirtualKeyboardStats(&stats, current_address, searched_address, searched_address_str_length);
    }

    printVirtualKeyboardOutput(&stats, searched_address);
    return true;
}

int main(int argc, char** argv)
{
    if(!isValidArg(argc - 1, argv[1]))
    {
        fprintf(stderr, "Error! Wrong number of program arguments (expected 0 or 1) or was provided invalid argument.\n");
        return EXIT_FAILURE;
    }

    char searched_address[MAX_ADDRESS_BUFFER_LENGTH] = {'\0', };
    getSearchedAddress(argc - 1, argv, searched_address);
    
    return runVirtualKeyboard(searched_address);
}