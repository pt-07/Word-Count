#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


//CHARACTER STRUCTS AND HELPER FUNCTIONS
typedef struct {
    char character; // The ASCII character
    int frequency; // The number of times the character appears
    int firstPosition; // The initial zero-based position the character occurred in
} CharacterInfo;
//WORD STRUCTS AND HELPER FUNCTIONS

typedef struct word{
  char *word;
  int numChars;
  int frequency;
  int firstPosition;
} WordInfo;
typedef struct {
    WordInfo** words;
    int size;
    int capacity;
} WordMap;

WordMap* createWordMap() {
    WordMap* wm = malloc(sizeof(WordMap));
    wm->words = malloc(sizeof(WordInfo*) * 10); // Start with capacity of 10
    wm->size = 0;
    wm->capacity = 10;
    return wm;
}

int compareWords(const void *a, const void *b) {
    WordInfo *wordInfoA = *(WordInfo**)a;
    WordInfo *wordInfoB = *(WordInfo**)b;
    return strcmp(wordInfoA->word, wordInfoB->word);
}

void addWord(WordMap* wm, WordInfo* wi) {
    if (wm->size == wm->capacity) {
        // Resize the array
        wm->capacity *= 2;
        wm->words = realloc(wm->words, sizeof(WordInfo*) * wm->capacity);
    }
    wm->words[wm->size++] = wi;
}

void freeWordMap(WordMap* wm) {
    for (int i = 0; i < wm->size; i++) {
        free(wm->words[i]->word);
        free(wm->words[i]);
    }
    free(wm->words);
    free(wm);
}
//LINE STRUCTS AND HELPER FUNCTIONS
typedef struct {
    char* line;
    int frequency;
    int firstPosition;
} LineInfo;

typedef struct {
    LineInfo** lines;
    int size;
} LineMap;

void addLine(LineMap* lm, LineInfo* li) {
    lm->lines = realloc(lm->lines, (lm->size + 1) * sizeof(LineInfo*));
    lm->lines[lm->size] = li;
    lm->size++;
}

int compareLines(const void* a, const void* b) {
    LineInfo* li1 = *(LineInfo**)a;
    LineInfo* li2 = *(LineInfo**)b;
    return strcmp(li1->line, li2->line);
}





