// Filename: string2.h
// Action: read, convert, write strings

#include <stdio.h>
#include <string.h>

struct WordArray{
	char name[25][25];
};
//----- FUNCTIONS ------------------------------------------------------

struct WordArray stringParser(const char *input);
bool substring(const char *str, const char *text);
void getSubstring(char *source, char *sub, int posStart, int posEnd);
bool stringContains(const char *str, const char *text);
bool stringIsEmpty(const char *str);
void stringAppend(char *destination, const char *text1,
	const char *text2);
//----------------------------------------------------------------------

struct WordArray stringParser(const char *input) {
	char input2[strlen(input)];
	strcpy(input2, input);
	char *ptr = strtok(input2, " ");
	struct WordArray output;
	
	// detect words
	unsigned wordcount = 0;
	while(ptr != NULL) {
		strcpy(output.name[wordcount], ptr);
		ptr = strtok(NULL, " ");
		wordcount++;
	}
	return output;
}
//----------------------------------------------------------------------

bool substring(const char *str, const char *text) {
	bool result = false;
	
	for(int i=0; i<(int)strlen(text); i++) {
		if(str[i] == text[i]) {
			result = true;
		} else {
			result = false;
			break;
		}
	}
	return result;
}
//----------------------------------------------------------------------

void getSubstring(char *source, char *sub, int posStart, int posEnd) {
    int i = 0;
    while (i < posEnd-posStart-1) {
		sub[i] = source[posStart + 1 + i];
        i++;
    }
    sub[i] = '\0';
}
//----------------------------------------------------------------------

bool stringContains(const char *str, const char *text) {
	if(strlen(str) == 0 || strlen(text) == 0) {
		printf("Zero input !\n");
		return false;
	} else {
		if (strstr(str, text) != NULL) {			
			return true;
		} else {
			return false;
		}
	}
}
//----------------------------------------------------------------------

bool stringIsEmpty(const char *str) {
    char ch;
    do {
		// char is number 0-255 chars, so can be ++, --
		//(*str)++, Increment the character pointed at by str
		// or *(str++), Increment the pointer in str
        ch = *str++;

        // Check non whitespace character
        if (ch != ' ' &&
			ch != '\t' &&
			ch != '\n' &&
			ch != '\r' &&
			ch != '\0')
            return false;

    } while (ch != '\0');

    return true;
}
//----------------------------------------------------------------------

void stringAppend(char *destination, const char *text1,
	const char *text2) {
	sprintf(destination, "%s%s", text1, text2);
}
