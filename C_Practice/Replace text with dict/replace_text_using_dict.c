/*
There are many way to do this , i did it so insted of going over the file each time to find a word i went over it once 
saved all the words in an array , and searched in that array ( i did not save the synonim , just the word to replace)
by that i saved a nice amount of time
*/


#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

#define WRONG_ARGUMENTS (-1)
#define CANNOT_OPEN_SOURCE_FILE (-2)
#define CANNOT_OPEN_DICTIONARY_FILE (-3)
#define ERROR_BAD_FORMAT (-4)
#define BUFFERSIZE 100
#define WORD_SIZE 20
/*get all non alpha chars untill next lapha char*/
char * get_non_alpha(FILE *fp);
/*get word index in the search array*/
int find_word_index(char *wordchar, char *search_index, int word_amount);
/*find how many lines are there in a file*/
const int countLines(FILE *fp);
/*append a char to end of line */
void append(char* s, char c);
/*remove all non abc from word*/
void CleanupWord(char* source);
/*get the synonym at index*/
char* get_synonym(FILE *fp, int index);
/*Init the array with the words from the file*/
int inint_word_search_array(FILE *fp, char *search_array);
/*replace all words with synonyms*/
void replace_words(FILE *fp_sc, FILE *fp_dict, char *search_array);
/*get next word in the file */
int get_next_word(FILE * fp, char **word);
/*gloval var for amount of words in search array*/
int word_amount = 0;

void main(int argc, char *argv[])
{

	//file pointers 
	FILE * fp_sc = NULL;
	FILE * fp_dict = NULL;


	/*check argument count*/
	if (argc != 3) {
		printf("Usage : source_file dict_file\n");
		printf("ERROR - wrong amount of arguments!\n");
		return WRONG_ARGUMENTS;
	}

	fp_sc = fopen(argv[1], "r+");
	if (fp_sc == NULL) {
		printf("ERROR - cannot open source file at %s.\n", argv[1]);
		return CANNOT_OPEN_SOURCE_FILE;
	}

	fp_dict = fopen(argv[2], "r"); // we dont need anything more than read
	if (fp_dict == NULL) {
		fclose(fp_sc);
		printf("ERROR - cannot open dictionary file at %s.\n", argv[3]);
		return CANNOT_OPEN_DICTIONARY_FILE;
	}

	/*dictionary format is  :
	<word> : <synonim>*/
	word_amount = countLines(fp_dict);
	size_t allocation_size = sizeof(char) * WORD_SIZE * word_amount;
	char *word_search_array = malloc(allocation_size);// the word search array will contain all the words that we found in the dict file
	const char *word_search_start = word_search_array;//incase we loose the mem location , we need to free it in the end

	/*init the word search array*/
	if (inint_word_search_array(fp_dict, word_search_start) == ERROR_BAD_FORMAT) {
		return ERROR_BAD_FORMAT;
	}

	char *after_replace = NULL;
	fseek(fp_sc, 0, SEEK_SET);
	printf("Starting to replace words\n".);
	replace_words(fp_sc, fp_dict, word_search_array, &after_replace);
	printf("Finished replacing words\n.");
	freopen(argv[1], "w", fp_sc);
	fputs(after_replace, fp_sc);
	/*free our allocated memory*/
	printf("Finished copying the data to the file.\nFreeing the memory and files.");
    free(after_replace);
	free(word_search_start);
	fclose(fp_dict);
	fclose(fp_sc);
}


/*get all non alpha chars untill next lapha char*/
char * get_non_alpha(FILE *fp) {
	char nonalpha[WORD_SIZE*2] = "";
	char c= fgetc(fp);
	while (!isalpha(c) && !feof(fp)) {
		append(nonalpha, c);
		c = fgetc(fp);
	} 
	if (isalnum(c)) {
		fseek(fp, -1L, SEEK_CUR);
	}
	return nonalpha;
}