//this function will count the number of unique ASCII characters in the file
void countCharacters(FILE *InputFile, FILE *OutputFile, bool alreadyPrinted){
    rewind(InputFile);
    //count the number of unique ASCII characters in the file
    //return the number of unique ASCII characters in the file
    //check if the output file is null
    //read the input file character by character
    //This flag means you should track each Ascii character 0-127. You should track how many times each character is used and the initial zero based position the character occurred in. You should also track the total number of chars and number of unique chars. When you print the output it should be in the following format, with a line for each character, and should only include characters you actually encountered in the document.
    CharacterInfo characterInfos[128];
    //q: do I need to allocate memory for the characterInfos array?
    //initialize the characterInfos array
    for (int i = 0; i < 128; i++){
        characterInfos[i].character = i;
        characterInfos[i].frequency = 0;
        characterInfos[i].firstPosition = -1;
    }
    //read the input file character by character
    char line [256];
    int totalChars = 0;
    while(fgets(line, sizeof(line), InputFile)){
        //read the line character by character
        for (int i = 0; i < strlen(line); i++){
            //check if the character is a valid ASCII character
            if (line[i] >= 0 && line[i] <= 127){
                //check if the character has been encountered before
                if (characterInfos[(unsigned char)line[i]].frequency == 0){
                    //if the character has not been encountered before, update the characterInfos array
                    characterInfos[(unsigned char)line[i]].frequency = 1;
                    characterInfos[(unsigned char)line[i]].firstPosition = totalChars;
                }
                else{
                    //if the character has been encountered before, update the characterInfos array
                    characterInfos[(unsigned char)line[i]].frequency++;
                }
            }
            totalChars++;
        }
    }
    int uniqueChars = 0;
    for (int i = 0; i < 128; i++){
        if (characterInfos[i].frequency != 0){
            uniqueChars++;
        }
    }
    if (OutputFile == NULL){
        //print to stdout
        if (alreadyPrinted){
            printf("\n");
        }
        //print the total number of characters to stdout
        printf("Total Number of Chars = %d\n", totalChars);
        //print the number of unique characters to stdout
        printf("Total Unique Chars = %d\n\n", uniqueChars);
        //need to print the characters in order of their ASCII value
        for(int i =0; i < 128; i++){
            if (characterInfos[i].frequency > 0){
                printf("Ascii Value: %d, Char: %c, Count: %d, Initial Position: %d\n", i, characterInfos[i].character, characterInfos[i].frequency, characterInfos[i].firstPosition);
            }
        }
        //malloc() SEGFAULT happens here
        //need to print the characters in order of their ASCII value
        //needs to be in this format: Total Number of Chars: <number of chars>
        //Total Unique Chars: <number unique chars>

        //Ascii Value: <ascii int>, Char: <character>, Count: <frequency>, Initial Position: <zero based initial position>\n
        //q: how can I change the format of the for loop below to account for the formatting in the lines above and do I need to sort the array?

       
    }
    else{
        //print to output file
        if (alreadyPrinted){
            fprintf(OutputFile, "\n");
        }
        //print the total number of characters to the output file
        fprintf(OutputFile, "Total Number of Chars = %d\n", totalChars);
        //print the number of unique characters to the output file
        fprintf(OutputFile, "Total Unique Chars = %d\n\n", uniqueChars);
        //need to print the characters in order of their ASCII value
        //needs to be in this format: Total Number of Chars: <number of chars>
        //Total Unique Chars: <number unique chars>

        //Ascii Value: <ascii int>, Char: <character>, Count: <frequency>, Initial Position: <zero based initial position>\n
        for(int i = 0; i < 128; i++){
            if (characterInfos[i].frequency > 0){
                fprintf(OutputFile, "Ascii Value: %d, Char: %c, Count: %d, Initial Position: %d\n", i, characterInfos[i].character, characterInfos[i].frequency, characterInfos[i].firstPosition);
            }
        }
    }
    
}
//this function will count the number of unique words in the file
void countWords(FILE *InputFile, FILE *OutputFile, bool alreadyPrinted){
    rewind(InputFile);
    WordMap* wm = createWordMap();
    char c;
    char word[256];
    int index = 0;
    int totalWords = 0;
    int uniqueWords = 0;


    while((c = fgetc(InputFile)) != EOF){ 
        if (isspace(c)) {
            if (index > 0) {
                word[index] = '\0'; // Null terminate the word
                int found = 0;
                for (int i = 0; i < wm->size; i++) {
                    if (strcmp(wm->words[i]->word, word) == 0) {
                        wm->words[i]->frequency++;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    WordInfo* wi = malloc(sizeof(WordInfo));
                    wi->word = strdup(word); // Copy the word
                    wi->frequency = 1;
                    wi->firstPosition = totalWords;
                    addWord(wm, wi);
                    uniqueWords++;
                }
                totalWords++;
                index = 0; // Reset the word index
            }
        } else {
            word[index++] = c; // Add the character to the word
        }
    }

    // Sort the words
    qsort(wm->words, wm->size, sizeof(WordInfo*), compareWords);
    if (OutputFile == NULL){
        //print to stdout
        if (alreadyPrinted){
            printf("\n");
        }
        printf("Total Number of Words: %d\nTotal Unique Words: %d\n\n", totalWords, uniqueWords);
        for (int i = 0; i < wm->size; i++) {
            printf("Word: %s, Freq: %d, Initial Position: %d\n", wm->words[i]->word, wm->words[i]->frequency, wm->words[i]->firstPosition);
        }
    }
    else{
        if (alreadyPrinted){
            fprintf(OutputFile, "\n");
        }
        fprintf(OutputFile, "Total Number of Words: %d\nTotal Unique Words: %d\n\n", totalWords, uniqueWords);
        for (int i = 0; i < wm->size; i++) {
            fprintf(OutputFile, "Word: %s, Freq: %d, Initial Position: %d\n", wm->words[i]->word, wm->words[i]->frequency, wm->words[i]->firstPosition);
        }
    }
    freeWordMap(wm);
}

//this function will count the number of lines in the file
void countLines(FILE *InputFile, FILE *OutputFile, bool alreadyPrinted){
    rewind(InputFile);
    char line[256];
    int totalLines = 0;
    int uniqueLines = 0;
    LineMap* lm = malloc(sizeof(LineMap));
    lm->lines = NULL;
    lm->size = 0;

    while(fgets(line, sizeof(line), InputFile)){ 
        line[strcspn(line, "\n")] = 0;  // Remove trailing newline
        totalLines++;
        int found = 0;
        for (int i = 0; i < lm->size; i++) {
            if (strcmp(lm->lines[i]->line, line) == 0) {
                lm->lines[i]->frequency++;
                found = 1;
                break;
            }
        }
        if (!found) {
            LineInfo* li = malloc(sizeof(LineInfo));
            li->line = strdup(line);
            li->frequency = 1;
            li->firstPosition = totalLines - 1;
            addLine(lm, li);
            uniqueLines++;
        }
    }

    // Sort the lines in ASCII order
    qsort(lm->lines, lm->size, sizeof(LineInfo*), compareLines);

    // Print the lines
    if (OutputFile == NULL){
        if(alreadyPrinted){
            printf("\n");
        }
        printf("Total Number of Lines: %d\nTotal Unique Lines: %d\n\n", totalLines, uniqueLines);
        for (int i = 0; i < lm->size; i++) {
            printf("Line: %s, Freq: %d, Initial Position: %d\n", lm->lines[i]->line, lm->lines[i]->frequency, lm->lines[i]->firstPosition);
        }
    }
    else{
        if(alreadyPrinted){
            fprintf(OutputFile, "\n");
        }
        fprintf(OutputFile, "Total Number of Lines: %d\nTotal Unique Lines: %d\n\n", totalLines, uniqueLines);
        for (int i = 0; i < lm->size; i++) {
            fprintf(OutputFile, "Line: %s, Freq: %d, Initial Position: %d\n", lm->lines[i]->line, lm->lines[i]->frequency, lm->lines[i]->firstPosition);
        }
    }

    // Don't forget to free the memory
    for (int i = 0; i < lm->size; i++) {
        free(lm->lines[i]->line);
        free(lm->lines[i]);
    }
    free(lm->lines);
    free(lm);
}

//this function will count the longest word in the file
void countLongestWord(FILE *InputFile, FILE *OutputFile, bool alreadyPrinted){
    rewind(InputFile);
    WordMap* wm = createWordMap();

    char line[256];
    int longestWordLength = 0;
    while(fgets(line, sizeof(line), InputFile)){
        char* word = strtok(line, " \n");
        while(word != NULL){
            //check if the word is longer than the longest word
            if (strlen(word) > longestWordLength){
                //clear the word map contents 
                for (int i = 0; i < wm->size; i++) {
                    free(wm->words[i]->word);
                    wm->words[i]->word = NULL; // Set pointer to NULL
                    free(wm->words[i]);
                    wm->words[i] = NULL; // Set pointer to NULL
                }
                free(wm->words);
                wm->words = NULL; // Set pointer to NULL
                wm->words = malloc(wm->capacity * sizeof(WordInfo*));
                wm->size = 0;
                wm->capacity = 10;
                //need to add the word to the word map
                WordInfo* wi = malloc(sizeof(WordInfo));
                wi->word = strdup(word); 
                wi->frequency = 1;
                wi->firstPosition = longestWordLength;
                addWord(wm, wi);       
                longestWordLength = strlen(word);       
                
            }
            //if it is the same length as the longest word, add it to the word map
            else if (strlen(word) == longestWordLength){
                int found = 0;
                for (int i = 0; i < wm->size; i++) {
                    if (strcmp(wm->words[i]->word, word) == 0) {
                        wm->words[i]->frequency++;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    WordInfo* wi = malloc(sizeof(WordInfo));
                    wi->word = strdup(word);
                    wi->frequency = 1;
                    wi->firstPosition = longestWordLength;
                    addWord(wm, wi);
                }
            }
            word = strtok(NULL, " \n");
        }
    }
    //need to sort the words in ASCII order
    qsort(wm->words, wm->size, sizeof(WordInfo*), compareWords);
   if (OutputFile == NULL){
    if (alreadyPrinted){
        printf("\n");
    }
    //print to stdout in format: Longest Word is <length> characters long:\n\t<word 1>\n\t<word 2>\n
        printf("Longest Word is %d characters long:\n", longestWordLength);
        char wordBuffer[256]; // Buffer to hold each word
        for (int i = 0; i < wm->size; i++) {
            sprintf(wordBuffer, "\t%s\n", wm->words[i]->word);
            printf("%s", wordBuffer);
        }
    }
    else{
        if(alreadyPrinted){
            fprintf(OutputFile, "\n");
        }
        //print to output file in format: Longest Word is <length> characters long:\n\t<word 1>\n\t<word 2>\n
        fprintf(OutputFile, "Longest Word is %d characters long:\n", longestWordLength);
        for (int i = 0; i < wm->size; i++) {
            fprintf(OutputFile, "\t%s\n", wm->words[i]->word);
        }
    }
    //free the memory allocated for the word map
    freeWordMap(wm);
}


//this function will count the longest line in the file
void countLongestLine(FILE *InputFile, FILE *OutputFile, bool alreadyPrinted){
    rewind(InputFile);
    char line[256];
    int longestLineLength = 0;
    LineMap* lm = malloc(sizeof(LineMap));
    lm->lines = NULL;
    lm->size = 0;

    while(fgets(line, sizeof(line), InputFile)){
        line[strcspn(line, "\n")] = 0;  // Remove trailing newline
        //check if the line is longer than the longest line
        if (strlen(line) > longestLineLength){
            //clear the line map contents 
            for (int i = 0; i < lm->size; i++) {
                free(lm->lines[i]->line);
                lm->lines[i]->line = NULL; // Set pointer to NULL
                free(lm->lines[i]);
                lm->lines[i] = NULL; // Set pointer to NULL
            }
            free(lm->lines);
            lm->lines = NULL; // Set pointer to NULL
            lm->lines = malloc(sizeof(LineInfo*));
            lm->size = 0;
            //need to add the line to the line map
            LineInfo* li = malloc(sizeof(LineInfo));
            li->line = strdup(line); 
            li->frequency = 1;
            li->firstPosition = longestLineLength;
            addLine(lm, li);       
            longestLineLength = strlen(line);       
            
        }
        //if it is the same length as the longest line, add it to the line map
        else if (strlen(line) == longestLineLength){
            int found = 0;
            for (int i = 0; i < lm->size; i++) {
                if (strcmp(lm->lines[i]->line, line) == 0) {
                    lm->lines[i]->frequency++;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                LineInfo* li = malloc(sizeof(LineInfo));
                li->line = strdup(line);
                li->frequency = 1;
                li->firstPosition = longestLineLength;
                addLine(lm, li);
            }
        }
    }
    //sort the lines in ASCII order
    qsort(lm->lines, lm->size, sizeof(LineInfo*), compareLines);
    //print the lines
    if(OutputFile == NULL){
        if(alreadyPrinted){
            printf("\n");
        }
        //print to stdout in format: Longest Line is <length> characters long:\n\t<line 1>\n\t<line 2>\n
        printf("Longest Line is %d characters long:\n", longestLineLength);
        char lineBuffer[256]; // Buffer to hold each line
        for (int i = 0; i < lm->size; i++) {
            sprintf(lineBuffer, "\t%s\n", lm->lines[i]->line);
            printf("%s", lineBuffer);
        }
    }
    else{
        if(alreadyPrinted){
            fprintf(OutputFile, "\n");
        }
        //print to output file in format: Longest Line is <length> characters long:\n\t<line 1>\n\t<line 2>\n
        fprintf(OutputFile, "Longest Line is %d characters long:\n", longestLineLength);
        for (int i = 0; i < lm->size; i++) {
            fprintf(OutputFile, "\t%s\n", lm->lines[i]->line);
        }
    }
    //free in memory 
    for (int i = 0; i < lm->size; i++) {
        free(lm->lines[i]->line);
        free(lm->lines[i]);
    }
    free(lm->lines);
    free(lm);

}



// //this function will handle each batch file line by checking the flags for errors, we will only print the error and return if there is an error
bool batchFlags(int argc, char *argv[]){
    if (argc< 3){
            printf("USAGE:\n\t./MADCounter -f <input file> -o <output file> -c -w -l -Lw -Ll\n\t\tOR\n\t./MADCounter -B <batch file>\n");
            return false;
    }
    //REDESIGN
    //create an array of flags to check if the flags are valid
    char *flags[8] = {"-f", "-o", "-c", "-w", "-l", "-Lw", "-Ll", "-B"};
    //checking the -f flag first
    bool inputFileSpecified = false;
    int inputFileIndex = 0;
    for (int i = 0; i < argc; i++){
        if (strcmp(argv[i], "-f") == 0){
            inputFileSpecified = true;
            inputFileIndex = i;
        }
    }
    //check if an output file is specified
    bool outputFileSpecified = false;
    int outputFileIndex = 0;
    for (int i = 0; i < argc; i++){
        if (strcmp(argv[i], "-o") == 0){
            outputFileSpecified = true;
            outputFileIndex = i;
        }
    }
    //checking for the batch file
    bool batchFileSpecified = false;
    for (int i = 0; i < argc; i++){
        if (strcmp(argv[i], "-B") == 0){
            batchFileSpecified = true;
        }
    }
    if(!batchFileSpecified && !inputFileSpecified){
        fprintf(stdout, "ERROR: No Input File Provided\n");
        return false;
    }
    //need to check that if there is an output file specified, that the next argument is not a flag
    if(outputFileSpecified && (outputFileIndex + 1 >= argc)){
        fprintf(stdout, "ERROR: No Output File Provided\n");
        return false;
    }
    else if (outputFileSpecified && (outputFileIndex < argc && argv[outputFileIndex + 1][0] == '-')){
        fprintf(stdout, "ERROR: No Output File Provided\n");
        return false;
    }
    else if (inputFileSpecified){
        //check if the next argument is a flag NO Input File Provided - the -f flag is immediately followed by another flag
        for (int i = 0; i < argc; i++){
            if ((strcmp(argv[i], "-f") == 0 && i+1 < argc && argv[i+1][0] == '-') || (strcmp(argv[i], "-f") == 0 && i+1 >= argc)) {
                fprintf(stdout, "ERROR: No Input File Provided\n");
                return false;
            }
        }
        //try to open the file in read mode, if it does not work, Can't Open Input File - If the is an error in opening the input file
        //need to dynamically allocate memory using malloc()
        char filename[100];
        strcpy(filename, argv[inputFileIndex + 1]);
        FILE *file = fopen(filename, "r");
        if (file == NULL){
            fprintf(stdout, "ERROR: Can't open input file\n");
            return false;
        }
        //check if the file is empty
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        if(fileSize == 0) {
            fclose(file);
            fprintf(stdout, "ERROR: Input File Empty\n");
            return false;
        }

    }
     //-B : This flag is placed before the name of the batch file we are analyzing.
    if (strcmp(argv[1], "-B") == 0){
        //try to open the batch file
        FILE *batchFile = fopen(argv[2], "r");
        //check if you can open the batch file 
        if (batchFile == NULL){
            fprintf(stdout, "ERROR: Can't open batch file\n");
            return false;
        }
        //check if the batch file is empty
        fseek(batchFile, 0, SEEK_END);
        long fileSize = ftell(batchFile);
        if(fileSize == 0) {
            fclose(batchFile);
            fprintf(stdout, "ERROR: Batch File Empty\n");
            return false;
        }
    }
    //-f : This flag is placed before the name of the file we are analyzing.
    //check if an input file is specified
    
    //-o : This flag is placed before the name of the file we are writing to.
    //All Flags should begin with a '-' and only flags should begin with this. If there is a flag that we did NOT specify elsewhere in this doc then print this error.
    //check that files, which can only exist after a -f, -B or -o do not begin with a '-'
    for(int i = 1; i < argc; i++){
        //check that after the -f flag, the next argument is a file name
        //unless the argument before is a -f -B or -o flag, this argument should begin with a '-' and should be contained in the flags array
        if (strcmp(argv[i-1], "-f") != 0 && strcmp(argv[i-1], "-B") != 0 && strcmp(argv[i-1], "-o") != 0){
            bool isValidFlag = false;
            for (int j = 0; j < 8; j++) {
                if (strcmp(argv[i], flags[j]) == 0) {
                    isValidFlag = true;
                    break;
                }
            }
            if (argv[i][0] != '-' || !isValidFlag) {
                fprintf(stdout, "ERROR: Invalid Flag Types\n");
                return false;
            }
        }
        else{
            //check that the file does not begin with a '-'
            if(argv[i][0] == '-'){
                fprintf(stdout, "ERROR: Invalid Flag Types\n");
                return false;
            }
        }
    }
    return true;
}
    

//this function will check the flags for errors, we will only print the error and exit if there is an error

void checkFlags(int argc, char *argv[]){
    //Usage error
    if (argc< 3){
            printf("USAGE:\n\t./MADCounter -f <input file> -o <output file> -c -w -l -Lw -Ll\n\t\tOR\n\t./MADCounter -B <batch file>\n");
            exit(1);
    }
    //REDESIGN
    //create an array of flags to check if the flags are valid
    char *flags[8] = {"-f", "-o", "-c", "-w", "-l", "-Lw", "-Ll", "-B"};
    //checking the -f flag first
    bool inputFileSpecified = false;
    int inputFileIndex = 0;
    for (int i = 0; i < argc; i++){
        if (strcmp(argv[i], "-f") == 0){
            inputFileSpecified = true;
            inputFileIndex = i;
        }
    }
    //check if an output file is specified
    bool outputFileSpecified = false;
    int outputFileIndex = 0;
    for (int i = 0; i < argc; i++){
        if (strcmp(argv[i], "-o") == 0){
            outputFileSpecified = true;
            outputFileIndex = i;
        }
    }
    //checking for the batch file
    bool batchFileSpecified = false;
    for (int i = 0; i < argc; i++){
        if (strcmp(argv[i], "-B") == 0){
            batchFileSpecified = true;
        }
    }
    if(!batchFileSpecified && !inputFileSpecified){
        fprintf(stdout, "ERROR: No Input File Provided\n");
        exit(1);
    }
    //need to check that if there is an output file specified, that the next argument is not a flag
    if(outputFileSpecified && (outputFileIndex + 1 >= argc)){
        fprintf(stdout, "ERROR: No Output File Provided\n");
        exit(1);
    }
    else if (outputFileSpecified && (outputFileIndex < argc && argv[outputFileIndex + 1][0] == '-')){
        fprintf(stdout, "ERROR: No Output File Provided\n");
        exit(1);
    }
    else if (inputFileSpecified){
        //check if the next argument is a flag NO Input File Provided - the -f flag is immediately followed by another flag
        for (int i = 0; i < argc; i++){
            if ((strcmp(argv[i], "-f") == 0 && i+1 < argc && argv[i+1][0] == '-') || (strcmp(argv[i], "-f") == 0 && i+1 >= argc)) {
                fprintf(stdout, "ERROR: No Input File Provided\n");
                exit(1);
            }
        }
        //try to open the file in read mode, if it does not work, Can't Open Input File - If the is an error in opening the input file
        //need to dynamically allocate memory using malloc()
        char filename[100];
        strcpy(filename, argv[inputFileIndex + 1]);
        FILE *file = fopen(filename, "r");
        if (file == NULL){
            fprintf(stdout, "ERROR: Can't open input file\n");
            exit(1);
        }
        //check if the file is empty
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        if(fileSize == 0) {
            fclose(file);
            fprintf(stdout, "ERROR: Input File Empty\n");
            exit(1);
        }

    }
     //-B : This flag is placed before the name of the batch file we are analyzing.
    if (strcmp(argv[1], "-B") == 0){
        //try to open the batch file
        FILE *batchFile = fopen(argv[2], "r");
        //check if you can open the batch file 
        if (batchFile == NULL){
            fprintf(stdout, "ERROR: Can't open batch file\n");
            exit(1);
        }
        //check if the batch file is empty
        fseek(batchFile, 0, SEEK_END);
        long fileSize = ftell(batchFile);
        if(fileSize == 0) {
            fclose(batchFile);
            fprintf(stdout, "ERROR: Batch File Empty\n");
            exit(1);
        }
        

    }
    //-f : This flag is placed before the name of the file we are analyzing.
    //check if an input file is specified
    
    //-o : This flag is placed before the name of the file we are writing to.
    //All Flags should begin with a '-' and only flags should begin with this. If there is a flag that we did NOT specify elsewhere in this doc then print this error.
    //check that files, which can only exist after a -f, -B or -o do not begin with a '-'
    for(int i = 1; i < argc; i++){
        //check that after the -f flag, the next argument is a file name
        //unless the argument before is a -f -B or -o flag, this argument should begin with a '-' and should be contained in the flags array
        if (strcmp(argv[i-1], "-f") != 0 && strcmp(argv[i-1], "-B") != 0 && strcmp(argv[i-1], "-o") != 0){
            bool isValidFlag = false;
            for (int j = 0; j < 8; j++) {
                if (strcmp(argv[i], flags[j]) == 0) {
                    isValidFlag = true;
                    break;
                }
            }
            if (argv[i][0] != '-' || !isValidFlag) {
                fprintf(stdout, "ERROR: Invalid Flag Types\n");
                exit(1);
            }
        }
        else{
            //check that the file does not begin with a '-'
            if(argv[i][0] == '-'){
                fprintf(stdout, "ERROR: Invalid Flag Types\n");
                exit(1);
            }
        }
    }
    //check batch file and what not 
   
}
void handleFlags(int argc, char *argv[]); //prototype declaration
//Entering BatchMode, which passes through the batch file line by line and calls batchFlags handleFlags on each line
void batchMode(FILE *batchFile){
    char line[256];
    while(fgets(line, sizeof(line), batchFile)){
        if(strlen(line) == 0){
            continue;
        }
        char *token = strtok(line, " \n");
        char *arguments[256]; // Array to store arguments
        int numArguments = 0;   
        while(token != NULL){
            arguments[numArguments] = token; // Store each argument
            numArguments++;
            token = strtok(NULL, " \n");
        }
        if(batchFlags(numArguments, arguments)){
            handleFlags(numArguments, arguments);
        }
    }
}

//handles flags and calls the appropriate functions
void handleFlags(int argc, char *argv[]){
    int i;
  //declare a file
    FILE *InputFile;
    FILE *OutputFile = NULL;
    bool alreadyPrinted= false;
    //need to loop through and set input file
    for(i = 0; i < argc; i++){
        if(strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "-B") == 0){
            //handle -f flag
            //open the file in read mode
            InputFile = fopen(argv[i+1], "r");
        }
    }
    for(i = 0; i < argc; i++){
        if(strcmp(argv[i], "-o") == 0){
            //handle -o flag
            OutputFile = fopen(argv[i+1], "w");
        }
    }
    for(i = 0; i < argc; i++){
        if(strcmp(argv[i], "-c") == 0){
            //handle -c flag
            //function to count the number of unique ASCII characters in the file
            countCharacters(InputFile, OutputFile, alreadyPrinted);
            alreadyPrinted = true;
        }
        else if(strcmp(argv[i], "-w") == 0){
            //handle -w flag
            //function to count the number of unique words in the file
            countWords(InputFile, OutputFile, alreadyPrinted);
            alreadyPrinted = true;
        }
        else if(strcmp(argv[i], "-l") == 0){
            //handle -l flag
            //function to count the number of lines in the file
            countLines(InputFile, OutputFile, alreadyPrinted);
            alreadyPrinted = true;
        }
        else if(strcmp(argv[i], "-Lw") == 0){
            //handle -Lw flag
            countLongestWord(InputFile, OutputFile, alreadyPrinted);
            alreadyPrinted = true;
        }
        else if(strcmp(argv[i], "-Ll") == 0){
            //handle -Lw flag
            countLongestLine(InputFile, OutputFile, alreadyPrinted);
            alreadyPrinted = true;
        }
        else if(strcmp(argv[i], "-B") == 0){
            //handle -B flag
            batchMode(InputFile);
        }
        else{
            continue;
        }
    }
}



//need to take in arguments, check the flags, deal with flags
int main(int argc, char *argv[]){
        checkFlags(argc, argv); 
        handleFlags(argc, argv);
        return 0;
}

