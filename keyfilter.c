// File: keyfilter.c
// Subject: IZP
// Project: #1
// Author: Andrii Klymenko, FIT VUT
// Login: xklyme00
// Date: 16.7.2023

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_ADDRESS_STR_LENGTH 100
#define MAX_ADDRESS_BUFFER_LENGTH (MAX_ADDRESS_STR_LENGTH + 1) // + '\0'

#define ASCII_TABLE_SIZE 128
#define NON_PRINTABLE_ASCII_CHARS_COUNT 32

// this structure is used for generating virtual keyboard's output
typedef struct {
    // it will be printed if only one address was matched, and it wasn't fully matched
    char first_matched_address[MAX_ADDRESS_BUFFER_LENGTH];
    
    // specifies what characters to enable when multiple matched addresses were found
    // non-printable ascii characters are not included here
    // if(possible_chars[ASCII_CHAR - NON_PRINTABLE_ASCII_CHARS_COUNT]) {enable(ASCII_CHAR);}
    bool possible_chars[ASCII_TABLE_SIZE - NON_PRINTABLE_ASCII_CHARS_COUNT];
    
    // used for determining which type of output to print: Found, Enable or Not found
    int matched_addresses_count;
    
    bool is_fully_matched; // specifies if there was found fully matched address
} Virtual_keyboard_stats;

void strToUpper(char* str);

bool strContainsAlphaChar(char* str);

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
    return isValidArgCount(arg_count) && (arg_count == 0 || (isValidArgLength(arg) && strContainsAlphaChar(arg)));
}

// copies searched address from argv to searched_address (if program was run with argument)
// and makes all its characters upper case if possible
void getSearchedAddress(int arg_count, char** argv, char* searched_address)
{
    if(arg_count != 0)
    {
        strcpy(searched_address, argv[1]);
        strToUpper(searched_address);
    }
}

// checks if string contains at least 1 alphabetic character
bool strContainsAlphaChar(char* str)
{
    for(int i = 0; str[i] != '\0'; ++i)
    {
        if(isalpha(str[i]))
        {
            return true;
        }
    }

    return false;
}

// checks if string address contains a valid address, address contains a value returned by fgets()
bool isValidInputAddress(char* address)
{
    int address_str_length = strlen(address);
    
    if(!strContainsAlphaChar(address) || (address_str_length == 101 && address[address_str_length - 1] != '\n'))
    {
        return false;
    }
    
    address[address_str_length - 1] = '\0'; // we don't need '\n'
    return true;
}

// tries to convert all string characters to upper case if possible
void strToUpper(char* str)
{
    for(int i = 0; str[i] != '\0'; ++i)
    {
        str[i] = toupper(str[i]);
    }
}

// checks if searched address and address_from_database are the same (i.e. fully matched) based on searched_address_str_length
// this function is called when at least partial match is found
bool isFullyMatchedAddress(char* address_from_database, int searched_address_str_length)
{
    return address_from_database[searched_address_str_length - 1] != '\0' &&
           address_from_database[searched_address_str_length] == '\0';
}

// program's output when multiple matched addresses were found
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

// updates Virtual_keyboard_stats structure's data
void updateVirtualKeyboardStats(Virtual_keyboard_stats* stats, char* current_address, char* searched_address, int searched_address_str_length)
{
    if(strncmp(searched_address, current_address, searched_address_str_length) == 0) // check for at least partial match
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

void printNotFound()
{
    printf("Not found\n");
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
        printNotFound();
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