# WordCount
A Program, MADCounter.c that can be run on a CLI (Command Line Interface) to gather details about the text of certain files.
Implementation: MADCounter.c works as intended on all tests. 
In the main function of MADCounter.c, we first pass the command line arguments and argc to check flags, to check for any errors that may exist 
so we can display the error message and exit the program
Next, we call handleFlags, which checks the command line arguments to set the input file, and then calls the appropriate function to based on 
the flag, which may include countCharacters, countWords, countLines, countLongestWord, countLongestLine and batchMode. In all of the count 
functions, we execute commands to gain statistics on the inputfile before printing them to stdout or the output file, if one was provided. 
In batchMode, we must loop line by line over the batch to execute the commands. But first, we check that the current line in the inputfile 
(batchfile) does not have any errors by calling batchFlags. If an error exists on the current line, we print the error to stdout and continue 
to the next line in the inputfile (batchfile). If there are no errors, we call handle flags to recieve the desired output. 
The structures to keep track of the statistics(characters, words, lines, etc) are listed at the beginning of the file, along with any helper
functions that were used. 
To keep track of characters, I used a simple static array that pointed to characterInfo structures.
To keep track of words, I created wordmap, which was really just an array of wordInfo structures that dynamically grew. For lines I used 
similar structures to that of the words. 
USAGE: ./MADCounter -f <input file> -o <output file> -c -w -l -Lw -Ll
OR
./MADCounter -B <batch file>

-f : This flag is placed before the name of the file we are analyzing.

-o : This flag is used to specify an output file. If this isn't specified than all the output should go to stdout.

Statistic Flags: These will determine which kinds of analysis you should do. You will print the output in the order these flags occur. We want the output to always be Ascii alphabetically sorted order. When printing multiple statistics, print an additional newline between the statistics output.


-c : This flag means you should track each Ascii character 0-127. You should track how many times each character is used and the initial zero based position the character occurred in. You should also track the total number of chars and number of unique chars. When you print the output it should be in the following format, with a line for each character, and should only include characters you actually encountered in the document.


Total Number of Chars = <number of chars>
Total Unique Chars = <number unique chars>

Ascii Value: <ascii int>, Char: <character>, Count: <frequency>, Initial Position: <zero based initial position>\n




-w : This flag means you should track each whitespace separated string. You should track how many times each string is used and the initial zero-based position the string occurred in. This will be the string position, not the character position. You should also track the total number of words and number of unique words. When you print the output it should be in the following format for each string.


Total Number of Words: <number of words>
Total Unique Words: <number unique words>

Word: <string>, Freq: <freq int>, Initial Position: <position int>\n




-l : This flag means you should track each newline separated line. You should track how many times each line is used and the initial zero-based position the line occurred in. This will be the line number of the first occurrence of the line, not the character position. You should also track the total number of lines and number of unique lines. When you print the output it should be in the following format, with a line for each string. When you print the lines, you should remove any trailing newline characters.


Total Number of Lines: <number of lines>
Total Unique Lines: <number unique lines>

Line: <line>, Freq: <freq int>, Initial Position: <postion int>\n




-Lw : This flag means you should keep track of the longest word(s) and how long the string(s) is. You will print the output in the following format. If there is more than one word with the same length than print them in an Ascii alphabetically sorted list.


Longest Word is <length> characters long:\n
\t<word 1>\n
\t<word 2>\n
...




-Ll : This flag means you should keep track of the longest line(s) and how many characters long the line(s) is. You will print the output in the following format. If there is more than one line with the same length than print them in an Ascii alphabetically-sorted list.


Longest Line is <length> characters long:\n
\t<Line 1>\n
\t<Line 2>\n
...



Batch Mode
One of the modes of your utility should take a file with a list of different text analysis requests. Each request will be on a line by itself and will take the same form as a single run above minus the executable names.
$batch.txt

-f welcome.txt -o output.txt -c -w -l -Lw -Ll
-f hollow.txt -o output.txt -c -w -l -Lw -Ll
-f welcome.txt -o output.txt -l -w -c
-f movies.txt -o output.txt -c
-f welcome.txt -o output.txt -Lw -Ll