/*get word index in the search array*/
int find_word_index(char *word, char *search_index) {
	
	int i = 0;
	
	for (i=0; i < word_amount; i++) {
		if (strcmp(search_index, word) == 0) {
			return i;
		}
		search_index += WORD_SIZE;
	}
	return -1; // not found
}

/*find how many lines are there in a file*/
const int countLines(FILE *fp) {
	char ch;
	int lines = 0;
	while (!feof(fp))
	{
		ch = fgetc(fp);
		if (ch == '\n' || ch == -1)
		{
			lines++;
		}
	}
	fseek(fp, 0, SEEK_SET);//jump back to start of file
	return lines;
}

/*append a char to end of line */
void append(char* s, char c)
{
	int len = strlen(s);
	s[len] = c;
	s[len + 1] = '\0';
}

/*remove all non abc from word*/
void CleanupWord(char* source)
{
	char* i = source;
	char* j = source;
	while (*j != 0)
	{
		*i = *j++;
		if (!isalpha(*i))
			i++;
	}
	*i = 0;
	/*if we have a newLine in end , remove it*/
	char *pos;
	if ((pos = strchr(source, '\n')) != NULL)
		*pos = '\0';
}

/*get the synonym at index*/
char* get_synonym(FILE *fp, int index) {
	/*restart the read , we want to start from 0*/
	fseek(fp, 0, SEEK_SET);
	char line[BUFFERSIZE] = "";
	char word[WORD_SIZE] = "";
	char *doubledotlocation = NULL;
	int i = 0;
	while (fgets(line, BUFFERSIZE, fp) != NULL && i != index)
	{
		++i;
	}
	doubledotlocation = strchr(line, ':');
	++doubledotlocation;
	strncpy(word, doubledotlocation, strlen(doubledotlocation)); //copys all chars after the the ':'
	CleanupWord(word);/*incase there is some wierd stuff around the word*/
	return word;
}

/*Init the array with the words from the file*/
int inint_word_search_array(FILE *fp, char *search_array) {
	char line[BUFFERSIZE] = "";
	char word[WORD_SIZE] = "";
	char *doubledotlocation = NULL;
	while (fgets(line, BUFFERSIZE, fp) != NULL)
	{
		doubledotlocation = strchr(line, ':');
		if (doubledotlocation == NULL) {
			printf("Wrong dict format , please check that your dict follows the rules of :\n");
			printf("<word> : <synonim>");
			return ERROR_BAD_FORMAT;
		}

		*doubledotlocation = '\0';// replace the ':' with \0 so that strlen will do as needed
		CleanupWord(word);

		strncpy(word, line, strlen(line)); //copys all chars up to the ':'
		CleanupWord(word);
		strcpy(search_array, word);
		search_array += WORD_SIZE;
	}


}

/*replace all words with synonyms*/
void replace_words(FILE *fp_sc, FILE *fp_dict, char *search_array , char** after_replace) {
	
	char word[WORD_SIZE] = "";
	int word_location = -1;

	/*get file length*/
	fseek(fp_sc, 0L, SEEK_END);
	size_t file_size = ftell(fp_sc);
	fseek(fp_sc, 0L, SEEK_SET);

	char *whole_file = malloc(file_size*1.5);//in case the synonyms are bigger than origianl word
	strcpy(whole_file, "");
	while (!feof(fp_sc) && get_next_word(fp_sc, word) != NULL){
		if (word != "") {
			word_location = find_word_index(word, search_array, word_amount);
			if (word_location >= 0) {// found one

				strcat(whole_file, get_synonym(fp_dict, word_location)); //place the synonym
			}
			else {
				strcat(whole_file, word); // place the normal word 
			}
		}
		strcat(whole_file, get_non_alpha(fp_sc));
		strcpy(word, ""); // clear the word
	} 

	*after_replace = whole_file;
}

/*get next word in the file */
int get_next_word(FILE *fp, char **word) {
	char c = fgetc(fp);
	do {
		append(word, c);
		c = fgetc(fp);
	} while (isalpha(c) && !feof(fp));
	if (!feof(fp)) {
		fseek(fp, -1L, SEEK_CUR);
		return 1;
	}
}
