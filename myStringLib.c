#include <stdio.h>
#include <stdlib.h>
#include "myStringLib.h"

int my_strlen(const char * const str1)
{
	
	if (str1 == NULL) {
        return 0; // Empty string
    }

    int length = 0;

    while (str1[length] != '\0') {
        length++;
    }

    return length;
}

int my_strcmp(const char * const str1, const char * const str2)
{

    if (my_strlen(str1) == my_strlen(str2)) {
        int i;
        int Similar = 0;
        for (i = 0; i < my_strlen(str1);i++) {

            if (str1[i] == str2[i]) {
                Similar++;
            }
        }
        if (Similar == my_strlen(str1)) {
            return 1;
        }
        else {
            return 0;
        }
        
    }

    else {
        return 0;
    }
}

int my_strcmpOrder(const char * const str1, const char * const str2)
{
	/*compare_str alphabetically compares two strings.
    	If str2 is alphabetically smaller than str1 (comes before str1),
    	the function returns a 1, else if str1 is smaller than str2, the
    	function returns a 0.*/

	// if two strings are completely identical in order, it returns -1.


    int i;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return (str1[i] > str2[i]) ? 1 : 0;
        }
        i++;
    }

    if (str1[i] == '\0' && str2[i] == '\0') {
        return -1; 
    } else if (str1[i] == '\0') {
        return 0; 
    } else {
        return 1; 
    }
}


char *my_strcat(const char * const str1, const char * const str2){

    char *z = malloc(sizeof(char) * ((my_strlen(str1) + my_strlen(str2)) + 1));

    int i;
    for (i = 0; i < my_strlen(str1); i++) {
        if (str1[i] != '\0') {
            z[i] = str1[i];
        }
    }

    for (i = 0; i < my_strlen(str2); i++) {
        if (str2[i] != '\0') {
            z[i + my_strlen(str1)] = str2[i];
        }
    }

    z[i + my_strlen(str1)] = '\0';
    return z;

	
}